/*
 * Copyright (C) 2017 Linaro.
 * Viresh Kumar <viresh.kumar@linaro.org>
 *
 * This file is released under the GPLv2.
 */

#define pr_fmt(fmt) "Boot Constraints: " fmt

#include <linux/err.h>
#include <linux/idr.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/slab.h>

#include "../base.h"
#include "core.h"

static DEFINE_IDA(pdev_index);

struct boot_constraint_pdata {
	struct device *dev;
	struct dev_boot_constraint constraint;
	int probe_failed;
	int index;
};

static void boot_constraint_remove(void *data)
{
	struct platform_device *pdev = data;
	struct boot_constraint_pdata *pdata = dev_get_platdata(&pdev->dev);

	ida_simple_remove(&pdev_index, pdata->index);
	kfree(pdata->constraint.data);
	platform_device_unregister(pdev);
}

/*
 * A platform device is added for each and every constraint, to handle
 * -EPROBE_DEFER properly.
 */
static int boot_constraint_probe(struct platform_device *pdev)
{
	struct boot_constraint_pdata *pdata = dev_get_platdata(&pdev->dev);
	struct dev_boot_constraint_info info;
	int ret;

	BUG_ON(!pdata);

	info.constraint = pdata->constraint;
	info.free_resources = boot_constraint_remove;
	info.free_resources_data = pdev;

	ret = dev_boot_constraint_add(pdata->dev, &info);
	if (ret) {
		if (ret == -EPROBE_DEFER)
			driver_enable_deferred_probe();
		else
			pdata->probe_failed = ret;
	}

	return ret;
}

static struct platform_driver boot_constraint_driver = {
	.driver = {
		.name = "boot-constraints-dev",
	},
	.probe = boot_constraint_probe,
};

static int __init boot_constraint_init(void)
{
	return platform_driver_register(&boot_constraint_driver);
}
core_initcall(boot_constraint_init);

static int _boot_constraint_add_dev(struct device *dev,
				    struct dev_boot_constraint *constraint)
{
	struct boot_constraint_pdata pdata = {
		.dev = dev,
		.constraint.type = constraint->type,
	};
	struct platform_device *pdev;
	struct boot_constraint_pdata *pdev_pdata;
	int size, ret;

	switch (constraint->type) {
	case DEV_BOOT_CONSTRAINT_CLK:
		size = sizeof(struct dev_boot_constraint_clk_info);
		break;
	case DEV_BOOT_CONSTRAINT_PM:
		size = 0;
		break;
	case DEV_BOOT_CONSTRAINT_SUPPLY:
		size = sizeof(struct dev_boot_constraint_supply_info);
		break;
	default:
		dev_err(dev, "%s: Constraint type (%d) not supported\n",
			__func__, constraint->type);
		return -EINVAL;
	}

	/* Will be freed from boot_constraint_remove() */
	pdata.constraint.data = kmemdup(constraint->data, size, GFP_KERNEL);
	if (!pdata.constraint.data)
		return -ENOMEM;

	ret = ida_simple_get(&pdev_index, 0, 256, GFP_KERNEL);
	if (ret < 0) {
		dev_err(dev, "failed to allocate index (%d)\n", ret);
		goto free;
	}

	pdata.index = ret;

	pdev = platform_device_register_data(NULL, "boot-constraints-dev", ret,
					     &pdata, sizeof(pdata));
	if (IS_ERR(pdev)) {
		dev_err(dev, "%s: Failed to create pdev (%ld)\n", __func__,
			PTR_ERR(pdev));
		ret = PTR_ERR(pdev);
		goto ida_remove;
	}

	/* Release resources if probe has failed */
	pdev_pdata = dev_get_platdata(&pdev->dev);
	if (pdev_pdata->probe_failed) {
		ret = pdev_pdata->probe_failed;
		goto remove_pdev;
	}

	return 0;

remove_pdev:
	platform_device_unregister(pdev);
ida_remove:
	ida_simple_remove(&pdev_index, pdata.index);
free:
	kfree(pdata.constraint.data);

	return ret;
}

int dev_boot_constraint_add_deferrable(struct device *dev,
			struct dev_boot_constraint *constraints, int count)
{
	int ret, i;

	if (boot_constraints_disabled)
		return -ENODEV;

	for (i = 0; i < count; i++) {
		ret = _boot_constraint_add_dev(dev, &constraints[i]);
		if (ret)
			return ret;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(dev_boot_constraint_add_deferrable);

/* This only creates platform devices for now */
int dev_boot_constraint_add_of_deferrable(const char *compatible,
			struct dev_boot_constraint *constraints, int count)
{
	struct platform_device *pdev;
	struct device_node *np;

	if (boot_constraints_disabled)
		return -ENODEV;

	np = of_find_compatible_node(NULL, NULL, compatible);
	if (!np)
		return -ENODEV;

	pdev = of_find_device_by_node(np);
	if (!pdev)
		pdev = of_platform_device_create(np, NULL, NULL);

	of_node_put(np);

	if (!pdev)
		return -ENODEV;

	return dev_boot_constraint_add_deferrable(&pdev->dev, constraints,
						  count);
}
EXPORT_SYMBOL_GPL(dev_boot_constraint_add_of_deferrable);
