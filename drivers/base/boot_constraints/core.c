/*
 * This takes care of boot time device constraints, normally set by the
 * Bootloader.
 *
 * Copyright (C) 2017 Linaro.
 * Viresh Kumar <viresh.kumar@linaro.org>
 *
 * This file is released under the GPLv2.
 */

#define pr_fmt(fmt) "Boot Constraints: " fmt

#include <linux/err.h>
#include <linux/export.h>
#include <linux/mutex.h>
#include <linux/slab.h>

#include "core.h"

#define for_each_constraint(_constraint, _temp, _cdev)		\
	list_for_each_entry_safe(_constraint, _temp, &_cdev->constraints, node)

/* Global list of all constraint devices currently registered */
static LIST_HEAD(constraint_devices);
static DEFINE_MUTEX(constraint_devices_mutex);

static bool boot_constraints_disabled;

static int __init constraints_disable(char *str)
{
	boot_constraints_disabled = true;
	pr_debug("disabled\n");

	return 0;
}
early_param("boot_constraints_disable", constraints_disable);

/* Boot constraints core */

static struct constraint_dev *constraint_device_find(struct device *dev)
{
	struct constraint_dev *cdev;

	list_for_each_entry(cdev, &constraint_devices, node) {
		if (cdev->dev == dev)
			return cdev;
	}

	return NULL;
}

static struct constraint_dev *constraint_device_allocate(struct device *dev)
{
	struct constraint_dev *cdev;

	cdev = kzalloc(sizeof(*cdev), GFP_KERNEL);
	if (!cdev)
		return ERR_PTR(-ENOMEM);

	cdev->dev = dev;
	INIT_LIST_HEAD(&cdev->node);
	INIT_LIST_HEAD(&cdev->constraints);

	list_add(&cdev->node, &constraint_devices);

	return cdev;
}

static void constraint_device_free(struct constraint_dev *cdev)
{
	list_del(&cdev->node);
	kfree(cdev);
}

static struct constraint_dev *constraint_device_get(struct device *dev)
{
	struct constraint_dev *cdev;

	cdev = constraint_device_find(dev);
	if (cdev)
		return cdev;

	cdev = constraint_device_allocate(dev);
	if (IS_ERR(cdev)) {
		dev_err(dev, "Failed to add constraint dev (%ld)\n",
			PTR_ERR(cdev));
	}

	return cdev;
}

static void constraint_device_put(struct constraint_dev *cdev)
{
	if (!list_empty(&cdev->constraints))
		return;

	constraint_device_free(cdev);
}

static struct constraint *constraint_allocate(struct constraint_dev *cdev,
					enum dev_boot_constraint_type type)
{
	struct constraint *constraint;
	int (*add)(struct constraint *constraint, void *data);
	void (*remove)(struct constraint *constraint);

	switch (type) {
	case DEV_BOOT_CONSTRAINT_CLK:
		add = constraint_clk_add;
		remove = constraint_clk_remove;
		break;
	case DEV_BOOT_CONSTRAINT_PM:
		add = constraint_pm_add;
		remove = constraint_pm_remove;
		break;
	case DEV_BOOT_CONSTRAINT_SUPPLY:
		add = constraint_supply_add;
		remove = constraint_supply_remove;
		break;
	default:
		return ERR_PTR(-EINVAL);
	}

	constraint = kzalloc(sizeof(*constraint), GFP_KERNEL);
	if (!constraint)
		return ERR_PTR(-ENOMEM);

	constraint->cdev = cdev;
	constraint->type = type;
	constraint->add = add;
	constraint->remove = remove;
	INIT_LIST_HEAD(&constraint->node);

	list_add(&constraint->node, &cdev->constraints);

	return constraint;
}

static void constraint_free(struct constraint *constraint)
{
	list_del(&constraint->node);
	kfree(constraint);
}

int dev_boot_constraint_add(struct device *dev,
			    struct dev_boot_constraint_info *info)
{
	struct constraint_dev *cdev;
	struct constraint *constraint;
	int ret;

	if (boot_constraints_disabled)
		return -ENODEV;

	mutex_lock(&constraint_devices_mutex);

	/* Find or add the cdev type first */
	cdev = constraint_device_get(dev);
	if (IS_ERR(cdev)) {
		ret = PTR_ERR(cdev);
		goto unlock;
	}

	constraint = constraint_allocate(cdev, info->constraint.type);
	if (IS_ERR(constraint)) {
		dev_err(dev, "Failed to add constraint type: %d (%ld)\n",
			info->constraint.type, PTR_ERR(constraint));
		ret = PTR_ERR(constraint);
		goto put_cdev;
	}

	constraint->free_resources = info->free_resources;
	constraint->free_resources_data = info->free_resources_data;

	/* Set constraint */
	ret = constraint->add(constraint, info->constraint.data);
	if (ret)
		goto free_constraint;

	dev_dbg(dev, "Added boot constraint-type (%d)\n",
		info->constraint.type);

	mutex_unlock(&constraint_devices_mutex);

	return 0;

free_constraint:
	constraint_free(constraint);
put_cdev:
	constraint_device_put(cdev);
unlock:
	mutex_unlock(&constraint_devices_mutex);

	return ret;
}
EXPORT_SYMBOL_GPL(dev_boot_constraint_add);

static void constraint_remove(struct constraint *constraint)
{
	constraint->remove(constraint);

	if (constraint->free_resources)
		constraint->free_resources(constraint->free_resources_data);

	constraint_free(constraint);
}

void dev_boot_constraints_remove(struct device *dev)
{
	struct constraint_dev *cdev;
	struct constraint *constraint, *temp;

	if (boot_constraints_disabled)
		return;

	mutex_lock(&constraint_devices_mutex);

	cdev = constraint_device_find(dev);
	if (!cdev)
		goto unlock;

	for_each_constraint(constraint, temp, cdev)
		constraint_remove(constraint);

	constraint_device_put(cdev);
unlock:
	mutex_unlock(&constraint_devices_mutex);
}
