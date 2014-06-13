/*
 * Copyright (c) 2015, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/thermal.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/eeprom-consumer.h>

/* TSENS register info */
#define S0_ST_ADDR(n)		((n) + 0x1030)
#define SN_ADDR_OFFSET		0x4
#define SN_ST_TEMP_MASK		0x3ff
#define SN_LOWER_ST		BIT(11)
#define SN_UPPER_ST		BIT(12)
#define ST_ADDR_OFFSET		2

#define TRDY_ADDR(n)		((n) + 0x105c)
#define TRDY_MASK		BIT(0)

#define BIT_APPEND		0x3
#define CAL_DEGC_PT1		30
#define CAL_DEGC_PT2		120
#define SLOPE_FACTOR		1000

#define ONE_PT_CALIB		0x1
#define ONE_PT_CALIB2		0x2
#define TWO_PT_CALIB		0x3

/* eeprom layout data for 8974 */
#define BASE1_MASK		0xff
#define S0_P1_MASK		0x3f00
#define S1_P1_MASK		0xfc000
#define S2_P1_MASK		0x3f00000
#define S3_P1_MASK		0xfc000000
#define S4_P1_MASK		0x3f
#define S5_P1_MASK		0xfc0
#define S6_P1_MASK		0x3f000
#define S7_P1_MASK		0xfc0000
#define S8_P1_MASK		0x3f000000
#define S8_P1_MASK_BKP		0x3f
#define S9_P1_MASK		0x3f
#define S9_P1_MASK_BKP		0xfc0
#define S10_P1_MASK		0xfc0
#define S10_P1_MASK_BKP		0x3f000
#define CAL_SEL_0_1		0xc0000000
#define CAL_SEL_2		0x40000000
#define CAL_SEL_SHIFT		30
#define CAL_SEL_SHIFT_2		28

#define S0_P1_SHIFT		8
#define S1_P1_SHIFT		14
#define S2_P1_SHIFT		20
#define S3_P1_SHIFT		26
#define S5_P1_SHIFT		6
#define S6_P1_SHIFT		12
#define S7_P1_SHIFT		18
#define S8_P1_SHIFT		24
#define S9_P1_BKP_SHIFT		6
#define S10_P1_SHIFT		6
#define S10_P1_BKP_SHIFT	12

#define BASE2_SHIFT		12
#define BASE2_BKP_SHIFT		18
#define S0_P2_SHIFT		20
#define S0_P2_BKP_SHIFT		26
#define S1_P2_SHIFT		26
#define S2_P2_BKP_SHIFT		6
#define S3_P2_SHIFT		6
#define S3_P2_BKP_SHIFT		12
#define S4_P2_SHIFT		12
#define S4_P2_BKP_SHIFT		18
#define S5_P2_SHIFT		18
#define S5_P2_BKP_SHIFT		24
#define S6_P2_SHIFT		24
#define S7_P2_BKP_SHIFT		6
#define S8_P2_SHIFT		6
#define S8_P2_BKP_SHIFT		12
#define S9_P2_SHIFT		12
#define S9_P2_BKP_SHIFT		18
#define S10_P2_SHIFT		18
#define S10_P2_BKP_SHIFT	24

#define BASE2_MASK		0xff000
#define BASE2_BKP_MASK		0xfc0000
#define S0_P2_MASK		0x3f00000
#define S0_P2_BKP_MASK		0xfc000000
#define S1_P2_MASK		0xfc000000
#define S1_P2_BKP_MASK		0x3f
#define S2_P2_MASK		0x3f
#define S2_P2_BKP_MASK		0xfc0
#define S3_P2_MASK		0xfc0
#define S3_P2_BKP_MASK		0x3f000
#define S4_P2_MASK		0x3f000
#define S4_P2_BKP_MASK		0xfc0000
#define S5_P2_MASK		0xfc0000
#define S5_P2_BKP_MASK		0x3f000000
#define S6_P2_MASK		0x3f000000
#define S6_P2_BKP_MASK		0x3f
#define S7_P2_MASK		0x3f
#define S7_P2_BKP_MASK		0xfc0
#define S8_P2_MASK		0xfc0
#define S8_P2_BKP_MASK		0x3f000
#define S9_P2_MASK		0x3f000
#define S9_P2_BKP_MASK		0xfc0000
#define S10_P2_MASK		0xfc0000
#define S10_P2_BKP_MASK		0x3f000000

#define BKP_SEL			0x3
#define BKP_REDUN_SEL		0xe0000000
#define BKP_REDUN_SHIFT		29

/* TSENS register data */
#define TSENS_THRESHOLD_MAX_CODE	0x3ff
#define TSENS_THRESHOLD_MIN_CODE	0x0

struct tsens_device;

struct tsens_sensor {
	struct thermal_zone_device	*tzd;
	enum thermal_device_mode	mode;
	unsigned int			id;
	int				offset;
	u32				slope;
	struct tsens_device		*tmdev;
	u32				status;
};

struct tsens_device {
	struct device			*dev;
	u32				num_sensor;
	void __iomem			*base;
	struct tsens_sensor		sensor[0];
};

static int tsens_tz_code_to_degc(u32 adc_code, const struct tsens_sensor *s)
{
	int degc, num, den;

	num = (adc_code * SLOPE_FACTOR) - s->offset;
	den = s->slope;

	if (num > 0)
		degc = num + (den / 2);
	else if (num < 0)
		degc = num - (den / 2);
	else
		degc = num;

	degc /= den;

	return degc;
}

static int tsens_tz_get_temp(void *data, long *temp)
{
	struct tsens_sensor *sensor = data;
	struct tsens_device *tmdev = sensor->tmdev;
	int id = sensor->id;
	unsigned int code;
	void __iomem *sensor_addr;
	void __iomem *trdy_addr;
	int last_temp = 0;

	trdy_addr = TRDY_ADDR(tmdev->base);
	sensor_addr = S0_ST_ADDR(tmdev->base) + id * 4;

	code = readl_relaxed(sensor_addr);
	last_temp = code & SN_ST_TEMP_MASK;

	*temp = tsens_tz_code_to_degc(last_temp, sensor);

	return 0;
}

static void compute_intercept_slope(struct tsens_device *tmdev, u32 *p1,
				    u32 *p2, u32 mode)
{
	int i;
	int num, den;

	for (i = 0; i < tmdev->num_sensor; i++) {
		dev_dbg(tmdev->dev,
			"sensor%d - data_point1:%#x data_point2:%#x\n",
			i, p1[i], p2[i]);

		if (mode == TWO_PT_CALIB) {
			/* slope (m) = adc_code2 - adc_code1 (y2 - y1)/
				temp_120_degc - temp_30_degc (x2 - x1) */
			num = p2[i] - p1[i];
			num *= SLOPE_FACTOR;
			den = CAL_DEGC_PT2 - CAL_DEGC_PT1;
			tmdev->sensor[i].slope = num / den;
		}

		tmdev->sensor[i].offset = (p1[i] * SLOPE_FACTOR) -
				(CAL_DEGC_PT1 *
				tmdev->sensor[i].slope);
		dev_dbg(tmdev->dev, "offset:%d\n", tmdev->sensor[i].offset);
	}
}

static inline u32 *qfprom_read(struct device *dev, const char *cname)
{
	struct eeprom_cell *cell;
	ssize_t data;


	cell = of_eeprom_cell_get_byname(dev, cname);
	if (IS_ERR(cell))
		return (u32 *)cell;

	return (u32 *)eeprom_cell_read(cell, &data);
}

static int tsens_calib_8974_sensors(struct tsens_device *tmdev)
{
	int base1 = 0, base2 = 0, i;
	u32 p1[11], p2[11];
	int mode = 0;
	u32 *calib, *bkp;
	u32 calib_redun_sel;

	calib = qfprom_read(tmdev->dev, "calib_data");
	if (IS_ERR(calib))
		return PTR_ERR(calib);

	bkp = qfprom_read(tmdev->dev, "calib_backup");
	if (IS_ERR(bkp))
		return PTR_ERR(bkp);

	calib_redun_sel =  bkp[1] & BKP_REDUN_SEL;
	calib_redun_sel >>= BKP_REDUN_SHIFT;

	if (calib_redun_sel == BKP_SEL) {
		mode = (calib[4] & CAL_SEL_0_1) >> CAL_SEL_SHIFT;
		mode |= (calib[5] & CAL_SEL_2) >> CAL_SEL_SHIFT_2;

		switch (mode) {
		case TWO_PT_CALIB:
			base2 = (bkp[2] & BASE2_BKP_MASK) >> BASE2_BKP_SHIFT;
			p2[0] = (bkp[2] & S0_P2_BKP_MASK) >> S0_P2_BKP_SHIFT;
			p2[1] = (bkp[3] & S1_P2_BKP_MASK);
			p2[2] = (bkp[3] & S2_P2_BKP_MASK) >> S2_P2_BKP_SHIFT;
			p2[3] = (bkp[3] & S3_P2_BKP_MASK) >> S3_P2_BKP_SHIFT;
			p2[4] = (bkp[3] & S4_P2_BKP_MASK) >> S4_P2_BKP_SHIFT;
			p2[5] = (calib[4] & S5_P2_BKP_MASK) >> S5_P2_BKP_SHIFT;
			p2[6] = (calib[5] & S6_P2_BKP_MASK);
			p2[7] = (calib[5] & S7_P2_BKP_MASK) >> S7_P2_BKP_SHIFT;
			p2[8] = (calib[5] & S8_P2_BKP_MASK) >> S8_P2_BKP_SHIFT;
			p2[9] = (calib[5] & S9_P2_BKP_MASK) >> S9_P2_BKP_SHIFT;
			p2[10] = (calib[5] & S10_P2_BKP_MASK) >> S10_P2_BKP_SHIFT;
			/* Fall through */
		case ONE_PT_CALIB:
		case ONE_PT_CALIB2:
			base1 = bkp[0] & BASE1_MASK;
			p1[0] = (bkp[0] & S0_P1_MASK) >> S0_P1_SHIFT;
			p1[1] = (bkp[0] & S1_P1_MASK) >> S1_P1_SHIFT;
			p1[2] = (bkp[0] & S2_P1_MASK) >> S2_P1_SHIFT;
			p1[3] = (bkp[0] & S3_P1_MASK) >> S3_P1_SHIFT;
			p1[4] = (bkp[1] & S4_P1_MASK);
			p1[5] = (bkp[1] & S5_P1_MASK) >> S5_P1_SHIFT;
			p1[6] = (bkp[1] & S6_P1_MASK) >> S6_P1_SHIFT;
			p1[7] = (bkp[1] & S7_P1_MASK) >> S7_P1_SHIFT;
			p1[8] = (bkp[2] & S8_P1_MASK_BKP) >> S8_P1_SHIFT;
			p1[9] = (bkp[2] & S9_P1_MASK_BKP) >> S9_P1_BKP_SHIFT;
			p1[10] = (bkp[2] & S10_P1_MASK_BKP) >> S10_P1_BKP_SHIFT;
			break;
		}
	} else {
		mode = (calib[1] & CAL_SEL_0_1) >> CAL_SEL_SHIFT;
		mode |= (calib[3] & CAL_SEL_2) >> CAL_SEL_SHIFT_2;

		switch (mode) {
		case TWO_PT_CALIB:
			base2 = (calib[2] & BASE2_MASK) >> BASE2_SHIFT;
			p2[0] = (calib[2] & S0_P2_MASK) >> S0_P2_SHIFT;
			p2[1] = (calib[2] & S1_P2_MASK) >> S1_P2_SHIFT;
			p2[2] = (calib[3] & S2_P2_MASK);
			p2[3] = (calib[3] & S3_P2_MASK) >> S3_P2_SHIFT;
			p2[4] = (calib[3] & S4_P2_MASK) >> S4_P2_SHIFT;
			p2[5] = (calib[3] & S5_P2_MASK) >> S5_P2_SHIFT;
			p2[6] = (calib[3] & S6_P2_MASK) >> S6_P2_SHIFT;
			p2[7] = (calib[4] & S7_P2_MASK);
			p2[8] = (calib[4] & S8_P2_MASK) >> S8_P2_SHIFT;
			p2[9] = (calib[4] & S9_P2_MASK) >> S9_P2_SHIFT;
			p2[10] = (calib[4] & S10_P2_MASK) >> S10_P2_SHIFT;
			/* Fall through */
		case ONE_PT_CALIB:
		case ONE_PT_CALIB2:
			base1 = calib[0] & BASE1_MASK;
			p1[0] = (calib[0] & S0_P1_MASK) >> S0_P1_SHIFT;
			p1[1] = (calib[0] & S1_P1_MASK) >> S1_P1_SHIFT;
			p1[2] = (calib[0] & S2_P1_MASK) >> S2_P1_SHIFT;
			p1[3] = (calib[0] & S3_P1_MASK) >> S3_P1_SHIFT;
			p1[4] = (calib[1] & S4_P1_MASK);
			p1[5] = (calib[1] & S5_P1_MASK) >> S5_P1_SHIFT;
			p1[6] = (calib[1] & S6_P1_MASK) >> S6_P1_SHIFT;
			p1[7] = (calib[1] & S7_P1_MASK) >> S7_P1_SHIFT;
			p1[8] = (calib[1] & S8_P1_MASK) >> S8_P1_SHIFT;
			p1[9] = (calib[2] & S9_P1_MASK);
			p1[10] = (calib[2] & S10_P1_MASK) >> S10_P1_SHIFT;
			break;
		}
	}

	switch (mode) {
	case ONE_PT_CALIB:
		for (i = 0; i < tmdev->num_sensor; i++)
			p1[i] += (base1 << 2) | BIT_APPEND;
		break;
	case TWO_PT_CALIB:
		for (i = 0; i < tmdev->num_sensor; i++) {
			p2[i] += base2;
			p2[i] <<= 2;
			p2[i] |= BIT_APPEND;
		}
		/* Fall through */
	case ONE_PT_CALIB2:
		for (i = 0; i < tmdev->num_sensor; i++) {
			p1[i] += base1;
			p1[i] <<= 2;
			p1[i] |= BIT_APPEND;
		}
		break;
	default:
		for (i = 0; i < tmdev->num_sensor; i++)
			p2[i] = 780;
		p1[0] = 502;
		p1[1] = 509;
		p1[2] = 503;
		p1[3] = 509;
		p1[4] = 505;
		p1[5] = 509;
		p1[6] = 507;
		p1[7] = 510;
		p1[8] = 508;
		p1[9] = 509;
		p1[10] = 508;
		break;
	}

	compute_intercept_slope(tmdev, p1, p2, mode);

	return 0;
}

static const struct thermal_zone_of_device_ops tsens_thermal_of_ops = {
	.get_temp = tsens_tz_get_temp,
};

static const struct of_device_id tsens_match_table[] = {
	{
		.compatible = "qcom,msm8974-tsens",
		.data = tsens_calib_8974_sensors,
	},
	{}
};
MODULE_DEVICE_TABLE(of, tsens_match_table);

static int tsens_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	int ret, i, num;
	struct tsens_sensor *s;
	struct tsens_device *tmdev;

	const struct of_device_id *id;
	struct thermal_zone_device *tzd;
	int (*tsens_cal_func)(struct tsens_device *);

	num = of_property_count_u32_elems(np, "qcom,tsens-slopes");
	if (num <= 0) {
		dev_err(&pdev->dev, "invalid tsens slopes\n");
		return -EINVAL;
	}

	tmdev = devm_kzalloc(&pdev->dev, sizeof(*tmdev) +
			     num * sizeof(*s), GFP_KERNEL);
	if (!tmdev)
		return -ENOMEM;

	tmdev->dev = &pdev->dev;
	tmdev->num_sensor = num;

	for (i = 0, s = tmdev->sensor; i < tmdev->num_sensor; i++, s++)
		of_property_read_u32_index(np, "qcom,tsens-slopes", i,
					   &s->slope);

	tmdev->base = of_iomap(np, 0);
	if (IS_ERR(tmdev->base))
		return -EINVAL;

	id = of_match_node(tsens_match_table, np);
	if (!id)
		return -ENODEV;

	tsens_cal_func = id->data;
	ret = tsens_cal_func(tmdev);
	if (ret < 0) {
		dev_err(&pdev->dev, "tsense calibration failed\n");
		return ret;
	}

	for (i = 0; i < tmdev->num_sensor; i++) {
		tmdev->sensor[i].tmdev = tmdev;
		tzd = thermal_zone_of_sensor_register(&pdev->dev, i,
						      &tmdev->sensor[i],
						      &tsens_thermal_of_ops);
		if (IS_ERR(tzd))
			continue;
		tmdev->sensor[i].tzd = tzd;
	}

	platform_set_drvdata(pdev, tmdev);
	return 0;
}

static int tsens_remove(struct platform_device *pdev)
{
	int i;
	struct tsens_device *tmdev = platform_get_drvdata(pdev);
	struct thermal_zone_device *tzd;

	for (i = 0; i < tmdev->num_sensor; i++) {
		tzd = tmdev->sensor[i].tzd;
		thermal_zone_of_sensor_unregister(&pdev->dev, tzd);
	}
	return 0;
}

static struct platform_driver tsens_driver = {
	.probe = tsens_probe,
	.remove = tsens_remove,
	.driver = {
		.name = "qcom-tsens-b",
		.of_match_table = tsens_match_table,
	},
};
module_platform_driver(tsens_driver);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("QCOM Temperature Sensor driver for B family");
MODULE_ALIAS("platform:qcom-tsens-b");
