/*
 * Copyright (c) 2017, The Linux Foundation. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/err.h>
#include <linux/export.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pm_domain.h>
#include <linux/mfd/qcom_rpm.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/soc/qcom/smd-rpm.h>

#include <dt-bindings/mfd/qcom-rpm.h>

#define domain_to_rpmpd(domain) container_of(domain, struct rpmpd, pd)
#define KEY_CORNER 0x6e657773

struct rpmpd_req {
	__le32 key;
	__le32 nbytes;
	__le32 value;
};

struct rpmpd {
	struct generic_pm_domain pd;
	const char *res_name;
	const int res_type;
	const int res_id;
	struct qcom_smd_rpm *rpm;
};

struct rpmpd_desc {
	struct rpmpd **rpmpds;
	size_t num_pds;
};

/* msm8996 RPM powerdomains */
static struct rpmpd msm8996_vddcx_pd = {
	.res_name = "smpa",
	.res_type = 1,
	.res_id = 1,
};

static struct rpmpd msm8996_vddmx_pd = {
	.res_name = "smpa",
	.res_type = 1,
	.res_id = 2,
};

static struct rpmpd *msm8996_rpmpds[] = {
	[0] = &msm8996_vddcx_pd,
	[1] = &msm8996_vddmx_pd,
};

static const struct rpmpd_desc msm8996_desc = {
	.rpmpds = msm8996_rpmpds,
	.num_pds = ARRAY_SIZE(msm8996_rpmpds),
};

static const struct of_device_id rpmpd_match_table[] = {
	{ .compatible = "qcom,rpmpd-msm8996", .data = &msm8996_desc },
	{ }
};
MODULE_DEVICE_TABLE(of, rpmpd_match_table);

static int rpmpd_power_on(struct generic_pm_domain *domain)
{
	/* Nothing to be done for RPM powerdomains */
	pr_debug("%s: %d\n", __func__, __LINE__);
	return 0;
}

static int rpmpd_power_off(struct generic_pm_domain *domain)
{
	/* Nothing to be done for RPM powerdomains */
	pr_debug("%s: %d\n", __func__, __LINE__);
	return 0;
}

static int rpmpd_performance(struct generic_pm_domain *domain,
			     unsigned int state)
{
	int ret;
	struct rpmpd *pd = domain_to_rpmpd(domain);
	struct rpmpd_req req = {
		.key = KEY_CORNER,
		.nbytes = cpu_to_le32(sizeof(u32)),
		.value = cpu_to_le32(state),
	};

	/* Send the performace state to RPM */
	ret = qcom_rpm_smd_write(pd->rpm, QCOM_RPM_ACTIVE_STATE, pd->res_type,
				 pd->res_id, &req, sizeof(req));

	pr_info("%s: %d: %d\n", __func__, __LINE__, state);

	return 0;
}

static int rpmpd_probe(struct platform_device *pdev)
{
	int i;
	size_t num;
	struct genpd_onecell_data *data;
	struct qcom_smd_rpm *rpm;
	struct rpmpd **rpmpds;
	const struct rpmpd_desc *desc;

	rpm = dev_get_drvdata(pdev->dev.parent);
	if (!rpm) {
		dev_err(&pdev->dev, "Unable to retrieve handle to RPM\n");
		return -ENODEV;
	}

	desc = of_device_get_match_data(&pdev->dev);
	if (!desc)
		return -EINVAL;

	rpmpds = desc->rpmpds;
	num = desc->num_pds;

	data = devm_kzalloc(&pdev->dev, sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	data->domains = devm_kcalloc(&pdev->dev, num, sizeof(*data->domains),
				     GFP_KERNEL);
	data->num_domains = num;

	for (i = 0; i < num; i++) {
		if (!rpmpds[i])
			continue;

		rpmpds[i]->rpm = rpm;
		rpmpds[i]->pd.power_off = rpmpd_power_off;
		rpmpds[i]->pd.power_on = rpmpd_power_on;
		rpmpds[i]->pd.set_performance_state = rpmpd_performance;
		pm_genpd_init(&rpmpds[i]->pd, NULL, false);

		data->domains[i] = &rpmpds[i]->pd;
	}

	return of_genpd_add_provider_onecell(pdev->dev.of_node, data);
}

static int rpmpd_remove(struct platform_device *pdev)
{
	of_genpd_del_provider(pdev->dev.of_node);
	return 0;
}

static struct platform_driver rpmpd_driver = {
	.driver = {
		.name = "qcom-rpmpd",
		.of_match_table = rpmpd_match_table,
	},
	.probe = rpmpd_probe,
	.remove = rpmpd_remove,
};

static int __init rpmpd_init(void)
{
	return platform_driver_register(&rpmpd_driver);
}
core_initcall(rpmpd_init);

static void __exit rpmpd_exit(void)
{
	platform_driver_unregister(&rpmpd_driver);
}
module_exit(rpmpd_exit);

MODULE_DESCRIPTION("Qualcomm RPM Power Domain Driver");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:qcom-rpmpd");
