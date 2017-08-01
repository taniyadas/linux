#include <linux/boot_constraint.h>
#include <linux/init.h>
#include <linux/kernel.h>

struct dev_boot_constraint_clk_info iface_clk_info = {
	.name = "iface_clk",
};

struct dev_boot_constraint_clk_info bus_clk_info = {
	.name = "bus_clk",
};

struct dev_boot_constraint_clk_info core_clk_info = {
	.name = "core_clk",
};

struct dev_boot_constraint_clk_info vsync_clk_info = {
	.name = "vsync_clk",
};

struct dev_boot_constraint_clk_info esc0_clk_info = {
	.name = "core_clk",
};

struct dev_boot_constraint_clk_info byte_clk_info = {
	.name = "byte_clk",
};

struct dev_boot_constraint_clk_info pixel_clk_info = {
	.name = "pixel_clk",
};

struct dev_boot_constraint_supply_info vdda_info = {
	.name = "vdda"
};

struct dev_boot_constraint_supply_info vddio_info = {
	.name = "vddio"
};

struct dev_boot_constraint constraints_mdss[] = {
	{
		.type = DEV_BOOT_CONSTRAINT_PM,
		.data = NULL,
	},
};

struct dev_boot_constraint constraints_mdp[] = {
	{
		.type = DEV_BOOT_CONSTRAINT_CLK,
		.data = &iface_clk_info,
	}, {
		.type = DEV_BOOT_CONSTRAINT_CLK,
		.data = &bus_clk_info,
	}, {
		.type = DEV_BOOT_CONSTRAINT_CLK,
		.data = &core_clk_info,
	}, {
		.type = DEV_BOOT_CONSTRAINT_CLK,
		.data = &vsync_clk_info,
	},
};

struct dev_boot_constraint constraints_dsi[] = {
	{
		.type = DEV_BOOT_CONSTRAINT_CLK,
		.data = &esc0_clk_info,
	}, {
		.type = DEV_BOOT_CONSTRAINT_CLK,
		.data = &byte_clk_info,
	}, {
		.type = DEV_BOOT_CONSTRAINT_CLK,
		.data = &pixel_clk_info,
	}, {
		.type = DEV_BOOT_CONSTRAINT_SUPPLY,
		.data = &vdda_info,

	}, {
		.type = DEV_BOOT_CONSTRAINT_SUPPLY,
		.data = &vddio_info,
	},
};

static int __init qcom_constraints_init(void)
{
	int ret;

	ret = dev_boot_constraint_add_of_deferrable("qcom,mdss",
					 constraints_mdss,
					 ARRAY_SIZE(constraints_mdss));
	if (ret)
		return ret;

	ret = dev_boot_constraint_add_of_deferrable("qcom,mdp5",
					 constraints_mdp,
					 ARRAY_SIZE(constraints_mdp));
	if (ret)
		return ret;

	ret = dev_boot_constraint_add_of_deferrable("qcom,mdss-dsi-ctrl",
					 constraints_dsi,
					 ARRAY_SIZE(constraints_dsi));
	return ret;
}
subsys_initcall(qcom_constraints_init);
