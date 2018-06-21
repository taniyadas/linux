// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2018, The Linux Foundation. All rights reserved.
 */

#include <linux/list.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/pm_domain.h>
#include <linux/pm_runtime.h>
#include <linux/slab.h>

#include "clk-pd.h"
#include "clk-regmap.h"

struct clk_powerdomain {
	struct list_head list;
	struct clk_powerdomain_class *pd;
};

static LIST_HEAD(clk_pd_list);

/* Find the corner required for a given clock rate */
static int find_rate_to_corner(struct clk_regmap *rclk, unsigned long rate)
{
	int corner;

	for (corner = 0; corner < rclk->pd->num_corners; corner++)
		if (rate <= rclk->rate_max[corner])
			break;

	if (corner == rclk->pd->num_corners) {
		pr_debug("Rate %lu for %s is > than highest Fmax\n", rate,
			 rclk->hw.init->name);
		return -EINVAL;
	}

	return corner;
}

static int genpd_update_corner_state(struct clk_powerdomain_class *pd)
{
	int corner, ret, *state = pd->corner, i;
	int cur_corner = pd->cur_corner, max_corner = pd->num_corners - 1;

	/* Aggregate the corner */
	for (corner = max_corner; corner > 0; corner--) {
		if (pd->corner_votes[corner])
			break;
	}

	if (corner == cur_corner)
		return 0;

	pr_debug("Set performance state to genpd(%s) for state %d, cur_corner %d, num_corner %d\n",
		 pd->pd_name, state[corner], cur_corner, pd->num_corners);

	for (i = 0; i < pd->num_pd; i++) {
		ret = dev_pm_genpd_set_performance_state(pd->powerdomain_dev[i],
							 state[corner]);
		if (ret)
			return ret;

		if (cur_corner == 0 || cur_corner == pd->num_corners) {
			pd->links[i] = device_link_add(pd->dev,
					pd->powerdomain_dev[i],
					DL_FLAG_STATELESS |
					DL_FLAG_PM_RUNTIME |
					DL_FLAG_RPM_ACTIVE);
			if (!pd->links[i])
				pr_err("Links for %d not created\n", i);
		}

		if (corner == 0)
			device_link_del(pd->links[i]);
	}

	pd->cur_corner = corner;

	return 0;
}

/* call from prepare & set rate */
int clk_power_domain_vote_rate(struct clk_regmap *rclk,
				unsigned long rate)
{
	int corner;

	if (!rclk->pd)
		return 0;

	corner = find_rate_to_corner(rclk, rate);
	if (corner < 0)
		return corner;

	mutex_lock(&rclk->pd->lock);

	rclk->pd->corner_votes[corner]++;

	/* update the corner to power domain */
	if (genpd_update_corner_state(rclk->pd) < 0)
		rclk->pd->corner_votes[corner]--;

	pr_debug("genpd(%s) prepare corner_votes_count %d, corner %d\n",
		 rclk->pd->pd_name, rclk->pd->corner_votes[corner],
		 corner);

	mutex_unlock(&rclk->pd->lock);

	return 0;
}
EXPORT_SYMBOL_GPL(clk_power_domain_vote_rate);

/* call from unprepare & set rate */
void clk_power_domain_unvote_rate(struct clk_regmap *rclk,
				   unsigned long rate)
{
	int corner;

	if (!rclk->pd)
		return;

	corner = find_rate_to_corner(rclk, rate);
	if (corner < 0)
		return;

	if (WARN(!rclk->pd->corner_votes[corner],
		 "Reference counts are incorrect for %s corner %d\n",
		 rclk->pd->pd_name, corner))
		return;

	mutex_lock(&rclk->pd->lock);

	rclk->pd->corner_votes[corner]--;

	if (genpd_update_corner_state(rclk->pd) < 0)
		rclk->pd->corner_votes[corner]++;

	pr_debug("genpd(%s) unprepare corner_votes_count %d, corner %d\n",
		 rclk->pd->pd_name, rclk->pd->corner_votes[corner],
		 corner);

	mutex_unlock(&rclk->pd->lock);
}
EXPORT_SYMBOL_GPL(clk_power_domain_unvote_rate);

int clk_pd_class_init(struct device *dev, struct clk_powerdomain_class *pd)
{
	struct clk_powerdomain *pwrd;
	int i, num_domains;

	if (!pd) {
		pr_debug("genpd not defined\n");
		return 0;
	}

	/* Deal only with devices using multiple PM domains. */
	num_domains = of_count_phandle_with_args(dev->of_node, "power-domains",
						 "#power-domain-cells");

	list_for_each_entry(pwrd, &clk_pd_list, list) {
		if (pwrd->pd == pd)
			return 0;
	}

	pr_debug("Voting for genpd_class %s\n", pd->pd_name);

	if (num_domains == 1) {
		pd->powerdomain_dev[0] = dev;
	} else {
		for (i = 0; i < pd->num_pd; i++) {
			int index = pd->pd_index[i];

			pd->powerdomain_dev[i] = genpd_dev_pm_attach_by_id(dev,
									index);
		}
	}

	pwrd = kmalloc(sizeof(*pwrd), GFP_KERNEL);
	if (!pwrd)
		return -ENOMEM;

	pwrd->pd = pd;
	list_add_tail(&pwrd->list, &clk_pd_list);

	pd->dev = dev;

	return 0;
}
EXPORT_SYMBOL_GPL(clk_pd_class_init);

MODULE_LICENSE("GPL v2");
