/*
 * Copyright (C) 2017 Linaro.
 * Viresh Kumar <viresh.kumar@linaro.org>
 *
 * This file is released under the GPLv2.
 */

#define pr_fmt(fmt) "PM Boot Constraints: " fmt

#include <linux/pm_domain.h>

#include "core.h"

int constraint_pm_add(struct constraint *constraint, void *data)
{
	struct device *dev = constraint->cdev->dev;
	int ret;

	ret = dev_pm_domain_attach(dev, true);
	if (ret)
		return ret;

	/* Debugfs */
	constraint_add_debugfs(constraint, "domain");

	return 0;
}

void constraint_pm_remove(struct constraint *constraint)
{
	constraint_remove_debugfs(constraint);
}
