DEFINE_CLK_FIXED_RATE(dummy_apb_pclk, CLK_IS_ROOT, 0x0, 0x0);

DEFINE_CLK_FIXED_RATE(mcbsp_clks, CLK_IS_ROOT, 0x0, 0x0);

DEFINE_CLK_FIXED_RATE(omap_32k_fck, CLK_IS_ROOT, 32768, 0x0);

DEFINE_CLK_FIXED_RATE(pclk_ck, CLK_IS_ROOT, 27000000, 0x0);

DEFINE_CLK_FIXED_RATE(rmii_ck, CLK_IS_ROOT, 50000000, 0x0);

DEFINE_CLK_FIXED_RATE(secure_32k_fck, CLK_IS_ROOT, 32768, 0x0);

DEFINE_CLK_FIXED_RATE(sys_altclk, CLK_IS_ROOT, 0x0, 0x0);

DEFINE_CLK_FIXED_RATE(virt_12m_ck, CLK_IS_ROOT, 12000000, 0x0);

DEFINE_CLK_FIXED_RATE(virt_13m_ck, CLK_IS_ROOT, 13000000, 0x0);

DEFINE_CLK_FIXED_RATE(virt_16_8m_ck, CLK_IS_ROOT, 16800000, 0x0);

DEFINE_CLK_FIXED_RATE(virt_19_2m_ck, CLK_IS_ROOT, 19200000, 0x0);

DEFINE_CLK_FIXED_RATE(virt_26m_ck, CLK_IS_ROOT, 26000000, 0x0);

DEFINE_CLK_FIXED_RATE(virt_38_4m_ck, CLK_IS_ROOT, 38400000, 0x0);

static const struct clksel_rate osc_sys_12m_rates[] = {
	{ .div = 1, .val = 0, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel_rate osc_sys_13m_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel_rate osc_sys_16_8m_rates[] = {
	{ .div = 1, .val = 5, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 0 }
};

static const struct clksel_rate osc_sys_19_2m_rates[] = {
	{ .div = 1, .val = 2, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel_rate osc_sys_26m_rates[] = {
	{ .div = 1, .val = 3, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel_rate osc_sys_38_4m_rates[] = {
	{ .div = 1, .val = 4, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel osc_sys_clksel[] = {
	{ .parent = &virt_12m_ck, .rates = osc_sys_12m_rates },
	{ .parent = &virt_13m_ck, .rates = osc_sys_13m_rates },
	{ .parent = &virt_16_8m_ck, .rates = osc_sys_16_8m_rates },
	{ .parent = &virt_19_2m_ck, .rates = osc_sys_19_2m_rates },
	{ .parent = &virt_26m_ck, .rates = osc_sys_26m_rates },
	{ .parent = &virt_38_4m_ck, .rates = osc_sys_38_4m_rates },
	{ .parent = NULL },
};

static const char *osc_sys_ck_parent_names[] = {
	"virt_12m_ck",
	"virt_13m_ck",
	"virt_16_8m_ck",
	"virt_19_2m_ck",
	"virt_26m_ck",
	"virt_38_4m_ck",
};

static struct clk osc_sys_ck;

static const struct clk_ops osc_sys_ck_ops = {
	.recalc_rate	= &omap2_clksel_recalc,
	.get_parent	= &omap2_init_clksel_parent,
	.set_parent	= &omap2_clksel_set_parent,
};

static struct clk_hw_omap osc_sys_ck_hw = {
	.hw = {
		.clk = &osc_sys_ck,
	},
	.clksel		= osc_sys_clksel,
	.clksel_reg	= OMAP3430_PRM_CLKSEL,
	.clksel_mask	= OMAP3430_SYS_CLKIN_SEL_MASK,
};

DEFINE_STRUCT_CLK(osc_sys_ck, osc_sys_ck_parent_names, osc_sys_ck_ops);

static const struct clksel_rate div2_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_3XXX },
	{ .div = 2, .val = 2, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel sys_clksel[] = {
	{ .parent = &osc_sys_ck, .rates = div2_rates },
	{ .parent = NULL },
};

static const char *sys_ck_parent_names[] = {
	"osc_sys_ck",
};

static struct clk sys_ck;

static const struct clk_ops sys_ck_ops = {
	.recalc_rate	= &omap2_clksel_recalc,
	.set_rate	= &omap2_clksel_set_rate,
	.round_rate	= &omap2_clksel_round_rate,
};

static struct clk_hw_omap sys_ck_hw = {
	.hw = {
		.clk = &sys_ck,
	},
	.clksel		= sys_clksel,
	.clksel_reg	= OMAP3430_PRM_CLKSRC_CTRL,
	.clksel_mask	= OMAP_SYSCLKDIV_MASK,
};

DEFINE_STRUCT_CLK(sys_ck, sys_ck_parent_names, sys_ck_ops);

static struct dpll_data dpll3_dd = {
	.mult_div1_reg	= OMAP_CM_REGADDR(PLL_MOD, CM_CLKSEL1),
	.mult_mask	= OMAP3430_CORE_DPLL_MULT_MASK,
	.div1_mask	= OMAP3430_CORE_DPLL_DIV_MASK,
	.clk_bypass	= &sys_ck,
	.clk_ref	= &sys_ck,
	.freqsel_mask	= OMAP3430_CORE_DPLL_FREQSEL_MASK,
	.control_reg	= OMAP_CM_REGADDR(PLL_MOD, CM_CLKEN),
	.enable_mask	= OMAP3430_EN_CORE_DPLL_MASK,
	.auto_recal_bit	= OMAP3430_EN_CORE_DPLL_DRIFTGUARD_SHIFT,
	.recal_en_bit	= OMAP3430_CORE_DPLL_RECAL_EN_SHIFT,
	.recal_st_bit	= OMAP3430_CORE_DPLL_ST_SHIFT,
	.autoidle_reg	= OMAP_CM_REGADDR(PLL_MOD, CM_AUTOIDLE),
	.autoidle_mask	= OMAP3430_AUTO_CORE_DPLL_MASK,
	.idlest_reg	= OMAP_CM_REGADDR(PLL_MOD, CM_IDLEST),
	.idlest_mask	= OMAP3430_ST_CORE_CLK_MASK,
	.max_multiplier	= OMAP3_MAX_DPLL_MULT,
	.min_divider	= 1,
	.max_divider	= OMAP3_MAX_DPLL_DIV,
};

static struct clk dpll3_ck;

static const char *dpll3_ck_parent_names[] = {
	"sys_ck",
};

static const struct clk_ops dpll3_ck_ops = {
	.init		= &omap2_init_clk_clkdm,
	.enable		= &omap3_noncore_dpll_enable,
	.disable	= &omap3_noncore_dpll_disable,
	.set_rate	= &omap3_noncore_dpll_set_rate,
	.get_parent	= &omap2_init_dpll_parent,
	.recalc_rate	= &omap3_dpll_recalc,
	.round_rate	= &omap2_dpll_round_rate,
};

static struct clk_hw_omap dpll3_ck_hw = {
	.hw = {
		.clk = &dpll3_ck,
	},
	.dpll_data	= &dpll3_dd,
	.clkdm_name	= "dpll3_clkdm",
};

DEFINE_STRUCT_CLK(dpll3_ck, dpll3_ck_parent_names, dpll3_ck_ops);

static const struct clksel_rate div31_dpll3_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_3XXX },
	{ .div = 2, .val = 2, .flags = RATE_IN_3XXX },
	{ .div = 3, .val = 3, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 4, .val = 4, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 5, .val = 5, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 6, .val = 6, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 7, .val = 7, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 8, .val = 8, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 9, .val = 9, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 10, .val = 10, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 11, .val = 11, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 12, .val = 12, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 13, .val = 13, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 14, .val = 14, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 15, .val = 15, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 16, .val = 16, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 17, .val = 17, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 18, .val = 18, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 19, .val = 19, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 20, .val = 20, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 21, .val = 21, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 22, .val = 22, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 23, .val = 23, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 24, .val = 24, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 25, .val = 25, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 26, .val = 26, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 27, .val = 27, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 28, .val = 28, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 29, .val = 29, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 30, .val = 30, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 31, .val = 31, .flags = RATE_IN_3430ES2PLUS_36XX },
	{ .div = 0 }
};

static const struct clksel div31_dpll3m2_clksel[] = {
	{ .parent = &dpll3_ck, .rates = div31_dpll3_rates },
	{ .parent = NULL },
};

static const char *dpll3_m2_ck_parent_names[] = {
	"dpll3_ck",
};

static struct clk dpll3_m2_ck;

static const struct clk_ops dpll3_m2_ck_ops = {
	.recalc_rate	= &omap2_clksel_recalc,
	.set_rate	= &omap3_core_dpll_m2_set_rate,
	.round_rate	= &omap2_clksel_round_rate,
};

static struct clk_hw_omap dpll3_m2_ck_hw = {
	.hw = {
		.clk = &dpll3_m2_ck,
	},
	.clksel		= div31_dpll3m2_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(PLL_MOD, CM_CLKSEL1),
	.clksel_mask	= OMAP3430_CORE_DPLL_CLKOUT_DIV_MASK,
};

DEFINE_STRUCT_CLK(dpll3_m2_ck, dpll3_m2_ck_parent_names, dpll3_m2_ck_ops);

static struct clk core_ck;

static const char *core_ck_parent_names[] = {
	"dpll3_m2_ck",
};

static const struct clk_ops core_ck_ops = {
};

static struct clk_hw_omap core_ck_hw = {
	.hw = {
		.clk = &core_ck,
	},
};

DEFINE_STRUCT_CLK(core_ck, core_ck_parent_names, core_ck_ops);

static const struct clksel div2_core_clksel[] = {
	{ .parent = &core_ck, .rates = div2_rates },
	{ .parent = NULL },
};

static const char *l3_ick_parent_names[] = {
	"core_ck",
};

static struct clk l3_ick;

static struct clk_hw_omap l3_ick_hw = {
	.hw = {
		.clk = &l3_ick,
	},
	.clksel		= div2_core_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP3430_CLKSEL_L3_MASK,
};

DEFINE_STRUCT_CLK(l3_ick, l3_ick_parent_names, sys_ck_ops);

static const struct clksel div2_l3_clksel[] = {
	{ .parent = &l3_ick, .rates = div2_rates },
	{ .parent = NULL },
};

static const char *l4_ick_parent_names[] = {
	"l3_ick",
};

static struct clk l4_ick;

static struct clk_hw_omap l4_ick_hw = {
	.hw = {
		.clk = &l4_ick,
	},
	.clksel		= div2_l3_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP3430_CLKSEL_L4_MASK,
};

DEFINE_STRUCT_CLK(l4_ick, l4_ick_parent_names, sys_ck_ops);

static struct clk security_l4_ick2;

static const char *security_l4_ick2_parent_names[] = {
	"l4_ick",
};

static struct clk_hw_omap security_l4_ick2_hw = {
	.hw = {
		.clk = &security_l4_ick2,
	},
};

DEFINE_STRUCT_CLK(security_l4_ick2, security_l4_ick2_parent_names, core_ck_ops);

static struct clk aes1_ick;

static const char *aes1_ick_parent_names[] = {
	"security_l4_ick2",
};

static const struct clk_ops aes1_ick_ops = {
	.enable		= &omap2_dflt_clk_enable,
	.disable	= &omap2_dflt_clk_disable,
};

static struct clk_hw_omap aes1_ick_hw = {
	.hw = {
		.clk = &aes1_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN2),
	.enable_bit	= OMAP3430_EN_AES1_SHIFT,
};

DEFINE_STRUCT_CLK(aes1_ick, aes1_ick_parent_names, aes1_ick_ops);

static struct clk core_l4_ick;

static const char *core_l4_ick_parent_names[] = {
	"l4_ick",
};

static struct clk_hw_omap core_l4_ick_hw = {
	.hw = {
		.clk = &core_l4_ick,
	},
};

DEFINE_STRUCT_CLK(core_l4_ick, core_l4_ick_parent_names, core_ck_ops);

static struct clk aes2_ick;

static const char *aes2_ick_parent_names[] = {
	"core_l4_ick",
};

static const struct clk_ops aes2_ick_ops = {
	.init		= &omap2_init_clk_clkdm,
	.enable		= &omap2_dflt_clk_enable,
	.disable	= &omap2_dflt_clk_disable,
};

static struct clk_hw_omap aes2_ick_hw = {
	.hw = {
		.clk = &aes2_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_AES2_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(aes2_ick, aes2_ick_parent_names, aes2_ick_ops);

static struct clk dpll1_fck;

static struct dpll_data dpll1_dd = {
	.mult_div1_reg	= OMAP_CM_REGADDR(MPU_MOD, OMAP3430_CM_CLKSEL1_PLL),
	.mult_mask	= OMAP3430_MPU_DPLL_MULT_MASK,
	.div1_mask	= OMAP3430_MPU_DPLL_DIV_MASK,
	.clk_bypass	= &dpll1_fck,
	.clk_ref	= &sys_ck,
	.freqsel_mask	= OMAP3430_MPU_DPLL_FREQSEL_MASK,
	.control_reg	= OMAP_CM_REGADDR(MPU_MOD, OMAP3430_CM_CLKEN_PLL),
	.enable_mask	= OMAP3430_EN_MPU_DPLL_MASK,
	.modes		= (1 << DPLL_LOW_POWER_BYPASS) | (1 << DPLL_LOCKED),
	.auto_recal_bit	= OMAP3430_EN_MPU_DPLL_DRIFTGUARD_SHIFT,
	.recal_en_bit	= OMAP3430_MPU_DPLL_RECAL_EN_SHIFT,
	.recal_st_bit	= OMAP3430_MPU_DPLL_ST_SHIFT,
	.autoidle_reg	= OMAP_CM_REGADDR(MPU_MOD, OMAP3430_CM_AUTOIDLE_PLL),
	.autoidle_mask	= OMAP3430_AUTO_MPU_DPLL_MASK,
	.idlest_reg	= OMAP_CM_REGADDR(MPU_MOD, OMAP3430_CM_IDLEST_PLL),
	.idlest_mask	= OMAP3430_ST_MPU_CLK_MASK,
	.max_multiplier	= OMAP3_MAX_DPLL_MULT,
	.min_divider	= 1,
	.max_divider	= OMAP3_MAX_DPLL_DIV,
};

static struct clk dpll1_ck;

static const char *dpll1_ck_parent_names[] = {
	"sys_ck",
};

static const struct clk_ops dpll1_ck_ops = {
	.init		= &omap2_init_clk_clkdm,
	.enable		= &omap3_noncore_dpll_enable,
	.disable	= &omap3_noncore_dpll_disable,
	.set_rate	= &omap3_noncore_dpll_set_rate,
	.get_parent	= &omap2_init_dpll_parent,
	.recalc_rate	= &omap3_dpll_recalc,
	.set_rate	= &omap3_noncore_dpll_set_rate,
	.round_rate	= &omap2_dpll_round_rate,
};

static struct clk_hw_omap dpll1_ck_hw = {
	.hw = {
		.clk = &dpll1_ck,
	},
	.dpll_data	= &dpll1_dd,
	.clkdm_name	= "dpll1_clkdm",
};

DEFINE_STRUCT_CLK(dpll1_ck, dpll1_ck_parent_names, dpll1_ck_ops);

static struct clk dpll1_x2_ck;

static const char *dpll1_x2_ck_parent_names[] = {
	"dpll1_ck",
};

static const struct clk_ops dpll1_x2_ck_ops = {
	.recalc_rate	= &omap3_clkoutx2_recalc,
};

static struct clk_hw_omap dpll1_x2_ck_hw = {
	.hw = {
		.clk = &dpll1_x2_ck,
	},
};

DEFINE_STRUCT_CLK(dpll1_x2_ck, dpll1_x2_ck_parent_names, dpll1_x2_ck_ops);

static const struct clksel_rate div16_dpll_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_3XXX },
	{ .div = 2, .val = 2, .flags = RATE_IN_3XXX },
	{ .div = 3, .val = 3, .flags = RATE_IN_3XXX },
	{ .div = 4, .val = 4, .flags = RATE_IN_3XXX },
	{ .div = 5, .val = 5, .flags = RATE_IN_3XXX },
	{ .div = 6, .val = 6, .flags = RATE_IN_3XXX },
	{ .div = 7, .val = 7, .flags = RATE_IN_3XXX },
	{ .div = 8, .val = 8, .flags = RATE_IN_3XXX },
	{ .div = 9, .val = 9, .flags = RATE_IN_3XXX },
	{ .div = 10, .val = 10, .flags = RATE_IN_3XXX },
	{ .div = 11, .val = 11, .flags = RATE_IN_3XXX },
	{ .div = 12, .val = 12, .flags = RATE_IN_3XXX },
	{ .div = 13, .val = 13, .flags = RATE_IN_3XXX },
	{ .div = 14, .val = 14, .flags = RATE_IN_3XXX },
	{ .div = 15, .val = 15, .flags = RATE_IN_3XXX },
	{ .div = 16, .val = 16, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel div16_dpll1_x2m2_clksel[] = {
	{ .parent = &dpll1_x2_ck, .rates = div16_dpll_rates },
	{ .parent = NULL },
};

static const char *dpll1_x2m2_ck_parent_names[] = {
	"dpll1_x2_ck",
};

static struct clk dpll1_x2m2_ck;

static struct clk_hw_omap dpll1_x2m2_ck_hw = {
	.hw = {
		.clk = &dpll1_x2m2_ck,
	},
	.clksel		= div16_dpll1_x2m2_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(MPU_MOD, OMAP3430_CM_CLKSEL2_PLL),
	.clksel_mask	= OMAP3430_MPU_DPLL_CLKOUT_DIV_MASK,
};

DEFINE_STRUCT_CLK(dpll1_x2m2_ck, dpll1_x2m2_ck_parent_names, sys_ck_ops);

static struct clk mpu_ck;

static const char *mpu_ck_parent_names[] = {
	"dpll1_x2m2_ck",
};

static struct clk_hw_omap mpu_ck_hw = {
	.hw = {
		.clk = &mpu_ck,
	},
};

DEFINE_STRUCT_CLK(mpu_ck, mpu_ck_parent_names, core_ck_ops);

static const struct clksel_rate arm_fck_rates[] = {
	{ .div = 1, .val = 0, .flags = RATE_IN_3XXX },
	{ .div = 2, .val = 1, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel arm_fck_clksel[] = {
	{ .parent = &mpu_ck, .rates = arm_fck_rates },
	{ .parent = NULL },
};

static const char *arm_fck_parent_names[] = {
	"mpu_ck",
};

static struct clk arm_fck;

static struct clk_hw_omap arm_fck_hw = {
	.hw = {
		.clk = &arm_fck,
	},
	.clksel		= arm_fck_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(MPU_MOD, OMAP3430_CM_IDLEST_PLL),
	.clksel_mask	= OMAP3430_ST_MPU_CLK_MASK,
};

DEFINE_STRUCT_CLK(arm_fck, arm_fck_parent_names, sys_ck_ops);

static const struct clksel_rate emu_src_sys_rates[] = {
	{ .div = 1, .val = 0, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel_rate emu_src_core_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel div16_dpll3_clksel[] = {
	{ .parent = &dpll3_ck, .rates = div16_dpll_rates },
	{ .parent = NULL },
};

static const char *dpll3_m3_ck_parent_names[] = {
	"dpll3_ck",
};

static struct clk dpll3_m3_ck;

static struct clk_hw_omap dpll3_m3_ck_hw = {
	.hw = {
		.clk = &dpll3_m3_ck,
	},
	.clksel		= div16_dpll3_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(OMAP3430_EMU_MOD, CM_CLKSEL1),
	.clksel_mask	= OMAP3430_DIV_DPLL3_MASK,
};

DEFINE_STRUCT_CLK(dpll3_m3_ck, dpll3_m3_ck_parent_names, sys_ck_ops);

static struct clk dpll3_m3x2_ck;

static const char *dpll3_m3x2_ck_parent_names[] = {
	"dpll3_m3_ck",
};

static const struct clk_ops dpll3_m3x2_ck_ops = {
	.init		= &omap2_init_clk_clkdm,
	.enable		= &omap2_dflt_clk_enable,
	.disable	= &omap2_dflt_clk_disable,
	.recalc_rate	= &omap3_clkoutx2_recalc,
};

static struct clk_hw_omap dpll3_m3x2_ck_hw = {
	.hw = {
		.clk = &dpll3_m3x2_ck,
	},
	.enable_reg	= OMAP_CM_REGADDR(PLL_MOD, CM_CLKEN),
	.enable_bit	= OMAP3430_PWRDN_EMU_CORE_SHIFT,
	.flags		= INVERT_ENABLE,
	.clkdm_name	= "dpll3_clkdm",
};

DEFINE_STRUCT_CLK(dpll3_m3x2_ck, dpll3_m3x2_ck_parent_names, dpll3_m3x2_ck_ops);

static struct clk emu_core_alwon_ck;

static const char *emu_core_alwon_ck_parent_names[] = {
	"dpll3_m3x2_ck",
};

static struct clk_hw_omap emu_core_alwon_ck_hw = {
	.hw = {
		.clk = &emu_core_alwon_ck,
	},
};

DEFINE_STRUCT_CLK(emu_core_alwon_ck, emu_core_alwon_ck_parent_names, core_ck_ops);

static const struct clksel_rate emu_src_per_rates[] = {
	{ .div = 1, .val = 2, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static struct clk dpll4_ck;

static const char *dpll4_ck_parent_names[] = {
	"sys_ck",
};

static const struct clk_ops dpll4_ck_ops = {
	.init		= &omap2_init_clk_clkdm,
	.recalc_rate	= &omap3_dpll_recalc,
	.set_rate	= &omap3_dpll4_set_rate,
	.round_rate	= &omap2_dpll_round_rate,
};

static struct clk_hw_omap dpll4_ck_hw = {
	.hw = {
		.clk = &dpll4_ck,
	},
	.clkdm_name	= "dpll4_clkdm",
};

DEFINE_STRUCT_CLK(dpll4_ck, dpll4_ck_parent_names, dpll4_ck_ops);

static const struct clksel_rate dpll4_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_3XXX },
	{ .div = 2, .val = 2, .flags = RATE_IN_3XXX },
	{ .div = 3, .val = 3, .flags = RATE_IN_3XXX },
	{ .div = 4, .val = 4, .flags = RATE_IN_3XXX },
	{ .div = 5, .val = 5, .flags = RATE_IN_3XXX },
	{ .div = 6, .val = 6, .flags = RATE_IN_3XXX },
	{ .div = 7, .val = 7, .flags = RATE_IN_3XXX },
	{ .div = 8, .val = 8, .flags = RATE_IN_3XXX },
	{ .div = 9, .val = 9, .flags = RATE_IN_3XXX },
	{ .div = 10, .val = 10, .flags = RATE_IN_3XXX },
	{ .div = 11, .val = 11, .flags = RATE_IN_3XXX },
	{ .div = 12, .val = 12, .flags = RATE_IN_3XXX },
	{ .div = 13, .val = 13, .flags = RATE_IN_3XXX },
	{ .div = 14, .val = 14, .flags = RATE_IN_3XXX },
	{ .div = 15, .val = 15, .flags = RATE_IN_3XXX },
	{ .div = 16, .val = 16, .flags = RATE_IN_3XXX },
	{ .div = 17, .val = 17, .flags = RATE_IN_36XX },
	{ .div = 18, .val = 18, .flags = RATE_IN_36XX },
	{ .div = 19, .val = 19, .flags = RATE_IN_36XX },
	{ .div = 20, .val = 20, .flags = RATE_IN_36XX },
	{ .div = 21, .val = 21, .flags = RATE_IN_36XX },
	{ .div = 22, .val = 22, .flags = RATE_IN_36XX },
	{ .div = 23, .val = 23, .flags = RATE_IN_36XX },
	{ .div = 24, .val = 24, .flags = RATE_IN_36XX },
	{ .div = 25, .val = 25, .flags = RATE_IN_36XX },
	{ .div = 26, .val = 26, .flags = RATE_IN_36XX },
	{ .div = 27, .val = 27, .flags = RATE_IN_36XX },
	{ .div = 28, .val = 28, .flags = RATE_IN_36XX },
	{ .div = 29, .val = 29, .flags = RATE_IN_36XX },
	{ .div = 30, .val = 30, .flags = RATE_IN_36XX },
	{ .div = 31, .val = 31, .flags = RATE_IN_36XX },
	{ .div = 32, .val = 32, .flags = RATE_IN_36XX },
	{ .div = 0 }
};

static const struct clksel dpll4_clksel[] = {
	{ .parent = &dpll4_ck, .rates = dpll4_rates },
	{ .parent = NULL },
};

static const char *dpll4_m6_ck_parent_names[] = {
	"dpll4_ck",
};

static struct clk dpll4_m6_ck;

static struct clk_hw_omap dpll4_m6_ck_hw = {
	.hw = {
		.clk = &dpll4_m6_ck,
	},
	.clksel		= dpll4_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(OMAP3430_EMU_MOD, CM_CLKSEL1),
	.clksel_mask	= OMAP3630_DIV_DPLL4_MASK,
};

DEFINE_STRUCT_CLK(dpll4_m6_ck, dpll4_m6_ck_parent_names, sys_ck_ops);

static struct clk dpll4_m6x2_ck;

static const char *dpll4_m6x2_ck_parent_names[] = {
	"dpll4_m6_ck",
};

static struct clk_hw_omap dpll4_m6x2_ck_hw = {
	.hw = {
		.clk = &dpll4_m6x2_ck,
	},
	.enable_reg	= OMAP_CM_REGADDR(PLL_MOD, CM_CLKEN),
	.enable_bit	= OMAP3430_PWRDN_EMU_PERIPH_SHIFT,
	.flags		= INVERT_ENABLE,
	.clkdm_name	= "dpll4_clkdm",
};

DEFINE_STRUCT_CLK(dpll4_m6x2_ck, dpll4_m6x2_ck_parent_names, dpll3_m3x2_ck_ops);

static struct clk emu_per_alwon_ck;

static const char *emu_per_alwon_ck_parent_names[] = {
	"dpll4_m6x2_ck",
};

static struct clk_hw_omap emu_per_alwon_ck_hw = {
	.hw = {
		.clk = &emu_per_alwon_ck,
	},
};

DEFINE_STRUCT_CLK(emu_per_alwon_ck, emu_per_alwon_ck_parent_names, core_ck_ops);

static const struct clksel_rate emu_src_mpu_rates[] = {
	{ .div = 1, .val = 3, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static struct clk emu_mpu_alwon_ck;

static const char *emu_mpu_alwon_ck_parent_names[] = {
	"mpu_ck",
};

static struct clk_hw_omap emu_mpu_alwon_ck_hw = {
	.hw = {
		.clk = &emu_mpu_alwon_ck,
	},
};

DEFINE_STRUCT_CLK(emu_mpu_alwon_ck, emu_mpu_alwon_ck_parent_names, core_ck_ops);

static const struct clksel emu_src_clksel[] = {
	{ .parent = &sys_ck, .rates = emu_src_sys_rates },
	{ .parent = &emu_core_alwon_ck, .rates = emu_src_core_rates },
	{ .parent = &emu_per_alwon_ck, .rates = emu_src_per_rates },
	{ .parent = &emu_mpu_alwon_ck, .rates = emu_src_mpu_rates },
	{ .parent = NULL },
};

static const char *emu_src_ck_parent_names[] = {
	"sys_ck",
	"emu_core_alwon_ck",
	"emu_per_alwon_ck",
	"emu_mpu_alwon_ck",
};

static struct clk emu_src_ck;

static struct clk_hw_omap emu_src_ck_hw = {
	.hw = {
		.clk = &emu_src_ck,
	},
	.clksel		= emu_src_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(OMAP3430_EMU_MOD, CM_CLKSEL1),
	.clksel_mask	= OMAP3430_MUX_CTRL_MASK,
};

DEFINE_STRUCT_CLK(emu_src_ck, emu_src_ck_parent_names, osc_sys_ck_ops);

static const struct clksel atclk_emu_clksel[] = {
	{ .parent = &emu_src_ck, .rates = div2_rates },
	{ .parent = NULL },
};

static const char *atclk_fck_parent_names[] = {
	"emu_src_ck",
};

static struct clk atclk_fck;

static struct clk_hw_omap atclk_fck_hw = {
	.hw = {
		.clk = &atclk_fck,
	},
	.clksel		= atclk_emu_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(OMAP3430_EMU_MOD, CM_CLKSEL1),
	.clksel_mask	= OMAP3430_CLKSEL_ATCLK_MASK,
};

DEFINE_STRUCT_CLK(atclk_fck, atclk_fck_parent_names, sys_ck_ops);

static struct clk cam_ick;

static const char *cam_ick_parent_names[] = {
	"l4_ick",
};

static struct clk_hw_omap cam_ick_hw = {
	.hw = {
		.clk = &cam_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_CAM_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430_EN_CAM_SHIFT,
	.clkdm_name	= "cam_clkdm",
};

DEFINE_STRUCT_CLK(cam_ick, cam_ick_parent_names, aes2_ick_ops);

static struct clk dpll4_m5_ck;

static struct clk_hw_omap dpll4_m5_ck_hw = {
	.hw = {
		.clk = &dpll4_m5_ck,
	},
	.clksel		= dpll4_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(OMAP3430_CAM_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP3630_CLKSEL_CAM_MASK,
};

DEFINE_STRUCT_CLK(dpll4_m5_ck, dpll4_m6_ck_parent_names, sys_ck_ops);

static struct clk dpll4_m5x2_ck;

static const char *dpll4_m5x2_ck_parent_names[] = {
	"dpll4_m5_ck",
};

static struct clk_hw_omap dpll4_m5x2_ck_hw = {
	.hw = {
		.clk = &dpll4_m5x2_ck,
	},
	.enable_reg	= OMAP_CM_REGADDR(PLL_MOD, CM_CLKEN),
	.enable_bit	= OMAP3430_PWRDN_CAM_SHIFT,
	.flags		= INVERT_ENABLE,
	.clkdm_name	= "dpll4_clkdm",
};

DEFINE_STRUCT_CLK(dpll4_m5x2_ck, dpll4_m5x2_ck_parent_names, dpll3_m3x2_ck_ops);

static struct clk cam_mclk;

static const char *cam_mclk_parent_names[] = {
	"dpll4_m5x2_ck",
};

static struct clk_hw_omap cam_mclk_hw = {
	.hw = {
		.clk = &cam_mclk,
	},
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_CAM_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_CAM_SHIFT,
	.clkdm_name	= "cam_clkdm",
};

DEFINE_STRUCT_CLK(cam_mclk, cam_mclk_parent_names, aes2_ick_ops);

static const struct clksel_rate clkout2_src_core_rates[] = {
	{ .div = 1, .val = 0, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel_rate clkout2_src_sys_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel_rate clkout2_src_96m_rates[] = {
	{ .div = 1, .val = 2, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static struct clk dpll4_m2_ck;

static struct clk_hw_omap dpll4_m2_ck_hw = {
	.hw = {
		.clk = &dpll4_m2_ck,
	},
	.clksel		= dpll4_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(PLL_MOD, OMAP3430_CM_CLKSEL3),
	.clksel_mask	= OMAP3630_DIV_96M_MASK,
};

DEFINE_STRUCT_CLK(dpll4_m2_ck, dpll4_m6_ck_parent_names, sys_ck_ops);

static struct clk dpll4_m2x2_ck;

static const char *dpll4_m2x2_ck_parent_names[] = {
	"dpll4_m2_ck",
};

static struct clk_hw_omap dpll4_m2x2_ck_hw = {
	.hw = {
		.clk = &dpll4_m2x2_ck,
	},
	.enable_reg	= OMAP_CM_REGADDR(PLL_MOD, CM_CLKEN),
	.enable_bit	= OMAP3430_PWRDN_96M_SHIFT,
	.flags		= INVERT_ENABLE,
	.clkdm_name	= "dpll4_clkdm",
};

DEFINE_STRUCT_CLK(dpll4_m2x2_ck, dpll4_m2x2_ck_parent_names, dpll3_m3x2_ck_ops);

static struct clk omap_96m_alwon_fck;

static const char *omap_96m_alwon_fck_parent_names[] = {
	"dpll4_m2x2_ck",
};

static struct clk_hw_omap omap_96m_alwon_fck_hw = {
	.hw = {
		.clk = &omap_96m_alwon_fck,
	},
};

DEFINE_STRUCT_CLK(omap_96m_alwon_fck, omap_96m_alwon_fck_parent_names, core_ck_ops);

static struct clk cm_96m_fck;

static const char *cm_96m_fck_parent_names[] = {
	"omap_96m_alwon_fck",
};

static struct clk_hw_omap cm_96m_fck_hw = {
	.hw = {
		.clk = &cm_96m_fck,
	},
};

DEFINE_STRUCT_CLK(cm_96m_fck, cm_96m_fck_parent_names, core_ck_ops);

static const struct clksel_rate clkout2_src_54m_rates[] = {
	{ .div = 1, .val = 3, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel_rate omap_54m_d4m3x2_rates[] = {
	{ .div = 1, .val = 0, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static struct clk dpll4_m3_ck;

static struct clk_hw_omap dpll4_m3_ck_hw = {
	.hw = {
		.clk = &dpll4_m3_ck,
	},
	.clksel		= dpll4_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(OMAP3430_DSS_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP3630_CLKSEL_TV_MASK,
};

DEFINE_STRUCT_CLK(dpll4_m3_ck, dpll4_m6_ck_parent_names, sys_ck_ops);

static struct clk dpll4_m3x2_ck;

static const char *dpll4_m3x2_ck_parent_names[] = {
	"dpll4_m3_ck",
};

static struct clk_hw_omap dpll4_m3x2_ck_hw = {
	.hw = {
		.clk = &dpll4_m3x2_ck,
	},
	.enable_reg	= OMAP_CM_REGADDR(PLL_MOD, CM_CLKEN),
	.enable_bit	= OMAP3430_PWRDN_TV_SHIFT,
	.flags		= INVERT_ENABLE,
	.clkdm_name	= "dpll4_clkdm",
};

DEFINE_STRUCT_CLK(dpll4_m3x2_ck, dpll4_m3x2_ck_parent_names, dpll3_m3x2_ck_ops);

static const struct clksel_rate omap_54m_alt_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel omap_54m_clksel[] = {
	{ .parent = &dpll4_m3x2_ck, .rates = omap_54m_d4m3x2_rates },
	{ .parent = &sys_altclk, .rates = omap_54m_alt_rates },
	{ .parent = NULL },
};

static const char *omap_54m_fck_parent_names[] = {
	"dpll4_m3x2_ck",
	"sys_altclk",
};

static struct clk omap_54m_fck;

static struct clk_hw_omap omap_54m_fck_hw = {
	.hw = {
		.clk = &omap_54m_fck,
	},
	.clksel		= omap_54m_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(PLL_MOD, CM_CLKSEL1),
	.clksel_mask	= OMAP3430_SOURCE_54M_MASK,
};

DEFINE_STRUCT_CLK(omap_54m_fck, omap_54m_fck_parent_names, osc_sys_ck_ops);

static const struct clksel clkout2_src_clksel[] = {
	{ .parent = &core_ck, .rates = clkout2_src_core_rates },
	{ .parent = &sys_ck, .rates = clkout2_src_sys_rates },
	{ .parent = &cm_96m_fck, .rates = clkout2_src_96m_rates },
	{ .parent = &omap_54m_fck, .rates = clkout2_src_54m_rates },
	{ .parent = NULL },
};

static const char *clkout2_src_ck_parent_names[] = {
	"core_ck",
	"sys_ck",
	"cm_96m_fck",
	"omap_54m_fck",
};

static struct clk clkout2_src_ck;

static const struct clk_ops clkout2_src_ck_ops = {
	.init		= &omap2_init_clk_clkdm,
	.enable		= &omap2_dflt_clk_enable,
	.disable	= &omap2_dflt_clk_disable,
	.recalc_rate	= &omap2_clksel_recalc,
	.get_parent	= &omap2_init_clksel_parent,
	.set_parent	= &omap2_clksel_set_parent,
};

static struct clk_hw_omap clkout2_src_ck_hw = {
	.hw = {
		.clk = &clkout2_src_ck,
	},
	.clksel		= clkout2_src_clksel,
	.clksel_reg	= OMAP3430_CM_CLKOUT_CTRL,
	.clksel_mask	= OMAP3430_CLKOUT2SOURCE_MASK,
	.enable_reg	= OMAP3430_CM_CLKOUT_CTRL,
	.enable_bit	= OMAP3430_CLKOUT2_EN_SHIFT,
	.clkdm_name	= "core_clkdm",
};

DEFINE_STRUCT_CLK(clkout2_src_ck, clkout2_src_ck_parent_names, clkout2_src_ck_ops);

static const struct clksel_rate omap_48m_cm96m_rates[] = {
	{ .div = 2, .val = 0, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel_rate omap_48m_alt_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel omap_48m_clksel[] = {
	{ .parent = &cm_96m_fck, .rates = omap_48m_cm96m_rates },
	{ .parent = &sys_altclk, .rates = omap_48m_alt_rates },
	{ .parent = NULL },
};

static const char *omap_48m_fck_parent_names[] = {
	"cm_96m_fck",
	"sys_altclk",
};

static struct clk omap_48m_fck;

static struct clk_hw_omap omap_48m_fck_hw = {
	.hw = {
		.clk = &omap_48m_fck,
	},
	.clksel		= omap_48m_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(PLL_MOD, CM_CLKSEL1),
	.clksel_mask	= OMAP3430_SOURCE_48M_MASK,
};

DEFINE_STRUCT_CLK(omap_48m_fck, omap_48m_fck_parent_names, osc_sys_ck_ops);

static struct clk omap_12m_fck;

static const char *omap_12m_fck_parent_names[] = {
	"omap_48m_fck",
};

static const struct clk_ops omap_12m_fck_ops = {
	.recalc_rate	= &omap_fixed_divisor_recalc,
};

static struct clk_hw_omap omap_12m_fck_hw = {
	.hw = {
		.clk = &omap_12m_fck,
	},
	.fixed_div	= 4,
};

DEFINE_STRUCT_CLK(omap_12m_fck, omap_12m_fck_parent_names, omap_12m_fck_ops);

static struct clk core_12m_fck;

static const char *core_12m_fck_parent_names[] = {
	"omap_12m_fck",
};

static struct clk_hw_omap core_12m_fck_hw = {
	.hw = {
		.clk = &core_12m_fck,
	},
};

DEFINE_STRUCT_CLK(core_12m_fck, core_12m_fck_parent_names, core_ck_ops);

static struct clk core_48m_fck;

static const char *core_48m_fck_parent_names[] = {
	"omap_48m_fck",
};

static struct clk_hw_omap core_48m_fck_hw = {
	.hw = {
		.clk = &core_48m_fck,
	},
};

DEFINE_STRUCT_CLK(core_48m_fck, core_48m_fck_parent_names, core_ck_ops);

static const struct clksel_rate omap_96m_dpll_rates[] = {
	{ .div = 1, .val = 0, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel_rate omap_96m_sys_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel omap_96m_fck_clksel[] = {
	{ .parent = &cm_96m_fck, .rates = omap_96m_dpll_rates },
	{ .parent = &sys_ck, .rates = omap_96m_sys_rates },
	{ .parent = NULL },
};

static const char *omap_96m_fck_parent_names[] = {
	"cm_96m_fck",
	"sys_ck",
};

static struct clk omap_96m_fck;

static struct clk_hw_omap omap_96m_fck_hw = {
	.hw = {
		.clk = &omap_96m_fck,
	},
	.clksel		= omap_96m_fck_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(PLL_MOD, CM_CLKSEL1),
	.clksel_mask	= OMAP3430_SOURCE_96M_MASK,
};

DEFINE_STRUCT_CLK(omap_96m_fck, omap_96m_fck_parent_names, osc_sys_ck_ops);

static struct clk core_96m_fck;

static const char *core_96m_fck_parent_names[] = {
	"omap_96m_fck",
};

static struct clk_hw_omap core_96m_fck_hw = {
	.hw = {
		.clk = &core_96m_fck,
	},
};

DEFINE_STRUCT_CLK(core_96m_fck, core_96m_fck_parent_names, core_ck_ops);

static struct clk core_l3_ick;

static const char *core_l3_ick_parent_names[] = {
	"l3_ick",
};

static struct clk_hw_omap core_l3_ick_hw = {
	.hw = {
		.clk = &core_l3_ick,
	},
};

DEFINE_STRUCT_CLK(core_l3_ick, core_l3_ick_parent_names, core_ck_ops);

static struct clk dpll3_m2x2_ck;

static const char *dpll3_m2x2_ck_parent_names[] = {
	"dpll3_m2_ck",
};

static struct clk_hw_omap dpll3_m2x2_ck_hw = {
	.hw = {
		.clk = &dpll3_m2x2_ck,
	},
};

DEFINE_STRUCT_CLK(dpll3_m2x2_ck, dpll3_m2x2_ck_parent_names, dpll1_x2_ck_ops);

static struct clk corex2_fck;

static const char *corex2_fck_parent_names[] = {
	"dpll3_m2x2_ck",
};

static struct clk_hw_omap corex2_fck_hw = {
	.hw = {
		.clk = &corex2_fck,
	},
};

DEFINE_STRUCT_CLK(corex2_fck, corex2_fck_parent_names, core_ck_ops);

static struct clk cpefuse_fck;

static const char *cpefuse_fck_parent_names[] = {
	"sys_ck",
};

static struct clk_hw_omap cpefuse_fck_hw = {
	.hw = {
		.clk = &cpefuse_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, OMAP3430ES2_CM_FCLKEN3),
	.enable_bit	= OMAP3430ES2_EN_CPEFUSE_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(cpefuse_fck, cpefuse_fck_parent_names, aes2_ick_ops);

static struct clk csi2_96m_fck;

static const char *csi2_96m_fck_parent_names[] = {
	"core_96m_fck",
};

static struct clk_hw_omap csi2_96m_fck_hw = {
	.hw = {
		.clk = &csi2_96m_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_CAM_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_CSI2_SHIFT,
	.clkdm_name	= "cam_clkdm",
};

DEFINE_STRUCT_CLK(csi2_96m_fck, csi2_96m_fck_parent_names, aes2_ick_ops);

static struct clk d2d_26m_fck;

static const char *d2d_26m_fck_parent_names[] = {
	"sys_ck",
};

static struct clk_hw_omap d2d_26m_fck_hw = {
	.hw = {
		.clk = &d2d_26m_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP3430ES1_EN_D2D_SHIFT,
	.clkdm_name	= "d2d_clkdm",
};

DEFINE_STRUCT_CLK(d2d_26m_fck, d2d_26m_fck_parent_names, aes2_ick_ops);

static struct clk des1_ick;

static const char *des1_ick_parent_names[] = {
	"security_l4_ick2",
};

static struct clk_hw_omap des1_ick_hw = {
	.hw = {
		.clk = &des1_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN2),
	.enable_bit	= OMAP3430_EN_DES1_SHIFT,
};

DEFINE_STRUCT_CLK(des1_ick, des1_ick_parent_names, aes1_ick_ops);

static struct clk des2_ick;

static const char *des2_ick_parent_names[] = {
	"core_l4_ick",
};

static struct clk_hw_omap des2_ick_hw = {
	.hw = {
		.clk = &des2_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_DES2_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(des2_ick, des2_ick_parent_names, aes2_ick_ops);

static const struct clksel_rate div4_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_3XXX },
	{ .div = 2, .val = 2, .flags = RATE_IN_3XXX },
	{ .div = 4, .val = 4, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel div4_core_clksel[] = {
	{ .parent = &core_ck, .rates = div4_rates },
	{ .parent = NULL },
};

static const char *dpll1_fck_parent_names[] = {
	"core_ck",
};

static struct clk dpll1_fck;

static struct clk_hw_omap dpll1_fck_hw = {
	.hw = {
		.clk = &dpll1_fck,
	},
	.clksel		= div4_core_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(MPU_MOD, OMAP3430_CM_CLKSEL1_PLL),
	.clksel_mask	= OMAP3430_MPU_CLK_SRC_MASK,
};

DEFINE_STRUCT_CLK(dpll1_fck, dpll1_fck_parent_names, sys_ck_ops);

static struct clk dpll2_fck;

static struct dpll_data dpll2_dd = {
	.mult_div1_reg	= OMAP_CM_REGADDR(OMAP3430_IVA2_MOD, OMAP3430_CM_CLKSEL1_PLL),
	.mult_mask	= OMAP3430_IVA2_DPLL_MULT_MASK,
	.div1_mask	= OMAP3430_IVA2_DPLL_DIV_MASK,
	.clk_bypass	= &dpll2_fck,
	.clk_ref	= &sys_ck,
	.freqsel_mask	= OMAP3430_IVA2_DPLL_FREQSEL_MASK,
	.control_reg	= OMAP_CM_REGADDR(OMAP3430_IVA2_MOD, OMAP3430_CM_CLKEN_PLL),
	.enable_mask	= OMAP3430_EN_IVA2_DPLL_MASK,
	.modes		= (1 << DPLL_LOW_POWER_STOP) | (1 << DPLL_LOCKED) | (1 << DPLL_LOW_POWER_BYPASS),
	.auto_recal_bit	= OMAP3430_EN_IVA2_DPLL_DRIFTGUARD_SHIFT,
	.recal_en_bit	= OMAP3430_PRM_IRQENABLE_MPU_IVA2_DPLL_RECAL_EN_SHIFT,
	.recal_st_bit	= OMAP3430_PRM_IRQSTATUS_MPU_IVA2_DPLL_ST_SHIFT,
	.autoidle_reg	= OMAP_CM_REGADDR(OMAP3430_IVA2_MOD, OMAP3430_CM_AUTOIDLE_PLL),
	.autoidle_mask	= OMAP3430_AUTO_IVA2_DPLL_MASK,
	.idlest_reg	= OMAP_CM_REGADDR(OMAP3430_IVA2_MOD, OMAP3430_CM_IDLEST_PLL),
	.idlest_mask	= OMAP3430_ST_IVA2_CLK_MASK,
	.max_multiplier	= OMAP3_MAX_DPLL_MULT,
	.min_divider	= 1,
	.max_divider	= OMAP3_MAX_DPLL_DIV,
};

static struct clk dpll2_ck;

static const char *dpll2_ck_parent_names[] = {
	"sys_ck",
};

static struct clk_hw_omap dpll2_ck_hw = {
	.hw = {
		.clk = &dpll2_ck,
	},
	.dpll_data	= &dpll2_dd,
	.clkdm_name	= "dpll2_clkdm",
};

DEFINE_STRUCT_CLK(dpll2_ck, dpll2_ck_parent_names, dpll1_ck_ops);

static struct clk dpll2_fck;

static struct clk_hw_omap dpll2_fck_hw = {
	.hw = {
		.clk = &dpll2_fck,
	},
	.clksel		= div4_core_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(OMAP3430_IVA2_MOD, OMAP3430_CM_CLKSEL1_PLL),
	.clksel_mask	= OMAP3430_IVA2_CLK_SRC_MASK,
};

DEFINE_STRUCT_CLK(dpll2_fck, dpll1_fck_parent_names, sys_ck_ops);

static const struct clksel div16_dpll2_m2x2_clksel[] = {
	{ .parent = &dpll2_ck, .rates = div16_dpll_rates },
	{ .parent = NULL },
};

static const char *dpll2_m2_ck_parent_names[] = {
	"dpll2_ck",
};

static struct clk dpll2_m2_ck;

static struct clk_hw_omap dpll2_m2_ck_hw = {
	.hw = {
		.clk = &dpll2_m2_ck,
	},
	.clksel		= div16_dpll2_m2x2_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(OMAP3430_IVA2_MOD, OMAP3430_CM_CLKSEL2_PLL),
	.clksel_mask	= OMAP3430_IVA2_DPLL_CLKOUT_DIV_MASK,
};

DEFINE_STRUCT_CLK(dpll2_m2_ck, dpll2_m2_ck_parent_names, sys_ck_ops);

static struct clk dpll3_x2_ck;

static const char *dpll3_x2_ck_parent_names[] = {
	"dpll3_ck",
};

static struct clk_hw_omap dpll3_x2_ck_hw = {
	.hw = {
		.clk = &dpll3_x2_ck,
	},
};

DEFINE_STRUCT_CLK(dpll3_x2_ck, dpll3_x2_ck_parent_names, dpll1_x2_ck_ops);

static struct clk dpll4_m4_ck;

static struct clk_hw_omap dpll4_m4_ck_hw = {
	.hw = {
		.clk = &dpll4_m4_ck,
	},
	.clksel		= dpll4_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(OMAP3430_DSS_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP3630_CLKSEL_DSS1_MASK,
};

DEFINE_STRUCT_CLK(dpll4_m4_ck, dpll4_m6_ck_parent_names, sys_ck_ops);

static struct clk dpll4_m4x2_ck;

static const char *dpll4_m4x2_ck_parent_names[] = {
	"dpll4_m4_ck",
};

static struct clk_hw_omap dpll4_m4x2_ck_hw = {
	.hw = {
		.clk = &dpll4_m4x2_ck,
	},
	.enable_reg	= OMAP_CM_REGADDR(PLL_MOD, CM_CLKEN),
	.enable_bit	= OMAP3430_PWRDN_DSS1_SHIFT,
	.flags		= INVERT_ENABLE,
	.clkdm_name	= "dpll4_clkdm",
};

DEFINE_STRUCT_CLK(dpll4_m4x2_ck, dpll4_m4x2_ck_parent_names, dpll3_m3x2_ck_ops);

static struct clk dpll4_x2_ck;

static const char *dpll4_x2_ck_parent_names[] = {
	"dpll4_ck",
};

static struct clk_hw_omap dpll4_x2_ck_hw = {
	.hw = {
		.clk = &dpll4_x2_ck,
	},
};

DEFINE_STRUCT_CLK(dpll4_x2_ck, dpll4_x2_ck_parent_names, dpll1_x2_ck_ops);

static struct dpll_data dpll5_dd = {
	.mult_div1_reg	= OMAP_CM_REGADDR(PLL_MOD, OMAP3430ES2_CM_CLKSEL4),
	.mult_mask	= OMAP3430ES2_PERIPH2_DPLL_MULT_MASK,
	.div1_mask	= OMAP3430ES2_PERIPH2_DPLL_DIV_MASK,
	.clk_bypass	= &sys_ck,
	.clk_ref	= &sys_ck,
	.freqsel_mask	= OMAP3430ES2_PERIPH2_DPLL_FREQSEL_MASK,
	.control_reg	= OMAP_CM_REGADDR(PLL_MOD, OMAP3430ES2_CM_CLKEN2),
	.enable_mask	= OMAP3430ES2_EN_PERIPH2_DPLL_MASK,
	.modes		= (1 << DPLL_LOW_POWER_STOP) | (1 << DPLL_LOCKED),
	.auto_recal_bit	= OMAP3430ES2_EN_PERIPH2_DPLL_DRIFTGUARD_SHIFT,
	.recal_en_bit	= OMAP3430ES2_SND_PERIPH_DPLL_RECAL_EN_SHIFT,
	.recal_st_bit	= OMAP3430ES2_SND_PERIPH_DPLL_ST_SHIFT,
	.autoidle_reg	= OMAP_CM_REGADDR(PLL_MOD, OMAP3430ES2_CM_AUTOIDLE2_PLL),
	.autoidle_mask	= OMAP3430ES2_AUTO_PERIPH2_DPLL_MASK,
	.idlest_reg	= OMAP_CM_REGADDR(PLL_MOD, CM_IDLEST2),
	.idlest_mask	= OMAP3430ES2_ST_PERIPH2_CLK_MASK,
	.max_multiplier	= OMAP3_MAX_DPLL_MULT,
	.min_divider	= 1,
	.max_divider	= OMAP3_MAX_DPLL_DIV,
};

static struct clk dpll5_ck;

static const char *dpll5_ck_parent_names[] = {
	"sys_ck",
};

static struct clk_hw_omap dpll5_ck_hw = {
	.hw = {
		.clk = &dpll5_ck,
	},
	.dpll_data	= &dpll5_dd,
	.clkdm_name	= "dpll5_clkdm",
};

DEFINE_STRUCT_CLK(dpll5_ck, dpll5_ck_parent_names, dpll1_ck_ops);

static const struct clksel div16_dpll5_clksel[] = {
	{ .parent = &dpll5_ck, .rates = div16_dpll_rates },
	{ .parent = NULL },
};

static const char *dpll5_m2_ck_parent_names[] = {
	"dpll5_ck",
};

static struct clk dpll5_m2_ck;

static struct clk_hw_omap dpll5_m2_ck_hw = {
	.hw = {
		.clk = &dpll5_m2_ck,
	},
	.clksel		= div16_dpll5_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(PLL_MOD, OMAP3430ES2_CM_CLKSEL5),
	.clksel_mask	= OMAP3430ES2_DIV_120M_MASK,
};

DEFINE_STRUCT_CLK(dpll5_m2_ck, dpll5_m2_ck_parent_names, sys_ck_ops);

static struct clk dss1_alwon_fck_3430es1;

static const char *dss1_alwon_fck_3430es1_parent_names[] = {
	"dpll4_m4x2_ck",
};

static struct clk_hw_omap dss1_alwon_fck_3430es1_hw = {
	.hw = {
		.clk = &dss1_alwon_fck_3430es1,
	},
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_DSS_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_DSS1_SHIFT,
	.clkdm_name	= "dss_clkdm",
};

DEFINE_STRUCT_CLK(dss1_alwon_fck_3430es1, dss1_alwon_fck_3430es1_parent_names, aes2_ick_ops);

static struct clk dss1_alwon_fck_3430es2;

static const char *dss1_alwon_fck_3430es2_parent_names[] = {
	"dpll4_m4x2_ck",
};

static const struct clk_ops dss1_alwon_fck_3430es2_ops = {
	.init		= &omap2_init_clk_clkdm,
};

static struct clk_hw_omap dss1_alwon_fck_3430es2_hw = {
	.hw = {
		.clk = &dss1_alwon_fck_3430es2,
	},
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_DSS_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_DSS1_SHIFT,
	.clkdm_name	= "dss_clkdm",
};

DEFINE_STRUCT_CLK(dss1_alwon_fck_3430es2, dss1_alwon_fck_3430es2_parent_names, dss1_alwon_fck_3430es2_ops);

static struct clk dss2_alwon_fck;

static const char *dss2_alwon_fck_parent_names[] = {
	"sys_ck",
};

static struct clk_hw_omap dss2_alwon_fck_hw = {
	.hw = {
		.clk = &dss2_alwon_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_DSS_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_DSS2_SHIFT,
	.clkdm_name	= "dss_clkdm",
};

DEFINE_STRUCT_CLK(dss2_alwon_fck, dss2_alwon_fck_parent_names, aes2_ick_ops);

static struct clk dss_96m_fck;

static const char *dss_96m_fck_parent_names[] = {
	"omap_96m_fck",
};

static struct clk_hw_omap dss_96m_fck_hw = {
	.hw = {
		.clk = &dss_96m_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_DSS_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_TV_SHIFT,
	.clkdm_name	= "dss_clkdm",
};

DEFINE_STRUCT_CLK(dss_96m_fck, dss_96m_fck_parent_names, aes2_ick_ops);

static struct clk dss_ick_3430es1;

static const char *dss_ick_3430es1_parent_names[] = {
	"l4_ick",
};

static struct clk_hw_omap dss_ick_3430es1_hw = {
	.hw = {
		.clk = &dss_ick_3430es1,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_DSS_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430_CM_ICLKEN_DSS_EN_DSS_SHIFT,
	.clkdm_name	= "dss_clkdm",
};

DEFINE_STRUCT_CLK(dss_ick_3430es1, dss_ick_3430es1_parent_names, aes2_ick_ops);

static struct clk dss_ick_3430es2;

static const char *dss_ick_3430es2_parent_names[] = {
	"l4_ick",
};

static struct clk_hw_omap dss_ick_3430es2_hw = {
	.hw = {
		.clk = &dss_ick_3430es2,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_DSS_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430_CM_ICLKEN_DSS_EN_DSS_SHIFT,
	.clkdm_name	= "dss_clkdm",
};

DEFINE_STRUCT_CLK(dss_ick_3430es2, dss_ick_3430es2_parent_names, dss1_alwon_fck_3430es2_ops);

static struct clk dss_tv_fck;

static const char *dss_tv_fck_parent_names[] = {
	"omap_54m_fck",
};

static struct clk_hw_omap dss_tv_fck_hw = {
	.hw = {
		.clk = &dss_tv_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_DSS_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_TV_SHIFT,
	.clkdm_name	= "dss_clkdm",
};

DEFINE_STRUCT_CLK(dss_tv_fck, dss_tv_fck_parent_names, aes2_ick_ops);

static struct clk emac_fck;

static const char *emac_fck_parent_names[] = {
	"rmii_ck",
};

static struct clk_hw_omap emac_fck_hw = {
	.hw = {
		.clk = &emac_fck,
	},
	.enable_reg	= OMAP343X_CTRL_REGADDR(AM35XX_CONTROL_IPSS_CLK_CTRL),
	.enable_bit	= AM35XX_CPGMAC_FCLK_SHIFT,
};

DEFINE_STRUCT_CLK(emac_fck, emac_fck_parent_names, aes1_ick_ops);

static struct clk ipss_ick;

static const char *ipss_ick_parent_names[] = {
	"core_l3_ick",
};

static struct clk_hw_omap ipss_ick_hw = {
	.hw = {
		.clk = &ipss_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= AM35XX_EN_IPSS_SHIFT,
	.clkdm_name	= "core_l3_clkdm",
};

DEFINE_STRUCT_CLK(ipss_ick, ipss_ick_parent_names, dss1_alwon_fck_3430es2_ops);

static struct clk emac_ick;

static const char *emac_ick_parent_names[] = {
	"ipss_ick",
};

static struct clk_hw_omap emac_ick_hw = {
	.hw = {
		.clk = &emac_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP343X_CTRL_REGADDR(AM35XX_CONTROL_IPSS_CLK_CTRL),
	.enable_bit	= AM35XX_CPGMAC_VBUSP_CLK_SHIFT,
	.clkdm_name	= "core_l3_clkdm",
};

DEFINE_STRUCT_CLK(emac_ick, emac_ick_parent_names, dss1_alwon_fck_3430es2_ops);

static struct clk fac_ick;

static const char *fac_ick_parent_names[] = {
	"core_l4_ick",
};

static struct clk_hw_omap fac_ick_hw = {
	.hw = {
		.clk = &fac_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430ES1_EN_FAC_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(fac_ick, fac_ick_parent_names, aes2_ick_ops);

static struct clk fshostusb_fck;

static const char *fshostusb_fck_parent_names[] = {
	"core_48m_fck",
};

static struct clk_hw_omap fshostusb_fck_hw = {
	.hw = {
		.clk = &fshostusb_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP3430ES1_EN_FSHOSTUSB_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(fshostusb_fck, fshostusb_fck_parent_names, aes2_ick_ops);

static struct clk gfx_l3_ck;

static const char *gfx_l3_ck_parent_names[] = {
	"l3_ick",
};

static struct clk_hw_omap gfx_l3_ck_hw = {
	.hw = {
		.clk = &gfx_l3_ck,
	},
	.enable_reg	= OMAP_CM_REGADDR(GFX_MOD, CM_ICLKEN),
	.enable_bit	= OMAP_EN_GFX_SHIFT,
};

DEFINE_STRUCT_CLK(gfx_l3_ck, gfx_l3_ck_parent_names, aes1_ick_ops);

static const struct clksel gfx_l3_clksel[] = {
	{ .parent = &l3_ick, .rates = gfx_l3_rates },
	{ .parent = NULL },
};

static const char *gfx_l3_fck_parent_names[] = {
	"l3_ick",
};

static struct clk gfx_l3_fck;

static struct clk_hw_omap gfx_l3_fck_hw = {
	.hw = {
		.clk = &gfx_l3_fck,
	},
	.clksel		= gfx_l3_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(GFX_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP_CLKSEL_GFX_MASK,
};

DEFINE_STRUCT_CLK(gfx_l3_fck, gfx_l3_fck_parent_names, sys_ck_ops);

static struct clk gfx_cg1_ck;

static const char *gfx_cg1_ck_parent_names[] = {
	"gfx_l3_fck",
};

static struct clk_hw_omap gfx_cg1_ck_hw = {
	.hw = {
		.clk = &gfx_cg1_ck,
	},
	.enable_reg	= OMAP_CM_REGADDR(GFX_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430ES1_EN_2D_SHIFT,
	.clkdm_name	= "gfx_3430es1_clkdm",
};

DEFINE_STRUCT_CLK(gfx_cg1_ck, gfx_cg1_ck_parent_names, aes2_ick_ops);

static struct clk gfx_cg2_ck;

static const char *gfx_cg2_ck_parent_names[] = {
	"gfx_l3_fck",
};

static struct clk_hw_omap gfx_cg2_ck_hw = {
	.hw = {
		.clk = &gfx_cg2_ck,
	},
	.enable_reg	= OMAP_CM_REGADDR(GFX_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430ES1_EN_3D_SHIFT,
	.clkdm_name	= "gfx_3430es1_clkdm",
};

DEFINE_STRUCT_CLK(gfx_cg2_ck, gfx_cg2_ck_parent_names, aes2_ick_ops);

static struct clk gfx_l3_ick;

static const char *gfx_l3_ick_parent_names[] = {
	"gfx_l3_ck",
};

static struct clk_hw_omap gfx_l3_ick_hw = {
	.hw = {
		.clk = &gfx_l3_ick,
	},
};

DEFINE_STRUCT_CLK(gfx_l3_ick, gfx_l3_ick_parent_names, core_ck_ops);

static struct clk wkup_32k_fck;

static const char *wkup_32k_fck_parent_names[] = {
	"omap_32k_fck",
};

static struct clk_hw_omap wkup_32k_fck_hw = {
	.hw = {
		.clk = &wkup_32k_fck,
	},
};

DEFINE_STRUCT_CLK(wkup_32k_fck, wkup_32k_fck_parent_names, core_ck_ops);

static struct clk gpio1_dbck;

static const char *gpio1_dbck_parent_names[] = {
	"wkup_32k_fck",
};

static struct clk_hw_omap gpio1_dbck_hw = {
	.hw = {
		.clk = &gpio1_dbck,
	},
	.enable_reg	= OMAP_CM_REGADDR(WKUP_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_GPIO1_SHIFT,
	.clkdm_name	= "wkup_clkdm",
};

DEFINE_STRUCT_CLK(gpio1_dbck, gpio1_dbck_parent_names, aes2_ick_ops);

static struct clk wkup_l4_ick;

static const char *wkup_l4_ick_parent_names[] = {
	"sys_ck",
};

static struct clk_hw_omap wkup_l4_ick_hw = {
	.hw = {
		.clk = &wkup_l4_ick,
	},
};

DEFINE_STRUCT_CLK(wkup_l4_ick, wkup_l4_ick_parent_names, core_ck_ops);

static struct clk gpio1_ick;

static const char *gpio1_ick_parent_names[] = {
	"wkup_l4_ick",
};

static struct clk_hw_omap gpio1_ick_hw = {
	.hw = {
		.clk = &gpio1_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(WKUP_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430_EN_GPIO1_SHIFT,
	.clkdm_name	= "wkup_clkdm",
};

DEFINE_STRUCT_CLK(gpio1_ick, gpio1_ick_parent_names, aes2_ick_ops);

static struct clk per_32k_alwon_fck;

static const char *per_32k_alwon_fck_parent_names[] = {
	"omap_32k_fck",
};

static struct clk_hw_omap per_32k_alwon_fck_hw = {
	.hw = {
		.clk = &per_32k_alwon_fck,
	},
};

DEFINE_STRUCT_CLK(per_32k_alwon_fck, per_32k_alwon_fck_parent_names, core_ck_ops);

static struct clk gpio2_dbck;

static const char *gpio2_dbck_parent_names[] = {
	"per_32k_alwon_fck",
};

static struct clk_hw_omap gpio2_dbck_hw = {
	.hw = {
		.clk = &gpio2_dbck,
	},
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_GPIO2_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(gpio2_dbck, gpio2_dbck_parent_names, aes2_ick_ops);

static struct clk per_l4_ick;

static const char *per_l4_ick_parent_names[] = {
	"l4_ick",
};

static struct clk_hw_omap per_l4_ick_hw = {
	.hw = {
		.clk = &per_l4_ick,
	},
};

DEFINE_STRUCT_CLK(per_l4_ick, per_l4_ick_parent_names, core_ck_ops);

static struct clk gpio2_ick;

static const char *gpio2_ick_parent_names[] = {
	"per_l4_ick",
};

static struct clk_hw_omap gpio2_ick_hw = {
	.hw = {
		.clk = &gpio2_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430_EN_GPIO2_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(gpio2_ick, gpio2_ick_parent_names, aes2_ick_ops);

static struct clk gpio3_dbck;

static const char *gpio3_dbck_parent_names[] = {
	"per_32k_alwon_fck",
};

static struct clk_hw_omap gpio3_dbck_hw = {
	.hw = {
		.clk = &gpio3_dbck,
	},
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_GPIO3_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(gpio3_dbck, gpio3_dbck_parent_names, aes2_ick_ops);

static struct clk gpio3_ick;

static const char *gpio3_ick_parent_names[] = {
	"per_l4_ick",
};

static struct clk_hw_omap gpio3_ick_hw = {
	.hw = {
		.clk = &gpio3_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430_EN_GPIO3_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(gpio3_ick, gpio3_ick_parent_names, aes2_ick_ops);

static struct clk gpio4_dbck;

static const char *gpio4_dbck_parent_names[] = {
	"per_32k_alwon_fck",
};

static struct clk_hw_omap gpio4_dbck_hw = {
	.hw = {
		.clk = &gpio4_dbck,
	},
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_GPIO4_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(gpio4_dbck, gpio4_dbck_parent_names, aes2_ick_ops);

static struct clk gpio4_ick;

static const char *gpio4_ick_parent_names[] = {
	"per_l4_ick",
};

static struct clk_hw_omap gpio4_ick_hw = {
	.hw = {
		.clk = &gpio4_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430_EN_GPIO4_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(gpio4_ick, gpio4_ick_parent_names, aes2_ick_ops);

static struct clk gpio5_dbck;

static const char *gpio5_dbck_parent_names[] = {
	"per_32k_alwon_fck",
};

static struct clk_hw_omap gpio5_dbck_hw = {
	.hw = {
		.clk = &gpio5_dbck,
	},
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_GPIO5_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(gpio5_dbck, gpio5_dbck_parent_names, aes2_ick_ops);

static struct clk gpio5_ick;

static const char *gpio5_ick_parent_names[] = {
	"per_l4_ick",
};

static struct clk_hw_omap gpio5_ick_hw = {
	.hw = {
		.clk = &gpio5_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430_EN_GPIO5_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(gpio5_ick, gpio5_ick_parent_names, aes2_ick_ops);

static struct clk gpio6_dbck;

static const char *gpio6_dbck_parent_names[] = {
	"per_32k_alwon_fck",
};

static struct clk_hw_omap gpio6_dbck_hw = {
	.hw = {
		.clk = &gpio6_dbck,
	},
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_GPIO6_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(gpio6_dbck, gpio6_dbck_parent_names, aes2_ick_ops);

static struct clk gpio6_ick;

static const char *gpio6_ick_parent_names[] = {
	"per_l4_ick",
};

static struct clk_hw_omap gpio6_ick_hw = {
	.hw = {
		.clk = &gpio6_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430_EN_GPIO6_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(gpio6_ick, gpio6_ick_parent_names, aes2_ick_ops);

static struct clk gpmc_fck;

static const char *gpmc_fck_parent_names[] = {
	"core_l3_ick",
};

static struct clk_hw_omap gpmc_fck_hw = {
	.hw = {
		.clk = &gpmc_fck,
	},
	.flags		= ENABLE_ON_INIT,
};

DEFINE_STRUCT_CLK(gpmc_fck, gpmc_fck_parent_names, core_ck_ops);

static const struct clksel omap343x_gpt_clksel[] = {
	{ .parent = &omap_32k_fck, .rates = gpt_32k_rates },
	{ .parent = &sys_ck, .rates = gpt_sys_rates },
	{ .parent = NULL },
};

static const char *gpt10_fck_parent_names[] = {
	"omap_32k_fck",
	"sys_ck",
};

static struct clk gpt10_fck;

static struct clk_hw_omap gpt10_fck_hw = {
	.hw = {
		.clk = &gpt10_fck,
	},
	.clksel		= omap343x_gpt_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP3430_CLKSEL_GPT10_MASK,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP3430_EN_GPT10_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(gpt10_fck, gpt10_fck_parent_names, clkout2_src_ck_ops);

static struct clk gpt10_ick;

static const char *gpt10_ick_parent_names[] = {
	"core_l4_ick",
};

static struct clk_hw_omap gpt10_ick_hw = {
	.hw = {
		.clk = &gpt10_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_GPT10_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(gpt10_ick, gpt10_ick_parent_names, aes2_ick_ops);

static struct clk gpt11_fck;

static struct clk_hw_omap gpt11_fck_hw = {
	.hw = {
		.clk = &gpt11_fck,
	},
	.clksel		= omap343x_gpt_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP3430_CLKSEL_GPT11_MASK,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP3430_EN_GPT11_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(gpt11_fck, gpt10_fck_parent_names, clkout2_src_ck_ops);

static struct clk gpt11_ick;

static const char *gpt11_ick_parent_names[] = {
	"core_l4_ick",
};

static struct clk_hw_omap gpt11_ick_hw = {
	.hw = {
		.clk = &gpt11_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_GPT11_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(gpt11_ick, gpt11_ick_parent_names, aes2_ick_ops);

static struct clk gpt12_fck;

static const char *gpt12_fck_parent_names[] = {
	"secure_32k_fck",
};

static struct clk_hw_omap gpt12_fck_hw = {
	.hw = {
		.clk = &gpt12_fck,
	},
};

DEFINE_STRUCT_CLK(gpt12_fck, gpt12_fck_parent_names, core_ck_ops);

static struct clk gpt12_ick;

static const char *gpt12_ick_parent_names[] = {
	"wkup_l4_ick",
};

static struct clk_hw_omap gpt12_ick_hw = {
	.hw = {
		.clk = &gpt12_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(WKUP_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430_EN_GPT12_SHIFT,
	.clkdm_name	= "wkup_clkdm",
};

DEFINE_STRUCT_CLK(gpt12_ick, gpt12_ick_parent_names, aes2_ick_ops);

static struct clk gpt1_fck;

static struct clk_hw_omap gpt1_fck_hw = {
	.hw = {
		.clk = &gpt1_fck,
	},
	.clksel		= omap343x_gpt_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(WKUP_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP3430_CLKSEL_GPT1_MASK,
	.enable_reg	= OMAP_CM_REGADDR(WKUP_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_GPT1_SHIFT,
	.clkdm_name	= "wkup_clkdm",
};

DEFINE_STRUCT_CLK(gpt1_fck, gpt10_fck_parent_names, clkout2_src_ck_ops);

static struct clk gpt1_ick;

static const char *gpt1_ick_parent_names[] = {
	"wkup_l4_ick",
};

static struct clk_hw_omap gpt1_ick_hw = {
	.hw = {
		.clk = &gpt1_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(WKUP_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430_EN_GPT1_SHIFT,
	.clkdm_name	= "wkup_clkdm",
};

DEFINE_STRUCT_CLK(gpt1_ick, gpt1_ick_parent_names, aes2_ick_ops);

static struct clk gpt2_fck;

static struct clk_hw_omap gpt2_fck_hw = {
	.hw = {
		.clk = &gpt2_fck,
	},
	.clksel		= omap343x_gpt_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP3430_CLKSEL_GPT2_MASK,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_GPT2_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(gpt2_fck, gpt10_fck_parent_names, clkout2_src_ck_ops);

static struct clk gpt2_ick;

static const char *gpt2_ick_parent_names[] = {
	"per_l4_ick",
};

static struct clk_hw_omap gpt2_ick_hw = {
	.hw = {
		.clk = &gpt2_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430_EN_GPT2_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(gpt2_ick, gpt2_ick_parent_names, aes2_ick_ops);

static struct clk gpt3_fck;

static struct clk_hw_omap gpt3_fck_hw = {
	.hw = {
		.clk = &gpt3_fck,
	},
	.clksel		= omap343x_gpt_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP3430_CLKSEL_GPT3_MASK,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_GPT3_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(gpt3_fck, gpt10_fck_parent_names, clkout2_src_ck_ops);

static struct clk gpt3_ick;

static const char *gpt3_ick_parent_names[] = {
	"per_l4_ick",
};

static struct clk_hw_omap gpt3_ick_hw = {
	.hw = {
		.clk = &gpt3_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430_EN_GPT3_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(gpt3_ick, gpt3_ick_parent_names, aes2_ick_ops);

static struct clk gpt4_fck;

static struct clk_hw_omap gpt4_fck_hw = {
	.hw = {
		.clk = &gpt4_fck,
	},
	.clksel		= omap343x_gpt_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP3430_CLKSEL_GPT4_MASK,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_GPT4_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(gpt4_fck, gpt10_fck_parent_names, clkout2_src_ck_ops);

static struct clk gpt4_ick;

static const char *gpt4_ick_parent_names[] = {
	"per_l4_ick",
};

static struct clk_hw_omap gpt4_ick_hw = {
	.hw = {
		.clk = &gpt4_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430_EN_GPT4_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(gpt4_ick, gpt4_ick_parent_names, aes2_ick_ops);

static struct clk gpt5_fck;

static struct clk_hw_omap gpt5_fck_hw = {
	.hw = {
		.clk = &gpt5_fck,
	},
	.clksel		= omap343x_gpt_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP3430_CLKSEL_GPT5_MASK,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_GPT5_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(gpt5_fck, gpt10_fck_parent_names, clkout2_src_ck_ops);

static struct clk gpt5_ick;

static const char *gpt5_ick_parent_names[] = {
	"per_l4_ick",
};

static struct clk_hw_omap gpt5_ick_hw = {
	.hw = {
		.clk = &gpt5_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430_EN_GPT5_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(gpt5_ick, gpt5_ick_parent_names, aes2_ick_ops);

static struct clk gpt6_fck;

static struct clk_hw_omap gpt6_fck_hw = {
	.hw = {
		.clk = &gpt6_fck,
	},
	.clksel		= omap343x_gpt_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP3430_CLKSEL_GPT6_MASK,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_GPT6_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(gpt6_fck, gpt10_fck_parent_names, clkout2_src_ck_ops);

static struct clk gpt6_ick;

static const char *gpt6_ick_parent_names[] = {
	"per_l4_ick",
};

static struct clk_hw_omap gpt6_ick_hw = {
	.hw = {
		.clk = &gpt6_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430_EN_GPT6_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(gpt6_ick, gpt6_ick_parent_names, aes2_ick_ops);

static struct clk gpt7_fck;

static struct clk_hw_omap gpt7_fck_hw = {
	.hw = {
		.clk = &gpt7_fck,
	},
	.clksel		= omap343x_gpt_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP3430_CLKSEL_GPT7_MASK,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_GPT7_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(gpt7_fck, gpt10_fck_parent_names, clkout2_src_ck_ops);

static struct clk gpt7_ick;

static const char *gpt7_ick_parent_names[] = {
	"per_l4_ick",
};

static struct clk_hw_omap gpt7_ick_hw = {
	.hw = {
		.clk = &gpt7_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430_EN_GPT7_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(gpt7_ick, gpt7_ick_parent_names, aes2_ick_ops);

static struct clk gpt8_fck;

static struct clk_hw_omap gpt8_fck_hw = {
	.hw = {
		.clk = &gpt8_fck,
	},
	.clksel		= omap343x_gpt_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP3430_CLKSEL_GPT8_MASK,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_GPT8_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(gpt8_fck, gpt10_fck_parent_names, clkout2_src_ck_ops);

static struct clk gpt8_ick;

static const char *gpt8_ick_parent_names[] = {
	"per_l4_ick",
};

static struct clk_hw_omap gpt8_ick_hw = {
	.hw = {
		.clk = &gpt8_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430_EN_GPT8_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(gpt8_ick, gpt8_ick_parent_names, aes2_ick_ops);

static struct clk gpt9_fck;

static struct clk_hw_omap gpt9_fck_hw = {
	.hw = {
		.clk = &gpt9_fck,
	},
	.clksel		= omap343x_gpt_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP3430_CLKSEL_GPT9_MASK,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_GPT9_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(gpt9_fck, gpt10_fck_parent_names, clkout2_src_ck_ops);

static struct clk gpt9_ick;

static const char *gpt9_ick_parent_names[] = {
	"per_l4_ick",
};

static struct clk_hw_omap gpt9_ick_hw = {
	.hw = {
		.clk = &gpt9_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430_EN_GPT9_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(gpt9_ick, gpt9_ick_parent_names, aes2_ick_ops);

static struct clk hdq_fck;

static const char *hdq_fck_parent_names[] = {
	"core_12m_fck",
};

static struct clk_hw_omap hdq_fck_hw = {
	.hw = {
		.clk = &hdq_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP3430_EN_HDQ_SHIFT,
};

DEFINE_STRUCT_CLK(hdq_fck, hdq_fck_parent_names, aes1_ick_ops);

static struct clk hdq_ick;

static const char *hdq_ick_parent_names[] = {
	"core_l4_ick",
};

static struct clk_hw_omap hdq_ick_hw = {
	.hw = {
		.clk = &hdq_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_HDQ_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(hdq_ick, hdq_ick_parent_names, aes2_ick_ops);

static struct clk hecc_ck;

static const char *hecc_ck_parent_names[] = {
	"sys_ck",
};

static struct clk_hw_omap hecc_ck_hw = {
	.hw = {
		.clk = &hecc_ck,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP343X_CTRL_REGADDR(AM35XX_CONTROL_IPSS_CLK_CTRL),
	.enable_bit	= AM35XX_HECC_VBUSP_CLK_SHIFT,
	.clkdm_name	= "core_l3_clkdm",
};

DEFINE_STRUCT_CLK(hecc_ck, hecc_ck_parent_names, dss1_alwon_fck_3430es2_ops);

static struct clk hsotgusb_fck_am35xx;

static const char *hsotgusb_fck_am35xx_parent_names[] = {
	"sys_ck",
};

static struct clk_hw_omap hsotgusb_fck_am35xx_hw = {
	.hw = {
		.clk = &hsotgusb_fck_am35xx,
	},
	.enable_reg	= OMAP343X_CTRL_REGADDR(AM35XX_CONTROL_IPSS_CLK_CTRL),
	.enable_bit	= AM35XX_USBOTG_FCLK_SHIFT,
	.clkdm_name	= "core_l3_clkdm",
};

DEFINE_STRUCT_CLK(hsotgusb_fck_am35xx, hsotgusb_fck_am35xx_parent_names, aes2_ick_ops);

static struct clk hsotgusb_ick_3430es1;

static const char *hsotgusb_ick_3430es1_parent_names[] = {
	"core_l3_ick",
};

static struct clk_hw_omap hsotgusb_ick_3430es1_hw = {
	.hw = {
		.clk = &hsotgusb_ick_3430es1,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_HSOTGUSB_SHIFT,
	.clkdm_name	= "core_l3_clkdm",
};

DEFINE_STRUCT_CLK(hsotgusb_ick_3430es1, hsotgusb_ick_3430es1_parent_names, aes2_ick_ops);

static struct clk hsotgusb_ick_3430es2;

static const char *hsotgusb_ick_3430es2_parent_names[] = {
	"core_l3_ick",
};

static struct clk_hw_omap hsotgusb_ick_3430es2_hw = {
	.hw = {
		.clk = &hsotgusb_ick_3430es2,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_HSOTGUSB_SHIFT,
	.clkdm_name	= "core_l3_clkdm",
};

DEFINE_STRUCT_CLK(hsotgusb_ick_3430es2, hsotgusb_ick_3430es2_parent_names, dss1_alwon_fck_3430es2_ops);

static struct clk hsotgusb_ick_am35xx;

static const char *hsotgusb_ick_am35xx_parent_names[] = {
	"ipss_ick",
};

static struct clk_hw_omap hsotgusb_ick_am35xx_hw = {
	.hw = {
		.clk = &hsotgusb_ick_am35xx,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP343X_CTRL_REGADDR(AM35XX_CONTROL_IPSS_CLK_CTRL),
	.enable_bit	= AM35XX_USBOTG_VBUSP_CLK_SHIFT,
	.clkdm_name	= "core_l3_clkdm",
};

DEFINE_STRUCT_CLK(hsotgusb_ick_am35xx, hsotgusb_ick_am35xx_parent_names, dss1_alwon_fck_3430es2_ops);

static struct clk i2c1_fck;

static const char *i2c1_fck_parent_names[] = {
	"core_96m_fck",
};

static struct clk_hw_omap i2c1_fck_hw = {
	.hw = {
		.clk = &i2c1_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP3430_EN_I2C1_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(i2c1_fck, i2c1_fck_parent_names, aes2_ick_ops);

static struct clk i2c1_ick;

static const char *i2c1_ick_parent_names[] = {
	"core_l4_ick",
};

static struct clk_hw_omap i2c1_ick_hw = {
	.hw = {
		.clk = &i2c1_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_I2C1_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(i2c1_ick, i2c1_ick_parent_names, aes2_ick_ops);

static struct clk i2c2_fck;

static const char *i2c2_fck_parent_names[] = {
	"core_96m_fck",
};

static struct clk_hw_omap i2c2_fck_hw = {
	.hw = {
		.clk = &i2c2_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP3430_EN_I2C2_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(i2c2_fck, i2c2_fck_parent_names, aes2_ick_ops);

static struct clk i2c2_ick;

static const char *i2c2_ick_parent_names[] = {
	"core_l4_ick",
};

static struct clk_hw_omap i2c2_ick_hw = {
	.hw = {
		.clk = &i2c2_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_I2C2_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(i2c2_ick, i2c2_ick_parent_names, aes2_ick_ops);

static struct clk i2c3_fck;

static const char *i2c3_fck_parent_names[] = {
	"core_96m_fck",
};

static struct clk_hw_omap i2c3_fck_hw = {
	.hw = {
		.clk = &i2c3_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP3430_EN_I2C3_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(i2c3_fck, i2c3_fck_parent_names, aes2_ick_ops);

static struct clk i2c3_ick;

static const char *i2c3_ick_parent_names[] = {
	"core_l4_ick",
};

static struct clk_hw_omap i2c3_ick_hw = {
	.hw = {
		.clk = &i2c3_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_I2C3_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(i2c3_ick, i2c3_ick_parent_names, aes2_ick_ops);

static struct clk icr_ick;

static const char *icr_ick_parent_names[] = {
	"core_l4_ick",
};

static struct clk_hw_omap icr_ick_hw = {
	.hw = {
		.clk = &icr_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_ICR_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(icr_ick, icr_ick_parent_names, aes2_ick_ops);

static struct clk iva2_ck;

static const char *iva2_ck_parent_names[] = {
	"dpll2_m2_ck",
};

static struct clk_hw_omap iva2_ck_hw = {
	.hw = {
		.clk = &iva2_ck,
	},
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_IVA2_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_CM_FCLKEN_IVA2_EN_IVA2_SHIFT,
	.clkdm_name	= "iva2_clkdm",
};

DEFINE_STRUCT_CLK(iva2_ck, iva2_ck_parent_names, aes2_ick_ops);

static struct clk mad2d_ick;

static const char *mad2d_ick_parent_names[] = {
	"l3_ick",
};

static struct clk_hw_omap mad2d_ick_hw = {
	.hw = {
		.clk = &mad2d_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN3),
	.enable_bit	= OMAP3430_EN_MAD2D_SHIFT,
	.clkdm_name	= "d2d_clkdm",
};

DEFINE_STRUCT_CLK(mad2d_ick, mad2d_ick_parent_names, aes2_ick_ops);

static struct clk mailboxes_ick;

static const char *mailboxes_ick_parent_names[] = {
	"core_l4_ick",
};

static struct clk_hw_omap mailboxes_ick_hw = {
	.hw = {
		.clk = &mailboxes_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_MAILBOXES_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mailboxes_ick, mailboxes_ick_parent_names, aes2_ick_ops);

static const struct clksel_rate common_mcbsp_96m_rates[] = {
	{ .div = 1, .val = 0, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel_rate common_mcbsp_mcbsp_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel mcbsp_15_clksel[] = {
	{ .parent = &core_96m_fck, .rates = common_mcbsp_96m_rates },
	{ .parent = &mcbsp_clks, .rates = common_mcbsp_mcbsp_rates },
	{ .parent = NULL },
};

static const char *mcbsp1_fck_parent_names[] = {
	"core_96m_fck",
	"mcbsp_clks",
};

static struct clk mcbsp1_fck;

static struct clk_hw_omap mcbsp1_fck_hw = {
	.hw = {
		.clk = &mcbsp1_fck,
	},
	.clksel		= mcbsp_15_clksel,
	.clksel_reg	= OMAP343X_CTRL_REGADDR(OMAP2_CONTROL_DEVCONF0),
	.clksel_mask	= OMAP2_MCBSP1_CLKS_MASK,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP3430_EN_MCBSP1_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mcbsp1_fck, mcbsp1_fck_parent_names, clkout2_src_ck_ops);

static struct clk mcbsp1_ick;

static const char *mcbsp1_ick_parent_names[] = {
	"core_l4_ick",
};

static struct clk_hw_omap mcbsp1_ick_hw = {
	.hw = {
		.clk = &mcbsp1_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_MCBSP1_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mcbsp1_ick, mcbsp1_ick_parent_names, aes2_ick_ops);

static struct clk per_96m_fck;

static const char *per_96m_fck_parent_names[] = {
	"omap_96m_alwon_fck",
};

static struct clk_hw_omap per_96m_fck_hw = {
	.hw = {
		.clk = &per_96m_fck,
	},
};

DEFINE_STRUCT_CLK(per_96m_fck, per_96m_fck_parent_names, core_ck_ops);

static const struct clksel mcbsp_234_clksel[] = {
	{ .parent = &per_96m_fck, .rates = common_mcbsp_96m_rates },
	{ .parent = &mcbsp_clks, .rates = common_mcbsp_mcbsp_rates },
	{ .parent = NULL },
};

static const char *mcbsp2_fck_parent_names[] = {
	"per_96m_fck",
	"mcbsp_clks",
};

static struct clk mcbsp2_fck;

static struct clk_hw_omap mcbsp2_fck_hw = {
	.hw = {
		.clk = &mcbsp2_fck,
	},
	.clksel		= mcbsp_234_clksel,
	.clksel_reg	= OMAP343X_CTRL_REGADDR(OMAP2_CONTROL_DEVCONF0),
	.clksel_mask	= OMAP2_MCBSP2_CLKS_MASK,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_MCBSP2_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(mcbsp2_fck, mcbsp2_fck_parent_names, clkout2_src_ck_ops);

static struct clk mcbsp2_ick;

static const char *mcbsp2_ick_parent_names[] = {
	"per_l4_ick",
};

static struct clk_hw_omap mcbsp2_ick_hw = {
	.hw = {
		.clk = &mcbsp2_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430_EN_MCBSP2_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(mcbsp2_ick, mcbsp2_ick_parent_names, aes2_ick_ops);

static struct clk mcbsp3_fck;

static struct clk_hw_omap mcbsp3_fck_hw = {
	.hw = {
		.clk = &mcbsp3_fck,
	},
	.clksel		= mcbsp_234_clksel,
	.clksel_reg	= OMAP343X_CTRL_REGADDR(OMAP343X_CONTROL_DEVCONF1),
	.clksel_mask	= OMAP2_MCBSP3_CLKS_MASK,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_MCBSP3_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(mcbsp3_fck, mcbsp2_fck_parent_names, clkout2_src_ck_ops);

static struct clk mcbsp3_ick;

static const char *mcbsp3_ick_parent_names[] = {
	"per_l4_ick",
};

static struct clk_hw_omap mcbsp3_ick_hw = {
	.hw = {
		.clk = &mcbsp3_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430_EN_MCBSP3_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(mcbsp3_ick, mcbsp3_ick_parent_names, aes2_ick_ops);

static struct clk mcbsp4_fck;

static struct clk_hw_omap mcbsp4_fck_hw = {
	.hw = {
		.clk = &mcbsp4_fck,
	},
	.clksel		= mcbsp_234_clksel,
	.clksel_reg	= OMAP343X_CTRL_REGADDR(OMAP343X_CONTROL_DEVCONF1),
	.clksel_mask	= OMAP2_MCBSP4_CLKS_MASK,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_MCBSP4_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(mcbsp4_fck, mcbsp2_fck_parent_names, clkout2_src_ck_ops);

static struct clk mcbsp4_ick;

static const char *mcbsp4_ick_parent_names[] = {
	"per_l4_ick",
};

static struct clk_hw_omap mcbsp4_ick_hw = {
	.hw = {
		.clk = &mcbsp4_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430_EN_MCBSP4_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(mcbsp4_ick, mcbsp4_ick_parent_names, aes2_ick_ops);

static struct clk mcbsp5_fck;

static struct clk_hw_omap mcbsp5_fck_hw = {
	.hw = {
		.clk = &mcbsp5_fck,
	},
	.clksel		= mcbsp_15_clksel,
	.clksel_reg	= OMAP343X_CTRL_REGADDR(OMAP343X_CONTROL_DEVCONF1),
	.clksel_mask	= OMAP2_MCBSP5_CLKS_MASK,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP3430_EN_MCBSP5_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mcbsp5_fck, mcbsp1_fck_parent_names, clkout2_src_ck_ops);

static struct clk mcbsp5_ick;

static const char *mcbsp5_ick_parent_names[] = {
	"core_l4_ick",
};

static struct clk_hw_omap mcbsp5_ick_hw = {
	.hw = {
		.clk = &mcbsp5_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_MCBSP5_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mcbsp5_ick, mcbsp5_ick_parent_names, aes2_ick_ops);

static struct clk mcspi1_fck;

static const char *mcspi1_fck_parent_names[] = {
	"core_48m_fck",
};

static struct clk_hw_omap mcspi1_fck_hw = {
	.hw = {
		.clk = &mcspi1_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP3430_EN_MCSPI1_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mcspi1_fck, mcspi1_fck_parent_names, aes2_ick_ops);

static struct clk mcspi1_ick;

static const char *mcspi1_ick_parent_names[] = {
	"core_l4_ick",
};

static struct clk_hw_omap mcspi1_ick_hw = {
	.hw = {
		.clk = &mcspi1_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_MCSPI1_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mcspi1_ick, mcspi1_ick_parent_names, aes2_ick_ops);

static struct clk mcspi2_fck;

static const char *mcspi2_fck_parent_names[] = {
	"core_48m_fck",
};

static struct clk_hw_omap mcspi2_fck_hw = {
	.hw = {
		.clk = &mcspi2_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP3430_EN_MCSPI2_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mcspi2_fck, mcspi2_fck_parent_names, aes2_ick_ops);

static struct clk mcspi2_ick;

static const char *mcspi2_ick_parent_names[] = {
	"core_l4_ick",
};

static struct clk_hw_omap mcspi2_ick_hw = {
	.hw = {
		.clk = &mcspi2_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_MCSPI2_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mcspi2_ick, mcspi2_ick_parent_names, aes2_ick_ops);

static struct clk mcspi3_fck;

static const char *mcspi3_fck_parent_names[] = {
	"core_48m_fck",
};

static struct clk_hw_omap mcspi3_fck_hw = {
	.hw = {
		.clk = &mcspi3_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP3430_EN_MCSPI3_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mcspi3_fck, mcspi3_fck_parent_names, aes2_ick_ops);

static struct clk mcspi3_ick;

static const char *mcspi3_ick_parent_names[] = {
	"core_l4_ick",
};

static struct clk_hw_omap mcspi3_ick_hw = {
	.hw = {
		.clk = &mcspi3_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_MCSPI3_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mcspi3_ick, mcspi3_ick_parent_names, aes2_ick_ops);

static struct clk mcspi4_fck;

static const char *mcspi4_fck_parent_names[] = {
	"core_48m_fck",
};

static struct clk_hw_omap mcspi4_fck_hw = {
	.hw = {
		.clk = &mcspi4_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP3430_EN_MCSPI4_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mcspi4_fck, mcspi4_fck_parent_names, aes2_ick_ops);

static struct clk mcspi4_ick;

static const char *mcspi4_ick_parent_names[] = {
	"core_l4_ick",
};

static struct clk_hw_omap mcspi4_ick_hw = {
	.hw = {
		.clk = &mcspi4_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_MCSPI4_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mcspi4_ick, mcspi4_ick_parent_names, aes2_ick_ops);

static struct clk mmchs1_fck;

static const char *mmchs1_fck_parent_names[] = {
	"core_96m_fck",
};

static struct clk_hw_omap mmchs1_fck_hw = {
	.hw = {
		.clk = &mmchs1_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP3430_EN_MMC1_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mmchs1_fck, mmchs1_fck_parent_names, aes2_ick_ops);

static struct clk mmchs1_ick;

static const char *mmchs1_ick_parent_names[] = {
	"core_l4_ick",
};

static struct clk_hw_omap mmchs1_ick_hw = {
	.hw = {
		.clk = &mmchs1_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_MMC1_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mmchs1_ick, mmchs1_ick_parent_names, aes2_ick_ops);

static struct clk mmchs2_fck;

static const char *mmchs2_fck_parent_names[] = {
	"core_96m_fck",
};

static struct clk_hw_omap mmchs2_fck_hw = {
	.hw = {
		.clk = &mmchs2_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP3430_EN_MMC2_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mmchs2_fck, mmchs2_fck_parent_names, aes2_ick_ops);

static struct clk mmchs2_ick;

static const char *mmchs2_ick_parent_names[] = {
	"core_l4_ick",
};

static struct clk_hw_omap mmchs2_ick_hw = {
	.hw = {
		.clk = &mmchs2_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_MMC2_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mmchs2_ick, mmchs2_ick_parent_names, aes2_ick_ops);

static struct clk mmchs3_fck;

static const char *mmchs3_fck_parent_names[] = {
	"core_96m_fck",
};

static struct clk_hw_omap mmchs3_fck_hw = {
	.hw = {
		.clk = &mmchs3_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP3430ES2_EN_MMC3_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mmchs3_fck, mmchs3_fck_parent_names, aes2_ick_ops);

static struct clk mmchs3_ick;

static const char *mmchs3_ick_parent_names[] = {
	"core_l4_ick",
};

static struct clk_hw_omap mmchs3_ick_hw = {
	.hw = {
		.clk = &mmchs3_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430ES2_EN_MMC3_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mmchs3_ick, mmchs3_ick_parent_names, aes2_ick_ops);

static struct clk modem_fck;

static const char *modem_fck_parent_names[] = {
	"sys_ck",
};

static struct clk_hw_omap modem_fck_hw = {
	.hw = {
		.clk = &modem_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP3430_EN_MODEM_SHIFT,
	.clkdm_name	= "d2d_clkdm",
};

DEFINE_STRUCT_CLK(modem_fck, modem_fck_parent_names, aes2_ick_ops);

static struct clk mspro_fck;

static const char *mspro_fck_parent_names[] = {
	"core_96m_fck",
};

static struct clk_hw_omap mspro_fck_hw = {
	.hw = {
		.clk = &mspro_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP3430_EN_MSPRO_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mspro_fck, mspro_fck_parent_names, aes2_ick_ops);

static struct clk mspro_ick;

static const char *mspro_ick_parent_names[] = {
	"core_l4_ick",
};

static struct clk_hw_omap mspro_ick_hw = {
	.hw = {
		.clk = &mspro_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_MSPRO_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mspro_ick, mspro_ick_parent_names, aes2_ick_ops);

static struct clk omap_192m_alwon_fck;

static const char *omap_192m_alwon_fck_parent_names[] = {
	"dpll4_m2x2_ck",
};

static struct clk_hw_omap omap_192m_alwon_fck_hw = {
	.hw = {
		.clk = &omap_192m_alwon_fck,
	},
};

DEFINE_STRUCT_CLK(omap_192m_alwon_fck, omap_192m_alwon_fck_parent_names, core_ck_ops);

static struct clk omap_32ksync_ick;

static const char *omap_32ksync_ick_parent_names[] = {
	"wkup_l4_ick",
};

static struct clk_hw_omap omap_32ksync_ick_hw = {
	.hw = {
		.clk = &omap_32ksync_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(WKUP_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430_EN_32KSYNC_SHIFT,
	.clkdm_name	= "wkup_clkdm",
};

DEFINE_STRUCT_CLK(omap_32ksync_ick, omap_32ksync_ick_parent_names, aes2_ick_ops);

static const struct clksel_rate omap_96m_alwon_fck_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_36XX },
	{ .div = 2, .val = 2, .flags = RATE_IN_36XX },
	{ .div = 0 }
};

static const struct clksel omap_96m_alwon_fck_clksel[] = {
	{ .parent = &omap_192m_alwon_fck, .rates = omap_96m_alwon_fck_rates },
	{ .parent = NULL },
};

static const char *omap_96m_alwon_fck_3630_parent_names[] = {
	"omap_192m_alwon_fck",
};

static struct clk omap_96m_alwon_fck_3630;

static struct clk_hw_omap omap_96m_alwon_fck_3630_hw = {
	.hw = {
		.clk = &omap_96m_alwon_fck_3630,
	},
	.clksel		= omap_96m_alwon_fck_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP3630_CLKSEL_96M_MASK,
};

DEFINE_STRUCT_CLK(omap_96m_alwon_fck_3630, omap_96m_alwon_fck_3630_parent_names, sys_ck_ops);

static struct clk omapctrl_ick;

static const char *omapctrl_ick_parent_names[] = {
	"core_l4_ick",
};

static struct clk_hw_omap omapctrl_ick_hw = {
	.hw = {
		.clk = &omapctrl_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_OMAPCTRL_SHIFT,
	.flags		= ENABLE_ON_INIT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(omapctrl_ick, omapctrl_ick_parent_names, aes2_ick_ops);

static const struct clksel_rate pclk_emu_rates[] = {
	{ .div = 2, .val = 2, .flags = RATE_IN_3XXX },
	{ .div = 3, .val = 3, .flags = RATE_IN_3XXX },
	{ .div = 4, .val = 4, .flags = RATE_IN_3XXX },
	{ .div = 6, .val = 6, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel pclk_emu_clksel[] = {
	{ .parent = &emu_src_ck, .rates = pclk_emu_rates },
	{ .parent = NULL },
};

static const char *pclk_fck_parent_names[] = {
	"emu_src_ck",
};

static struct clk pclk_fck;

static struct clk_hw_omap pclk_fck_hw = {
	.hw = {
		.clk = &pclk_fck,
	},
	.clksel		= pclk_emu_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(OMAP3430_EMU_MOD, CM_CLKSEL1),
	.clksel_mask	= OMAP3430_CLKSEL_PCLK_MASK,
};

DEFINE_STRUCT_CLK(pclk_fck, pclk_fck_parent_names, sys_ck_ops);

static const struct clksel_rate pclkx2_emu_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_3XXX },
	{ .div = 2, .val = 2, .flags = RATE_IN_3XXX },
	{ .div = 3, .val = 3, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel pclkx2_emu_clksel[] = {
	{ .parent = &emu_src_ck, .rates = pclkx2_emu_rates },
	{ .parent = NULL },
};

static const char *pclkx2_fck_parent_names[] = {
	"emu_src_ck",
};

static struct clk pclkx2_fck;

static struct clk_hw_omap pclkx2_fck_hw = {
	.hw = {
		.clk = &pclkx2_fck,
	},
	.clksel		= pclkx2_emu_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(OMAP3430_EMU_MOD, CM_CLKSEL1),
	.clksel_mask	= OMAP3430_CLKSEL_PCLKX2_MASK,
};

DEFINE_STRUCT_CLK(pclkx2_fck, pclkx2_fck_parent_names, sys_ck_ops);

static struct clk per_48m_fck;

static const char *per_48m_fck_parent_names[] = {
	"omap_48m_fck",
};

static struct clk_hw_omap per_48m_fck_hw = {
	.hw = {
		.clk = &per_48m_fck,
	},
};

DEFINE_STRUCT_CLK(per_48m_fck, per_48m_fck_parent_names, core_ck_ops);

static struct clk security_l3_ick;

static const char *security_l3_ick_parent_names[] = {
	"l3_ick",
};

static struct clk_hw_omap security_l3_ick_hw = {
	.hw = {
		.clk = &security_l3_ick,
	},
};

DEFINE_STRUCT_CLK(security_l3_ick, security_l3_ick_parent_names, core_ck_ops);

static struct clk pka_ick;

static const char *pka_ick_parent_names[] = {
	"security_l3_ick",
};

static struct clk_hw_omap pka_ick_hw = {
	.hw = {
		.clk = &pka_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN2),
	.enable_bit	= OMAP3430_EN_PKA_SHIFT,
};

DEFINE_STRUCT_CLK(pka_ick, pka_ick_parent_names, aes1_ick_ops);

static const struct clksel div2_l4_clksel[] = {
	{ .parent = &l4_ick, .rates = div2_rates },
	{ .parent = NULL },
};

static const char *rm_ick_parent_names[] = {
	"l4_ick",
};

static struct clk rm_ick;

static struct clk_hw_omap rm_ick_hw = {
	.hw = {
		.clk = &rm_ick,
	},
	.clksel		= div2_l4_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(WKUP_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP3430_CLKSEL_RM_MASK,
};

DEFINE_STRUCT_CLK(rm_ick, rm_ick_parent_names, sys_ck_ops);

static struct clk rng_ick;

static const char *rng_ick_parent_names[] = {
	"security_l4_ick2",
};

static struct clk_hw_omap rng_ick_hw = {
	.hw = {
		.clk = &rng_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN2),
	.enable_bit	= OMAP3430_EN_RNG_SHIFT,
};

DEFINE_STRUCT_CLK(rng_ick, rng_ick_parent_names, aes1_ick_ops);

static struct clk sad2d_ick;

static const char *sad2d_ick_parent_names[] = {
	"l3_ick",
};

static struct clk_hw_omap sad2d_ick_hw = {
	.hw = {
		.clk = &sad2d_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_SAD2D_SHIFT,
	.clkdm_name	= "d2d_clkdm",
};

DEFINE_STRUCT_CLK(sad2d_ick, sad2d_ick_parent_names, aes2_ick_ops);

static struct clk sdrc_ick;

static const char *sdrc_ick_parent_names[] = {
	"core_l3_ick",
};

static struct clk_hw_omap sdrc_ick_hw = {
	.hw = {
		.clk = &sdrc_ick,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_SDRC_SHIFT,
	.flags		= ENABLE_ON_INIT,
	.clkdm_name	= "core_l3_clkdm",
};

DEFINE_STRUCT_CLK(sdrc_ick, sdrc_ick_parent_names, aes2_ick_ops);

static const struct clksel_rate sgx_core_rates[] = {
	{ .div = 2, .val = 5, .flags = RATE_IN_36XX },
	{ .div = 3, .val = 0, .flags = RATE_IN_3XXX },
	{ .div = 4, .val = 1, .flags = RATE_IN_3XXX },
	{ .div = 6, .val = 2, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel_rate sgx_96m_rates[] = {
	{ .div = 1, .val = 3, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel_rate sgx_192m_rates[] = {
	{ .div = 1, .val = 4, .flags = RATE_IN_36XX },
	{ .div = 0 }
};

static const struct clksel_rate sgx_corex2_rates[] = {
	{ .div = 3, .val = 6, .flags = RATE_IN_36XX },
	{ .div = 5, .val = 7, .flags = RATE_IN_36XX },
	{ .div = 0 }
};

static const struct clksel sgx_clksel[] = {
	{ .parent = &core_ck, .rates = sgx_core_rates },
	{ .parent = &cm_96m_fck, .rates = sgx_96m_rates },
	{ .parent = &omap_192m_alwon_fck, .rates = sgx_192m_rates },
	{ .parent = &corex2_fck, .rates = sgx_corex2_rates },
	{ .parent = NULL },
};

static const char *sgx_fck_parent_names[] = {
	"core_ck",
	"cm_96m_fck",
	"omap_192m_alwon_fck",
	"corex2_fck",
};

static struct clk sgx_fck;

static const struct clk_ops sgx_fck_ops = {
	.init		= &omap2_init_clk_clkdm,
	.enable		= &omap2_dflt_clk_enable,
	.disable	= &omap2_dflt_clk_disable,
	.recalc_rate	= &omap2_clksel_recalc,
	.set_rate	= &omap2_clksel_set_rate,
	.round_rate	= &omap2_clksel_round_rate,
	.get_parent	= &omap2_init_clksel_parent,
	.set_parent	= &omap2_clksel_set_parent,
};

static struct clk_hw_omap sgx_fck_hw = {
	.hw = {
		.clk = &sgx_fck,
	},
	.clksel		= sgx_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(OMAP3430ES2_SGX_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP3430ES2_CLKSEL_SGX_MASK,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430ES2_SGX_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430ES2_CM_FCLKEN_SGX_EN_SGX_SHIFT,
	.clkdm_name	= "sgx_clkdm",
};

DEFINE_STRUCT_CLK(sgx_fck, sgx_fck_parent_names, sgx_fck_ops);

static struct clk sgx_ick;

static const char *sgx_ick_parent_names[] = {
	"l3_ick",
};

static struct clk_hw_omap sgx_ick_hw = {
	.hw = {
		.clk = &sgx_ick,
	},
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430ES2_SGX_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430ES2_CM_ICLKEN_SGX_EN_SGX_SHIFT,
	.clkdm_name	= "sgx_clkdm",
};

DEFINE_STRUCT_CLK(sgx_ick, sgx_ick_parent_names, aes2_ick_ops);

static struct clk sha11_ick;

static const char *sha11_ick_parent_names[] = {
	"security_l4_ick2",
};

static struct clk_hw_omap sha11_ick_hw = {
	.hw = {
		.clk = &sha11_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN2),
	.enable_bit	= OMAP3430_EN_SHA11_SHIFT,
};

DEFINE_STRUCT_CLK(sha11_ick, sha11_ick_parent_names, aes1_ick_ops);

static struct clk sha12_ick;

static const char *sha12_ick_parent_names[] = {
	"core_l4_ick",
};

static struct clk_hw_omap sha12_ick_hw = {
	.hw = {
		.clk = &sha12_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_SHA12_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(sha12_ick, sha12_ick_parent_names, aes2_ick_ops);

static struct clk sr1_fck;

static const char *sr1_fck_parent_names[] = {
	"sys_ck",
};

static struct clk_hw_omap sr1_fck_hw = {
	.hw = {
		.clk = &sr1_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(WKUP_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_SR1_SHIFT,
	.clkdm_name	= "wkup_clkdm",
};

DEFINE_STRUCT_CLK(sr1_fck, sr1_fck_parent_names, aes2_ick_ops);

static struct clk sr2_fck;

static const char *sr2_fck_parent_names[] = {
	"sys_ck",
};

static struct clk_hw_omap sr2_fck_hw = {
	.hw = {
		.clk = &sr2_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(WKUP_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_SR2_SHIFT,
	.clkdm_name	= "wkup_clkdm",
};

DEFINE_STRUCT_CLK(sr2_fck, sr2_fck_parent_names, aes2_ick_ops);

static struct clk sr_l4_ick;

static const char *sr_l4_ick_parent_names[] = {
	"l4_ick",
};

static struct clk_hw_omap sr_l4_ick_hw = {
	.hw = {
		.clk = &sr_l4_ick,
	},
};

DEFINE_STRUCT_CLK(sr_l4_ick, sr_l4_ick_parent_names, core_ck_ops);

static struct clk ssi_l4_ick;

static const char *ssi_l4_ick_parent_names[] = {
	"l4_ick",
};

static struct clk_hw_omap ssi_l4_ick_hw = {
	.hw = {
		.clk = &ssi_l4_ick,
	},
};

DEFINE_STRUCT_CLK(ssi_l4_ick, ssi_l4_ick_parent_names, core_ck_ops);

static struct clk ssi_ick_3430es1;

static const char *ssi_ick_3430es1_parent_names[] = {
	"ssi_l4_ick",
};

static struct clk_hw_omap ssi_ick_3430es1_hw = {
	.hw = {
		.clk = &ssi_ick_3430es1,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_SSI_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(ssi_ick_3430es1, ssi_ick_3430es1_parent_names, aes2_ick_ops);

static struct clk ssi_ick_3430es2;

static const char *ssi_ick_3430es2_parent_names[] = {
	"ssi_l4_ick",
};

static struct clk_hw_omap ssi_ick_3430es2_hw = {
	.hw = {
		.clk = &ssi_ick_3430es2,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_SSI_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(ssi_ick_3430es2, ssi_ick_3430es2_parent_names, dss1_alwon_fck_3430es2_ops);

static const struct clksel_rate ssi_ssr_corex2_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_3XXX },
	{ .div = 2, .val = 2, .flags = RATE_IN_3XXX },
	{ .div = 3, .val = 3, .flags = RATE_IN_3XXX },
	{ .div = 4, .val = 4, .flags = RATE_IN_3XXX },
	{ .div = 6, .val = 6, .flags = RATE_IN_3XXX },
	{ .div = 8, .val = 8, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel ssi_ssr_clksel[] = {
	{ .parent = &corex2_fck, .rates = ssi_ssr_corex2_rates },
	{ .parent = NULL },
};

static const char *ssi_ssr_fck_3430es1_parent_names[] = {
	"corex2_fck",
};

static struct clk ssi_ssr_fck_3430es1;

static const struct clk_ops ssi_ssr_fck_3430es1_ops = {
	.init		= &omap2_init_clk_clkdm,
	.enable		= &omap2_dflt_clk_enable,
	.disable	= &omap2_dflt_clk_disable,
	.recalc_rate	= &omap2_clksel_recalc,
	.set_rate	= &omap2_clksel_set_rate,
	.round_rate	= &omap2_clksel_round_rate,
};

static struct clk_hw_omap ssi_ssr_fck_3430es1_hw = {
	.hw = {
		.clk = &ssi_ssr_fck_3430es1,
	},
	.clksel		= ssi_ssr_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP3430_CLKSEL_SSI_MASK,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP3430_EN_SSI_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(ssi_ssr_fck_3430es1, ssi_ssr_fck_3430es1_parent_names, ssi_ssr_fck_3430es1_ops);

static struct clk ssi_ssr_fck_3430es2;

static const struct clk_ops ssi_ssr_fck_3430es2_ops = {
	.init		= &omap2_init_clk_clkdm,
	.recalc_rate	= &omap2_clksel_recalc,
	.set_rate	= &omap2_clksel_set_rate,
	.round_rate	= &omap2_clksel_round_rate,
};

static struct clk_hw_omap ssi_ssr_fck_3430es2_hw = {
	.hw = {
		.clk = &ssi_ssr_fck_3430es2,
	},
	.clksel		= ssi_ssr_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP3430_CLKSEL_SSI_MASK,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP3430_EN_SSI_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(ssi_ssr_fck_3430es2, ssi_ssr_fck_3430es1_parent_names, ssi_ssr_fck_3430es2_ops);

static struct clk ssi_sst_fck_3430es1;

static const char *ssi_sst_fck_3430es1_parent_names[] = {
	"ssi_ssr_fck_3430es1",
};

static struct clk_hw_omap ssi_sst_fck_3430es1_hw = {
	.hw = {
		.clk = &ssi_sst_fck_3430es1,
	},
	.fixed_div	= 2,
};

DEFINE_STRUCT_CLK(ssi_sst_fck_3430es1, ssi_sst_fck_3430es1_parent_names, omap_12m_fck_ops);

static struct clk ssi_sst_fck_3430es2;

static const char *ssi_sst_fck_3430es2_parent_names[] = {
	"ssi_ssr_fck_3430es2",
};

static struct clk_hw_omap ssi_sst_fck_3430es2_hw = {
	.hw = {
		.clk = &ssi_sst_fck_3430es2,
	},
	.fixed_div	= 2,
};

DEFINE_STRUCT_CLK(ssi_sst_fck_3430es2, ssi_sst_fck_3430es2_parent_names, omap_12m_fck_ops);

static struct clk sys_clkout1;

static const char *sys_clkout1_parent_names[] = {
	"osc_sys_ck",
};

static struct clk_hw_omap sys_clkout1_hw = {
	.hw = {
		.clk = &sys_clkout1,
	},
	.enable_reg	= OMAP3430_PRM_CLKOUT_CTRL,
	.enable_bit	= OMAP3430_CLKOUT_EN_SHIFT,
};

DEFINE_STRUCT_CLK(sys_clkout1, sys_clkout1_parent_names, aes1_ick_ops);

static const struct clksel_rate sys_clkout2_rates[] = {
	{ .div = 1, .val = 0, .flags = RATE_IN_3XXX },
	{ .div = 2, .val = 1, .flags = RATE_IN_3XXX },
	{ .div = 4, .val = 2, .flags = RATE_IN_3XXX },
	{ .div = 8, .val = 3, .flags = RATE_IN_3XXX },
	{ .div = 16, .val = 4, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel sys_clkout2_clksel[] = {
	{ .parent = &clkout2_src_ck, .rates = sys_clkout2_rates },
	{ .parent = NULL },
};

static const char *sys_clkout2_parent_names[] = {
	"clkout2_src_ck",
};

static struct clk sys_clkout2;

static struct clk_hw_omap sys_clkout2_hw = {
	.hw = {
		.clk = &sys_clkout2,
	},
	.clksel		= sys_clkout2_clksel,
	.clksel_reg	= OMAP3430_CM_CLKOUT_CTRL,
	.clksel_mask	= OMAP3430_CLKOUT2_DIV_MASK,
};

DEFINE_STRUCT_CLK(sys_clkout2, sys_clkout2_parent_names, sys_ck_ops);

static const struct clksel_rate traceclk_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_3XXX },
	{ .div = 2, .val = 2, .flags = RATE_IN_3XXX },
	{ .div = 4, .val = 4, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static struct clk traceclk_src_fck;

static struct clk_hw_omap traceclk_src_fck_hw = {
	.hw = {
		.clk = &traceclk_src_fck,
	},
	.clksel		= emu_src_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(OMAP3430_EMU_MOD, CM_CLKSEL1),
	.clksel_mask	= OMAP3430_TRACE_MUX_CTRL_MASK,
};

DEFINE_STRUCT_CLK(traceclk_src_fck, emu_src_ck_parent_names, osc_sys_ck_ops);

static const struct clksel traceclk_clksel[] = {
	{ .parent = &traceclk_src_fck, .rates = traceclk_rates },
	{ .parent = NULL },
};

static const char *traceclk_fck_parent_names[] = {
	"traceclk_src_fck",
};

static struct clk traceclk_fck;

static struct clk_hw_omap traceclk_fck_hw = {
	.hw = {
		.clk = &traceclk_fck,
	},
	.clksel		= traceclk_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(OMAP3430_EMU_MOD, CM_CLKSEL1),
	.clksel_mask	= OMAP3430_CLKSEL_TRACECLK_MASK,
};

DEFINE_STRUCT_CLK(traceclk_fck, traceclk_fck_parent_names, sys_ck_ops);

static struct clk ts_fck;

static const char *ts_fck_parent_names[] = {
	"omap_32k_fck",
};

static struct clk_hw_omap ts_fck_hw = {
	.hw = {
		.clk = &ts_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, OMAP3430ES2_CM_FCLKEN3),
	.enable_bit	= OMAP3430ES2_EN_TS_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(ts_fck, ts_fck_parent_names, aes2_ick_ops);

static struct clk uart1_fck;

static const char *uart1_fck_parent_names[] = {
	"core_48m_fck",
};

static struct clk_hw_omap uart1_fck_hw = {
	.hw = {
		.clk = &uart1_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP3430_EN_UART1_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(uart1_fck, uart1_fck_parent_names, aes2_ick_ops);

static struct clk uart1_ick;

static const char *uart1_ick_parent_names[] = {
	"core_l4_ick",
};

static struct clk_hw_omap uart1_ick_hw = {
	.hw = {
		.clk = &uart1_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_UART1_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(uart1_ick, uart1_ick_parent_names, aes2_ick_ops);

static struct clk uart2_fck;

static const char *uart2_fck_parent_names[] = {
	"core_48m_fck",
};

static struct clk_hw_omap uart2_fck_hw = {
	.hw = {
		.clk = &uart2_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP3430_EN_UART2_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(uart2_fck, uart2_fck_parent_names, aes2_ick_ops);

static struct clk uart2_ick;

static const char *uart2_ick_parent_names[] = {
	"core_l4_ick",
};

static struct clk_hw_omap uart2_ick_hw = {
	.hw = {
		.clk = &uart2_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430_EN_UART2_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(uart2_ick, uart2_ick_parent_names, aes2_ick_ops);

static struct clk uart3_fck;

static const char *uart3_fck_parent_names[] = {
	"per_48m_fck",
};

static struct clk_hw_omap uart3_fck_hw = {
	.hw = {
		.clk = &uart3_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_UART3_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(uart3_fck, uart3_fck_parent_names, aes2_ick_ops);

static struct clk uart3_ick;

static const char *uart3_ick_parent_names[] = {
	"per_l4_ick",
};

static struct clk_hw_omap uart3_ick_hw = {
	.hw = {
		.clk = &uart3_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430_EN_UART3_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(uart3_ick, uart3_ick_parent_names, aes2_ick_ops);

static struct clk uart4_fck;

static const char *uart4_fck_parent_names[] = {
	"per_48m_fck",
};

static struct clk_hw_omap uart4_fck_hw = {
	.hw = {
		.clk = &uart4_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3630_EN_UART4_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(uart4_fck, uart4_fck_parent_names, aes2_ick_ops);

static struct clk uart4_fck_am35xx;

static const char *uart4_fck_am35xx_parent_names[] = {
	"per_48m_fck",
};

static struct clk_hw_omap uart4_fck_am35xx_hw = {
	.hw = {
		.clk = &uart4_fck_am35xx,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP3430_EN_UART4_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(uart4_fck_am35xx, uart4_fck_am35xx_parent_names, aes2_ick_ops);

static struct clk uart4_ick;

static const char *uart4_ick_parent_names[] = {
	"per_l4_ick",
};

static struct clk_hw_omap uart4_ick_hw = {
	.hw = {
		.clk = &uart4_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3630_EN_UART4_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(uart4_ick, uart4_ick_parent_names, aes2_ick_ops);

static struct clk uart4_ick_am35xx;

static const char *uart4_ick_am35xx_parent_names[] = {
	"core_l4_ick",
};

static struct clk_hw_omap uart4_ick_am35xx_hw = {
	.hw = {
		.clk = &uart4_ick_am35xx,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= AM35XX_EN_UART4_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(uart4_ick_am35xx, uart4_ick_am35xx_parent_names, aes2_ick_ops);

static const struct clksel usb_l4_clksel[] = {
	{ .parent = &l4_ick, .rates = div2_rates },
	{ .parent = NULL },
};

static const char *usb_l4_ick_parent_names[] = {
	"l4_ick",
};

static struct clk usb_l4_ick;

static struct clk_hw_omap usb_l4_ick_hw = {
	.hw = {
		.clk = &usb_l4_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.clksel		= usb_l4_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP3430ES1_CLKSEL_FSHOSTUSB_MASK,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP3430ES1_EN_FSHOSTUSB_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(usb_l4_ick, usb_l4_ick_parent_names, ssi_ssr_fck_3430es1_ops);

static struct clk usbhost_120m_fck;

static const char *usbhost_120m_fck_parent_names[] = {
	"dpll5_m2_ck",
};

static struct clk_hw_omap usbhost_120m_fck_hw = {
	.hw = {
		.clk = &usbhost_120m_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430ES2_USBHOST_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430ES2_EN_USBHOST2_SHIFT,
	.clkdm_name	= "usbhost_clkdm",
};

DEFINE_STRUCT_CLK(usbhost_120m_fck, usbhost_120m_fck_parent_names, aes2_ick_ops);

static struct clk usbhost_48m_fck;

static const char *usbhost_48m_fck_parent_names[] = {
	"omap_48m_fck",
};

static struct clk_hw_omap usbhost_48m_fck_hw = {
	.hw = {
		.clk = &usbhost_48m_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430ES2_USBHOST_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430ES2_EN_USBHOST1_SHIFT,
	.clkdm_name	= "usbhost_clkdm",
};

DEFINE_STRUCT_CLK(usbhost_48m_fck, usbhost_48m_fck_parent_names, dss1_alwon_fck_3430es2_ops);

static struct clk usbhost_ick;

static const char *usbhost_ick_parent_names[] = {
	"l4_ick",
};

static struct clk_hw_omap usbhost_ick_hw = {
	.hw = {
		.clk = &usbhost_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430ES2_USBHOST_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430ES2_EN_USBHOST_SHIFT,
	.clkdm_name	= "usbhost_clkdm",
};

DEFINE_STRUCT_CLK(usbhost_ick, usbhost_ick_parent_names, dss1_alwon_fck_3430es2_ops);

static struct clk usbtll_fck;

static const char *usbtll_fck_parent_names[] = {
	"dpll5_m2_ck",
};

static struct clk_hw_omap usbtll_fck_hw = {
	.hw = {
		.clk = &usbtll_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, OMAP3430ES2_CM_FCLKEN3),
	.enable_bit	= OMAP3430ES2_EN_USBTLL_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(usbtll_fck, usbtll_fck_parent_names, aes2_ick_ops);

static struct clk usbtll_ick;

static const char *usbtll_ick_parent_names[] = {
	"core_l4_ick",
};

static struct clk_hw_omap usbtll_ick_hw = {
	.hw = {
		.clk = &usbtll_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN3),
	.enable_bit	= OMAP3430ES2_EN_USBTLL_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(usbtll_ick, usbtll_ick_parent_names, aes2_ick_ops);

static const struct clksel_rate usim_96m_rates[] = {
	{ .div = 2, .val = 3, .flags = RATE_IN_3XXX },
	{ .div = 4, .val = 4, .flags = RATE_IN_3XXX },
	{ .div = 8, .val = 5, .flags = RATE_IN_3XXX },
	{ .div = 10, .val = 6, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel_rate usim_120m_rates[] = {
	{ .div = 4, .val = 7, .flags = RATE_IN_3XXX },
	{ .div = 8, .val = 8, .flags = RATE_IN_3XXX },
	{ .div = 16, .val = 9, .flags = RATE_IN_3XXX },
	{ .div = 20, .val = 10, .flags = RATE_IN_3XXX },
	{ .div = 0 }
};

static const struct clksel usim_clksel[] = {
	{ .parent = &omap_96m_fck, .rates = usim_96m_rates },
	{ .parent = &dpll5_m2_ck, .rates = usim_120m_rates },
	{ .parent = &sys_ck, .rates = div2_rates },
	{ .parent = NULL },
};

static const char *usim_fck_parent_names[] = {
	"omap_96m_fck",
	"dpll5_m2_ck",
	"sys_ck",
};

static struct clk usim_fck;

static const struct clk_ops usim_fck_ops = {
	.enable		= &omap2_dflt_clk_enable,
	.disable	= &omap2_dflt_clk_disable,
	.recalc_rate	= &omap2_clksel_recalc,
	.get_parent	= &omap2_init_clksel_parent,
	.set_parent	= &omap2_clksel_set_parent,
};

static struct clk_hw_omap usim_fck_hw = {
	.hw = {
		.clk = &usim_fck,
	},
	.clksel		= usim_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(WKUP_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP3430ES2_CLKSEL_USIMOCP_MASK,
	.enable_reg	= OMAP_CM_REGADDR(WKUP_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430ES2_EN_USIMOCP_SHIFT,
};

DEFINE_STRUCT_CLK(usim_fck, usim_fck_parent_names, usim_fck_ops);

static struct clk usim_ick;

static const char *usim_ick_parent_names[] = {
	"wkup_l4_ick",
};

static struct clk_hw_omap usim_ick_hw = {
	.hw = {
		.clk = &usim_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(WKUP_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430ES2_EN_USIMOCP_SHIFT,
	.clkdm_name	= "wkup_clkdm",
};

DEFINE_STRUCT_CLK(usim_ick, usim_ick_parent_names, aes2_ick_ops);

static struct clk vpfe_fck;

static const char *vpfe_fck_parent_names[] = {
	"pclk_ck",
};

static struct clk_hw_omap vpfe_fck_hw = {
	.hw = {
		.clk = &vpfe_fck,
	},
	.enable_reg	= OMAP343X_CTRL_REGADDR(AM35XX_CONTROL_IPSS_CLK_CTRL),
	.enable_bit	= AM35XX_VPFE_FCLK_SHIFT,
};

DEFINE_STRUCT_CLK(vpfe_fck, vpfe_fck_parent_names, aes1_ick_ops);

static struct clk vpfe_ick;

static const char *vpfe_ick_parent_names[] = {
	"ipss_ick",
};

static struct clk_hw_omap vpfe_ick_hw = {
	.hw = {
		.clk = &vpfe_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP343X_CTRL_REGADDR(AM35XX_CONTROL_IPSS_CLK_CTRL),
	.enable_bit	= AM35XX_VPFE_VBUSP_CLK_SHIFT,
	.clkdm_name	= "core_l3_clkdm",
};

DEFINE_STRUCT_CLK(vpfe_ick, vpfe_ick_parent_names, dss1_alwon_fck_3430es2_ops);

static struct clk wdt1_fck;

static const char *wdt1_fck_parent_names[] = {
	"secure_32k_fck",
};

static struct clk_hw_omap wdt1_fck_hw = {
	.hw = {
		.clk = &wdt1_fck,
	},
};

DEFINE_STRUCT_CLK(wdt1_fck, wdt1_fck_parent_names, core_ck_ops);

static struct clk wdt1_ick;

static const char *wdt1_ick_parent_names[] = {
	"wkup_l4_ick",
};

static struct clk_hw_omap wdt1_ick_hw = {
	.hw = {
		.clk = &wdt1_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(WKUP_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430_EN_WDT1_SHIFT,
	.clkdm_name	= "wkup_clkdm",
};

DEFINE_STRUCT_CLK(wdt1_ick, wdt1_ick_parent_names, aes2_ick_ops);

static struct clk wdt2_fck;

static const char *wdt2_fck_parent_names[] = {
	"wkup_32k_fck",
};

static struct clk_hw_omap wdt2_fck_hw = {
	.hw = {
		.clk = &wdt2_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(WKUP_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_WDT2_SHIFT,
	.clkdm_name	= "wkup_clkdm",
};

DEFINE_STRUCT_CLK(wdt2_fck, wdt2_fck_parent_names, aes2_ick_ops);

static struct clk wdt2_ick;

static const char *wdt2_ick_parent_names[] = {
	"wkup_l4_ick",
};

static struct clk_hw_omap wdt2_ick_hw = {
	.hw = {
		.clk = &wdt2_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(WKUP_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430_EN_WDT2_SHIFT,
	.clkdm_name	= "wkup_clkdm",
};

DEFINE_STRUCT_CLK(wdt2_ick, wdt2_ick_parent_names, aes2_ick_ops);

static struct clk wdt3_fck;

static const char *wdt3_fck_parent_names[] = {
	"per_32k_alwon_fck",
};

static struct clk_hw_omap wdt3_fck_hw = {
	.hw = {
		.clk = &wdt3_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_FCLKEN),
	.enable_bit	= OMAP3430_EN_WDT3_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(wdt3_fck, wdt3_fck_parent_names, aes2_ick_ops);

static struct clk wdt3_ick;

static const char *wdt3_ick_parent_names[] = {
	"per_l4_ick",
};

static struct clk_hw_omap wdt3_ick_hw = {
	.hw = {
		.clk = &wdt3_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(OMAP3430_PER_MOD, CM_ICLKEN),
	.enable_bit	= OMAP3430_EN_WDT3_SHIFT,
	.clkdm_name	= "per_clkdm",
};

DEFINE_STRUCT_CLK(wdt3_ick, wdt3_ick_parent_names, aes2_ick_ops);

