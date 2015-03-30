/*
 * Copyright (C) 2015 Srinivas Kandagatla <srinivas.kandagatla@linaro.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/device.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include "eeprom-mmio.h"

int eeprom_mmio_remove(struct platform_device *pdev)
{
	struct eeprom_device *eeprom = platform_get_drvdata(pdev);

	return eeprom_unregister(eeprom);
}
EXPORT_SYMBOL_GPL(eeprom_mmio_remove);

int eeprom_mmio_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct resource *res;
	const struct eeprom_mmio_data *data;
	struct eeprom_device *eeprom;
	struct regmap *regmap;
	const struct of_device_id *match;
	void __iomem *base;

	if (!dev || !dev->driver)
		return -ENODEV;

	match = of_match_device(dev->driver->of_match_table, dev);
	if (!match || !match->data)
		return -EINVAL;

	data = match->data;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	base = devm_ioremap_resource(dev, res);
	if (IS_ERR(base))
		return PTR_ERR(base);

	data->regmap_config->max_register = resource_size(res) - 1;

	regmap = devm_regmap_init_mmio(dev, base, data->regmap_config);
	if (IS_ERR(regmap)) {
		dev_err(dev, "regmap init failed\n");
		return PTR_ERR(regmap);
	}
	data->eeprom_config->dev = dev;
	eeprom = eeprom_register(data->eeprom_config);
	if (IS_ERR(eeprom))
		return PTR_ERR(eeprom);

	platform_set_drvdata(pdev, eeprom);

	return 0;
}
EXPORT_SYMBOL_GPL(eeprom_mmio_probe);
