/*
 * MMIO based EEPROM providers.
 *
 * Copyright (C) 2015 Srinivas Kandagatla <srinivas.kandagatla@linaro.org>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#ifndef _LINUX_EEPROM_MMIO_H
#define _LINUX_EEPROM_MMIO_H

#include <linux/platform_device.h>
#include <linux/eeprom-provider.h>
#include <linux/regmap.h>

struct eeprom_mmio_data {
	struct regmap_config *regmap_config;
	struct eeprom_config *eeprom_config;
};

#if IS_ENABLED(CONFIG_EEPROM)

int eeprom_mmio_probe(struct platform_device *pdev);
int eeprom_mmio_remove(struct platform_device *pdev);

#else

static inline int eeprom_mmio_probe(struct platform_device *pdev)
{
	return -ENOSYS;
}

static inline int eeprom_mmio_remove(struct platform_device *pdev)
{
	return -ENOSYS;
}
#endif

#endif  /* ifndef _LINUX_EEPROM_MMIO_H */
