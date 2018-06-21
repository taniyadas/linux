// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2018, The Linux Foundation. All rights reserved.
 */

#include <linux/list.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/pm_domain.h>
#include <linux/slab.h>

#include "clk-genpd.h"
#include "clk-regmap.h"

struct clk_genpd {
	struct list_head list;
	struct clk_genpd_class *genpd;
};

static LIST_HEAD(clk_genpd_list);

/* Find the corner required for a given clock rate */
static int find_rate_to_corner(struct clk_regmap *rclk, unsigned long rate)
{
	int corner;

	for (corner = 0; corner < rclk->genpd->num_corners; corner++)
		if (rate <= rclk->rate_max[corner])
			break;

	if (corner == rclk->genpd->num_corners) {
		pr_debug("Rate %lu for %s is > than highest Fmax\n", rate,
			 rclk->hw.init->name);
		return -EINVAL;
	}

	return corner;
}

static int genpd_update_corner_state(struct clk_genpd_class *pd)
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

	pr_debug("Set performance state to genpd(%s) for state %d\n",
		 pd->pd_name, state[corner]);

	for (i = 0; i < pd->num_genpd; i++) {
		ret = dev_pm_genpd_set_performance_state(pd->genpd_dev[i],
							 state[corner]);
		if (ret)
			return ret;
	}

	pd->cur_corner = corner;

	return 0;
}

/* call from prepare */
int genpd_clk_prepare_vote_rate(struct clk_regmap *rclk,
				unsigned long rate)
{
	int corner;

	if (!rclk->genpd)
		return 0;

	corner = find_rate_to_corner(rclk, rate);
	if (corner < 0)
		return corner;

	mutex_lock(&rclk->genpd->lock);

	rclk->genpd->corner_votes[corner]++;

	/* update the corner to genpd */
	if (genpd_update_corner_state(rclk->genpd) < 0)
		rclk->genpd->corner_votes[corner]--;

	pr_debug("genpd(%s) prepare corner_votes_count %d, corner %d\n",
		 rclk->genpd->pd_name, rclk->genpd->corner_votes[corner],
		 corner);

	mutex_unlock(&rclk->genpd->lock);

	return 0;
}
EXPORT_SYMBOL_GPL(genpd_clk_prepare_vote_rate);

/* call from unprepare */
void genpd_clk_unprepare_vote_rate(struct clk_regmap *rclk,
				   unsigned long rate)
{
	int corner;

	if (!rclk->genpd)
		return;

	corner = find_rate_to_corner(rclk, rate);
	if (corner < 0)
		return;

	if (WARN(!rclk->genpd->corner_votes[corner],
		 "Reference counts are incorrect for %s corner %d\n",
		 rclk->genpd->pd_name, corner))
		return;

	mutex_lock(&rclk->genpd->lock);

	rclk->genpd->corner_votes[corner]--;

	if (genpd_update_corner_state(rclk->genpd) < 0)
		rclk->genpd->corner_votes[corner]++;

	pr_debug("genpd(%s) unprepare corner_votes_count %d, corner %d\n",
		 rclk->genpd->pd_name, rclk->genpd->corner_votes[corner],
		 corner);

	mutex_unlock(&rclk->genpd->lock);
}
EXPORT_SYMBOL_GPL(genpd_clk_unprepare_vote_rate);

int genpd_class_init(struct device *dev, struct clk_genpd_class *genpd)
{
	struct clk_genpd *pd;
	int i, num_domains;

	if (!genpd) {
		pr_debug("genpd not defined\n");
		return 0;
	}

	/* Deal only with devices using multiple PM domains. */
	num_domains = of_count_phandle_with_args(dev->of_node, "power-domains",
						 "#power-domain-cells");

	list_for_each_entry(pd, &clk_genpd_list, list) {
		if (pd->genpd == genpd) {
			pr_debug("Genpd is already part of List\n");
			return 0;
		}
	}

	pr_debug("Voting for genpd_class %s\n", genpd->pd_name);

	if (num_domains == 1) {
		genpd->genpd_dev[0] = dev;
	} else {
		for (i = 0; i < genpd->num_genpd; i++) {
			int index = genpd->genpd_index[i];

			genpd->genpd_dev[i] = genpd_dev_pm_attach_by_id(dev,
									index);
		}
	}

	/*
	 * vote for max corner during boot -- ToDo, as no current way to remove
	 * the genpd max vote.
	 */
	pd = kmalloc(sizeof(*pd), GFP_KERNEL);
	if (!pd)
		return -ENOMEM;

	pd->genpd = genpd;

	list_add_tail(&pd->list, &clk_genpd_list);

	return 0;
}
EXPORT_SYMBOL_GPL(genpd_class_init);

MODULE_LICENSE("GPL v2");
