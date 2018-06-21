/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (c) 2018, The Linux Foundation. All rights reserved. */

struct clk_regmap;

/**
 * struct clk_powerdomain_class - Power Domain scaling class
 * @pd_name:		name of the power domain class
 * @powerdomain_dev:	array of power domain devices
 * @num_pd:		size of power domain devices array.
 * @pd_index:		array of power domain index which would
 *			be used to attach the power domain using
 *			genpd_dev_pm_attach_by_id(dev, index).
 * @corner:		sorted 2D array of legal corner settings,
			indexed by the corner.
 * @corner_votes:	array of votes for each corner
 * @num_corner:		specifies the size of corner_votes array
 * @cur_corner:		current set power domain corner
 * @lock:		lock to protect this struct
 */
struct clk_powerdomain_class {
	const char *pd_name;
	struct device *dev;
	struct device **powerdomain_dev;
	struct device_link **links;
	int num_pd;
	int *pd_index;
	int *corner;
	int *corner_votes;
	int num_corners;
	unsigned int cur_corner;
	/* Protect this struct */
	struct mutex lock;
};

#define CLK_POWERDOMAIN_INIT(_name, _num_corners, _num_pd, _corner)	\
	struct clk_powerdomain_class _name = {				\
		.pd_name = #_name,					\
		.powerdomain_dev = (struct device *([_num_pd])) {},	\
		.links = (struct device_link *([_num_pd])) {},		\
		.num_pd = _num_pd,					\
		.pd_index = (int [_num_pd]) {},				\
		.corner_votes = (int [_num_corners]) {},		\
		.num_corners = _num_corners,				\
		.corner = _corner,					\
		.cur_corner = _num_corners,				\
		.lock = __MUTEX_INITIALIZER(_name.lock)			\
	}

int clk_pd_class_init(struct device *dev, struct clk_powerdomain_class *pd);
int clk_power_domain_vote_rate(struct clk_regmap *rclk, unsigned long rate);
void clk_power_domain_unvote_rate(struct clk_regmap *rclk, unsigned long rate);
