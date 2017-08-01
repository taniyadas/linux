/*
 * Boot constraints header.
 *
 * Copyright (C) 2017 Linaro.
 * Viresh Kumar <viresh.kumar@linaro.org>
 *
 * This file is released under the GPLv2
 */
#ifndef _LINUX_BOOT_CONSTRAINT_H
#define _LINUX_BOOT_CONSTRAINT_H

#include <linux/err.h>
#include <linux/types.h>

struct device;

enum dev_boot_constraint_type {
	DEV_BOOT_CONSTRAINT_CLK,
	DEV_BOOT_CONSTRAINT_PM,
	DEV_BOOT_CONSTRAINT_SUPPLY,
};

struct dev_boot_constraint_clk_info {
	const char *name;
};

struct dev_boot_constraint_supply_info {
	const char *name;
	unsigned int u_volt_min;
	unsigned int u_volt_max;
};

struct dev_boot_constraint {
	enum dev_boot_constraint_type type;
	void *data;
};

struct dev_boot_constraint_info {
	struct dev_boot_constraint constraint;

	/* This will be called just before the constraint is removed */
	void (*free_resources)(void *data);
	void *free_resources_data;
};

#ifdef CONFIG_DEV_BOOT_CONSTRAINTS
int dev_boot_constraint_add(struct device *dev, struct dev_boot_constraint_info *info);
void dev_boot_constraints_remove(struct device *dev);
#else
static inline int dev_boot_constraint_add(struct device *dev,
				      struct dev_boot_constraint_info *info)
{ return -EINVAL; }
static inline void dev_boot_constraints_remove(struct device *dev) {}
#endif /* CONFIG_DEV_BOOT_CONSTRAINTS */

#endif /* _LINUX_BOOT_CONSTRAINT_H */
