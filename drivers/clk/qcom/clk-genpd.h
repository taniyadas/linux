/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (c) 2013, 2018, The Linux Foundation. All rights reserved. */

struct clk_regmap;

/**
 * struct clk_genpd_class - Power Domain scaling class
 * @pd_name:		name of the power domain class
 * @genpd_dev:		array of power domain devices
 * @num_genpd:		size of power domain devices array.
 * @genpd_index:	array of power domain index which would
 *			be used to attach the power domain using
 *			genpd_dev_pm_attach_by_id(dev, index).
 * @corner:		sorted 2D array of legal corner settings,
			indexed by the corner.
 * @corner_votes:	array of votes for each corner
 * @num_corner:		specifies the size of corner_votes array
 * @cur_corner:		current set power domain corner
 * @lock:		lock to protect this struct
 */
struct clk_genpd_class {
	const char *pd_name;
	struct device **genpd_dev;
	int num_genpd;
	int *genpd_index;
	int *corner;
	int *corner_votes;
	int num_corners;
	unsigned int cur_corner;
	/* Protect this struct */
	struct mutex lock;
};

#define CLK_GENPD_INIT(_name, _num_corners, _num_genpd, _corner)	\
	struct clk_genpd_class _name = {				\
		.pd_name = #_name,					\
		.genpd_dev = (struct device *([_num_genpd])) {},	\
		.num_genpd = _num_genpd,				\
		.genpd_index = (int [_num_genpd]) {},			\
		.corner_votes = (int [_num_corners]) {},		\
		.num_corners = _num_corners,				\
		.corner = _corner,					\
		.cur_corner = _num_corners,				\
		.lock = __MUTEX_INITIALIZER(_name.lock)			\
	}

int genpd_class_init(struct device *dev, struct clk_genpd_class *genpd);
int genpd_clk_prepare_vote_rate(struct clk_regmap *rclk, unsigned long rate);
void genpd_clk_unprepare_vote_rate(struct clk_regmap *rclk,
				   unsigned long rate);
