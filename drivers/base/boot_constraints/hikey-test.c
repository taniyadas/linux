#include <linux/boot_constraint.h>
#include <linux/init.h>
#include <linux/kernel.h>

struct dev_boot_constraint_clk_info clk_info = {
	.name = "ciu",
};

struct dev_boot_constraint_supply_info supply_info[] = {
	{
		.name = "vmmc",
		.u_volt_min = 1800000,
		.u_volt_max = 3000000,
	}, {
		.name = "vmmcaux",
		.u_volt_min = 1800000,
		.u_volt_max = 3000000,
	}
};

struct dev_boot_constraint constraints[] = {
	{
		.type = DEV_BOOT_CONSTRAINT_SUPPLY,
		.data = &supply_info[0],
	}, {
		.type = DEV_BOOT_CONSTRAINT_SUPPLY,
		.data = &supply_info[1],
	}, {
		.type = DEV_BOOT_CONSTRAINT_CLK,
		.data = &clk_info,
	}, {
		.type = DEV_BOOT_CONSTRAINT_PM,
		.data = NULL,
	},
};

static int __init test_constraints_init(void)
{
	int ret;

	ret = dev_boot_constraint_add_of_deferrable("hisilicon,hi6220-dw-mshc",
					 constraints, ARRAY_SIZE(constraints));

	pr_info("%s: %d: %d\n", __func__, __LINE__, ret);
	return ret;
}
subsys_initcall(test_constraints_init);
