DEFINE_CLK_FIXED_RATE(alt_ck, CLK_IS_ROOT, 54000000, 0x0);

DEFINE_CLK_FIXED_RATE(func_32k_ck, CLK_IS_ROOT, 32768, 0x0);

DEFINE_CLK_FIXED_RATE(mcbsp_clks, CLK_IS_ROOT, 0x0, 0x0);

DEFINE_CLK_FIXED_RATE(osc_ck, CLK_IS_ROOT, 0x0, 0x0);

DEFINE_CLK_FIXED_RATE(secure_32k_ck, CLK_IS_ROOT, 32768, 0x0);

static struct clk sys_ck;

static const char *sys_ck_parent_names[] = {
	"osc_ck",
};

static const struct clk_ops sys_ck_ops = {
	.recalc_rate	= &omap2xxx_sys_clk_recalc,
};

static struct clk_hw_omap sys_ck_hw = {
	.hw = {
		.clk = &sys_ck,
	},
};

DEFINE_STRUCT_CLK(sys_ck, sys_ck_parent_names, sys_ck_ops);

static struct dpll_data dpll_dd = {
	.mult_div1_reg	= OMAP_CM_REGADDR(PLL_MOD, CM_CLKSEL1),
	.mult_mask	= OMAP24XX_DPLL_MULT_MASK,
	.div1_mask	= OMAP24XX_DPLL_DIV_MASK,
	.clk_bypass	= &sys_ck,
	.clk_ref	= &sys_ck,
	.control_reg	= OMAP_CM_REGADDR(PLL_MOD, CM_CLKEN),
	.enable_mask	= OMAP24XX_EN_DPLL_MASK,
	.max_multiplier	= 1023,
	.min_divider	= 1,
	.max_divider	= 16,
};

static struct clk dpll_ck;

static const char *dpll_ck_parent_names[] = {
	"sys_ck",
};

static const struct clk_ops dpll_ck_ops = {
	.init		= &omap2_init_clk_clkdm,
	.enable		= &omap3_noncore_dpll_enable,
	.disable	= &omap3_noncore_dpll_disable,
	.set_rate	= &omap3_noncore_dpll_set_rate,
	.get_parent	= &omap2_init_dpll_parent,
	.recalc_rate	= &omap2_dpllcore_recalc,
	.set_rate	= &omap2_reprogram_dpllcore,
};

static struct clk_hw_omap dpll_ck_hw = {
	.hw = {
		.clk = &dpll_ck,
	},
	.dpll_data	= &dpll_dd,
	.clkdm_name	= "wkup_clkdm",
};

DEFINE_STRUCT_CLK(dpll_ck, dpll_ck_parent_names, dpll_ck_ops);

static struct clk core_ck;

static const char *core_ck_parent_names[] = {
	"dpll_ck",
};

static const struct clk_ops core_ck_ops = {
};

static struct clk_hw_omap core_ck_hw = {
	.hw = {
		.clk = &core_ck,
	},
};

DEFINE_STRUCT_CLK(core_ck, core_ck_parent_names, core_ck_ops);

static const struct clksel_rate core_l3_core_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_24XX },
	{ .div = 2, .val = 2, .flags = RATE_IN_242X },
	{ .div = 4, .val = 4, .flags = RATE_IN_24XX },
	{ .div = 6, .val = 6, .flags = RATE_IN_24XX },
	{ .div = 8, .val = 8, .flags = RATE_IN_242X },
	{ .div = 12, .val = 12, .flags = RATE_IN_242X },
	{ .div = 16, .val = 16, .flags = RATE_IN_242X },
	{ .div = 0 }
};

static const struct clksel core_l3_clksel[] = {
	{ .parent = &core_ck, .rates = core_l3_core_rates },
	{ .parent = NULL },
};

static const char *core_l3_ck_parent_names[] = {
	"core_ck",
};

static struct clk core_l3_ck;

static const struct clk_ops core_l3_ck_ops = {
	.recalc_rate	= &omap2_clksel_recalc,
	.set_rate	= &omap2_clksel_set_rate,
	.round_rate	= &omap2_clksel_round_rate,
};

static struct clk_hw_omap core_l3_ck_hw = {
	.hw = {
		.clk = &core_l3_ck,
	},
	.clksel		= core_l3_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_CLKSEL1),
	.clksel_mask	= OMAP24XX_CLKSEL_L3_MASK,
};

DEFINE_STRUCT_CLK(core_l3_ck, core_l3_ck_parent_names, core_l3_ck_ops);

static const struct clksel_rate l4_core_l3_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_24XX },
	{ .div = 2, .val = 2, .flags = RATE_IN_24XX },
	{ .div = 0 }
};

static const struct clksel l4_clksel[] = {
	{ .parent = &core_l3_ck, .rates = l4_core_l3_rates },
	{ .parent = NULL },
};

static const char *l4_ck_parent_names[] = {
	"core_l3_ck",
};

static struct clk l4_ck;

static struct clk_hw_omap l4_ck_hw = {
	.hw = {
		.clk = &l4_ck,
	},
	.clksel		= l4_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_CLKSEL1),
	.clksel_mask	= OMAP24XX_CLKSEL_L4_MASK,
};

DEFINE_STRUCT_CLK(l4_ck, l4_ck_parent_names, core_l3_ck_ops);

static struct clk aes_ick;

static const char *aes_ick_parent_names[] = {
	"l4_ck",
};

static const struct clk_ops aes_ick_ops = {
	.init		= &omap2_init_clk_clkdm,
	.enable		= &omap2_dflt_clk_enable,
	.disable	= &omap2_dflt_clk_disable,
};

static struct clk_hw_omap aes_ick_hw = {
	.hw = {
		.clk = &aes_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, OMAP24XX_CM_ICLKEN4),
	.enable_bit	= OMAP24XX_EN_AES_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(aes_ick, aes_ick_parent_names, aes_ick_ops);

static struct clk apll54_ck;

static const char *apll54_ck_parent_names[] = {
	"sys_ck",
};

static const struct clk_ops apll54_ck_ops = {
	.init		= &omap2_init_clk_clkdm,
};

static struct clk_hw_omap apll54_ck_hw = {
	.hw = {
		.clk = &apll54_ck,
	},
	.enable_reg	= OMAP_CM_REGADDR(PLL_MOD, CM_CLKEN),
	.enable_bit	= OMAP24XX_EN_54M_PLL_SHIFT,
	.flags		= ENABLE_ON_INIT,
	.clkdm_name	= "wkup_clkdm",
};

DEFINE_STRUCT_CLK(apll54_ck, apll54_ck_parent_names, apll54_ck_ops);

static struct clk apll96_ck;

static const char *apll96_ck_parent_names[] = {
	"sys_ck",
};

static struct clk_hw_omap apll96_ck_hw = {
	.hw = {
		.clk = &apll96_ck,
	},
	.enable_reg	= OMAP_CM_REGADDR(PLL_MOD, CM_CLKEN),
	.enable_bit	= OMAP24XX_EN_96M_PLL_SHIFT,
	.flags		= ENABLE_ON_INIT,
	.clkdm_name	= "wkup_clkdm",
};

DEFINE_STRUCT_CLK(apll96_ck, apll96_ck_parent_names, apll54_ck_ops);

static struct clk func_96m_ck;

static const char *func_96m_ck_parent_names[] = {
	"apll96_ck",
};

static struct clk_hw_omap func_96m_ck_hw = {
	.hw = {
		.clk = &func_96m_ck,
	},
};

DEFINE_STRUCT_CLK(func_96m_ck, func_96m_ck_parent_names, core_ck_ops);

static struct clk cam_fck;

static const char *cam_fck_parent_names[] = {
	"func_96m_ck",
};

static struct clk_hw_omap cam_fck_hw = {
	.hw = {
		.clk = &cam_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP24XX_EN_CAM_SHIFT,
	.clkdm_name	= "core_l3_clkdm",
};

DEFINE_STRUCT_CLK(cam_fck, cam_fck_parent_names, aes_ick_ops);

static struct clk cam_ick;

static const char *cam_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap cam_ick_hw = {
	.hw = {
		.clk = &cam_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP24XX_EN_CAM_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(cam_ick, cam_ick_parent_names, aes_ick_ops);

static struct clk des_ick;

static const char *des_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap des_ick_hw = {
	.hw = {
		.clk = &des_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, OMAP24XX_CM_ICLKEN4),
	.enable_bit	= OMAP24XX_EN_DES_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(des_ick, des_ick_parent_names, aes_ick_ops);

static const struct clksel_rate dsp_fck_core_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_24XX },
	{ .div = 2, .val = 2, .flags = RATE_IN_24XX },
	{ .div = 3, .val = 3, .flags = RATE_IN_24XX },
	{ .div = 4, .val = 4, .flags = RATE_IN_24XX },
	{ .div = 6, .val = 6, .flags = RATE_IN_242X },
	{ .div = 8, .val = 8, .flags = RATE_IN_242X },
	{ .div = 12, .val = 12, .flags = RATE_IN_242X },
	{ .div = 0 }
};

static const struct clksel dsp_fck_clksel[] = {
	{ .parent = &core_ck, .rates = dsp_fck_core_rates },
	{ .parent = NULL },
};

static const char *dsp_fck_parent_names[] = {
	"core_ck",
};

static struct clk dsp_fck;

static const struct clk_ops dsp_fck_ops = {
	.init		= &omap2_init_clk_clkdm,
	.enable		= &omap2_dflt_clk_enable,
	.disable	= &omap2_dflt_clk_disable,
	.recalc_rate	= &omap2_clksel_recalc,
	.set_rate	= &omap2_clksel_set_rate,
	.round_rate	= &omap2_clksel_round_rate,
};

static struct clk_hw_omap dsp_fck_hw = {
	.hw = {
		.clk = &dsp_fck,
	},
	.clksel		= dsp_fck_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(OMAP24XX_DSP_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP24XX_CLKSEL_DSP_MASK,
	.enable_reg	= OMAP_CM_REGADDR(OMAP24XX_DSP_MOD, CM_FCLKEN),
	.enable_bit	= OMAP24XX_CM_FCLKEN_DSP_EN_DSP_SHIFT,
	.clkdm_name	= "dsp_clkdm",
};

DEFINE_STRUCT_CLK(dsp_fck, dsp_fck_parent_names, dsp_fck_ops);

static const struct clksel dsp_ick_clksel[] = {
	{ .parent = &dsp_fck, .rates = dsp_ick_rates },
	{ .parent = NULL },
};

static const char *dsp_ick_parent_names[] = {
	"dsp_fck",
};

static struct clk dsp_ick;

static struct clk_hw_omap dsp_ick_hw = {
	.hw = {
		.clk = &dsp_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.clksel		= dsp_ick_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(OMAP24XX_DSP_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP24XX_CLKSEL_DSP_IF_MASK,
	.enable_reg	= OMAP_CM_REGADDR(OMAP24XX_DSP_MOD, CM_ICLKEN),
	.enable_bit	= OMAP2420_EN_DSP_IPI_SHIFT,
	.clkdm_name	= "dsp_clkdm",
};

DEFINE_STRUCT_CLK(dsp_ick, dsp_ick_parent_names, dsp_fck_ops);

static const struct clksel_rate dss1_fck_sys_rates[] = {
	{ .div = 1, .val = 0, .flags = RATE_IN_24XX },
	{ .div = 0 }
};

static const struct clksel_rate dss1_fck_core_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_24XX },
	{ .div = 2, .val = 2, .flags = RATE_IN_24XX },
	{ .div = 3, .val = 3, .flags = RATE_IN_24XX },
	{ .div = 4, .val = 4, .flags = RATE_IN_24XX },
	{ .div = 5, .val = 5, .flags = RATE_IN_24XX },
	{ .div = 6, .val = 6, .flags = RATE_IN_24XX },
	{ .div = 8, .val = 8, .flags = RATE_IN_24XX },
	{ .div = 9, .val = 9, .flags = RATE_IN_24XX },
	{ .div = 12, .val = 12, .flags = RATE_IN_24XX },
	{ .div = 16, .val = 16, .flags = RATE_IN_24XX },
	{ .div = 0 }
};

static const struct clksel dss1_fck_clksel[] = {
	{ .parent = &sys_ck, .rates = dss1_fck_sys_rates },
	{ .parent = &core_ck, .rates = dss1_fck_core_rates },
	{ .parent = NULL },
};

static const char *dss1_fck_parent_names[] = {
	"sys_ck",
	"core_ck",
};

static struct clk dss1_fck;

static const struct clk_ops dss1_fck_ops = {
	.init		= &omap2_init_clk_clkdm,
	.enable		= &omap2_dflt_clk_enable,
	.disable	= &omap2_dflt_clk_disable,
	.recalc_rate	= &omap2_clksel_recalc,
	.get_parent	= &omap2_init_clksel_parent,
	.set_parent	= &omap2_clksel_set_parent,
};

static struct clk_hw_omap dss1_fck_hw = {
	.hw = {
		.clk = &dss1_fck,
	},
	.clksel		= dss1_fck_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_CLKSEL1),
	.clksel_mask	= OMAP24XX_CLKSEL_DSS1_MASK,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP24XX_EN_DSS1_SHIFT,
	.clkdm_name	= "dss_clkdm",
};

DEFINE_STRUCT_CLK(dss1_fck, dss1_fck_parent_names, dss1_fck_ops);

static const struct clksel_rate dss2_fck_sys_rates[] = {
	{ .div = 1, .val = 0, .flags = RATE_IN_24XX },
	{ .div = 0 }
};

static const struct clksel_rate dss2_fck_48m_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_24XX },
	{ .div = 0 }
};

static const struct clksel_rate func_48m_apll96_rates[] = {
	{ .div = 2, .val = 0, .flags = RATE_IN_24XX },
	{ .div = 0 }
};

static const struct clksel_rate func_48m_alt_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_24XX },
	{ .div = 0 }
};

static const struct clksel func_48m_clksel[] = {
	{ .parent = &apll96_ck, .rates = func_48m_apll96_rates },
	{ .parent = &alt_ck, .rates = func_48m_alt_rates },
	{ .parent = NULL },
};

static const char *func_48m_ck_parent_names[] = {
	"apll96_ck",
	"alt_ck",
};

static struct clk func_48m_ck;

static const struct clk_ops func_48m_ck_ops = {
	.recalc_rate	= &omap2_clksel_recalc,
	.set_rate	= &omap2_clksel_set_rate,
	.round_rate	= &omap2_clksel_round_rate,
	.get_parent	= &omap2_init_clksel_parent,
	.set_parent	= &omap2_clksel_set_parent,
};

static struct clk_hw_omap func_48m_ck_hw = {
	.hw = {
		.clk = &func_48m_ck,
	},
	.clksel		= func_48m_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(PLL_MOD, CM_CLKSEL1),
	.clksel_mask	= OMAP24XX_48M_SOURCE_MASK,
};

DEFINE_STRUCT_CLK(func_48m_ck, func_48m_ck_parent_names, func_48m_ck_ops);

static const struct clksel dss2_fck_clksel[] = {
	{ .parent = &sys_ck, .rates = dss2_fck_sys_rates },
	{ .parent = &func_48m_ck, .rates = dss2_fck_48m_rates },
	{ .parent = NULL },
};

static const char *dss2_fck_parent_names[] = {
	"sys_ck",
	"func_48m_ck",
};

static struct clk dss2_fck;

static struct clk_hw_omap dss2_fck_hw = {
	.hw = {
		.clk = &dss2_fck,
	},
	.clksel		= dss2_fck_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_CLKSEL1),
	.clksel_mask	= OMAP24XX_CLKSEL_DSS2_MASK,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP24XX_EN_DSS2_SHIFT,
	.clkdm_name	= "dss_clkdm",
};

DEFINE_STRUCT_CLK(dss2_fck, dss2_fck_parent_names, dss1_fck_ops);

static const struct clksel_rate func_54m_apll54_rates[] = {
	{ .div = 1, .val = 0, .flags = RATE_IN_24XX },
	{ .div = 0 }
};

static const struct clksel_rate func_54m_alt_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_24XX },
	{ .div = 0 }
};

static const struct clksel func_54m_clksel[] = {
	{ .parent = &apll54_ck, .rates = func_54m_apll54_rates },
	{ .parent = &alt_ck, .rates = func_54m_alt_rates },
	{ .parent = NULL },
};

static const char *func_54m_ck_parent_names[] = {
	"apll54_ck",
	"alt_ck",
};

static struct clk func_54m_ck;

static const struct clk_ops func_54m_ck_ops = {
	.recalc_rate	= &omap2_clksel_recalc,
	.get_parent	= &omap2_init_clksel_parent,
	.set_parent	= &omap2_clksel_set_parent,
};

static struct clk_hw_omap func_54m_ck_hw = {
	.hw = {
		.clk = &func_54m_ck,
	},
	.clksel		= func_54m_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(PLL_MOD, CM_CLKSEL1),
	.clksel_mask	= OMAP24XX_54M_SOURCE_MASK,
};

DEFINE_STRUCT_CLK(func_54m_ck, func_54m_ck_parent_names, func_54m_ck_ops);

static struct clk dss_54m_fck;

static const char *dss_54m_fck_parent_names[] = {
	"func_54m_ck",
};

static struct clk_hw_omap dss_54m_fck_hw = {
	.hw = {
		.clk = &dss_54m_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP24XX_EN_TV_SHIFT,
	.clkdm_name	= "dss_clkdm",
};

DEFINE_STRUCT_CLK(dss_54m_fck, dss_54m_fck_parent_names, aes_ick_ops);

static struct clk dss_ick;

static const char *dss_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap dss_ick_hw = {
	.hw = {
		.clk = &dss_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP24XX_EN_DSS1_SHIFT,
	.clkdm_name	= "dss_clkdm",
};

DEFINE_STRUCT_CLK(dss_ick, dss_ick_parent_names, aes_ick_ops);

static struct clk eac_fck;

static const char *eac_fck_parent_names[] = {
	"func_96m_ck",
};

static struct clk_hw_omap eac_fck_hw = {
	.hw = {
		.clk = &eac_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP2420_EN_EAC_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(eac_fck, eac_fck_parent_names, aes_ick_ops);

static struct clk eac_ick;

static const char *eac_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap eac_ick_hw = {
	.hw = {
		.clk = &eac_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP2420_EN_EAC_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(eac_ick, eac_ick_parent_names, aes_ick_ops);

static struct clk emul_ck;

static const char *emul_ck_parent_names[] = {
	"func_54m_ck",
};

static struct clk_hw_omap emul_ck_hw = {
	.hw = {
		.clk = &emul_ck,
	},
	.enable_reg	= OMAP2420_PRCM_CLKEMUL_CTRL,
	.enable_bit	= OMAP24XX_EMULATION_EN_SHIFT,
	.clkdm_name	= "wkup_clkdm",
};

DEFINE_STRUCT_CLK(emul_ck, emul_ck_parent_names, aes_ick_ops);

static struct clk func_12m_ck;

static const char *func_12m_ck_parent_names[] = {
	"func_48m_ck",
};

static const struct clk_ops func_12m_ck_ops = {
	.recalc_rate	= &omap_fixed_divisor_recalc,
};

static struct clk_hw_omap func_12m_ck_hw = {
	.hw = {
		.clk = &func_12m_ck,
	},
	.fixed_div	= 4,
};

DEFINE_STRUCT_CLK(func_12m_ck, func_12m_ck_parent_names, func_12m_ck_ops);

static struct clk fac_fck;

static const char *fac_fck_parent_names[] = {
	"func_12m_ck",
};

static struct clk_hw_omap fac_fck_hw = {
	.hw = {
		.clk = &fac_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP24XX_EN_FAC_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(fac_fck, fac_fck_parent_names, aes_ick_ops);

static struct clk fac_ick;

static const char *fac_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap fac_ick_hw = {
	.hw = {
		.clk = &fac_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP24XX_EN_FAC_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(fac_ick, fac_ick_parent_names, aes_ick_ops);

static const struct clksel gfx_fck_clksel[] = {
	{ .parent = &core_l3_ck, .rates = gfx_l3_rates },
	{ .parent = NULL },
};

static const char *gfx_2d_fck_parent_names[] = {
	"core_l3_ck",
};

static struct clk gfx_2d_fck;

static struct clk_hw_omap gfx_2d_fck_hw = {
	.hw = {
		.clk = &gfx_2d_fck,
	},
	.clksel		= gfx_fck_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(GFX_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP_CLKSEL_GFX_MASK,
	.enable_reg	= OMAP_CM_REGADDR(GFX_MOD, CM_FCLKEN),
	.enable_bit	= OMAP24XX_EN_2D_SHIFT,
	.clkdm_name	= "gfx_clkdm",
};

DEFINE_STRUCT_CLK(gfx_2d_fck, gfx_2d_fck_parent_names, dsp_fck_ops);

static struct clk gfx_3d_fck;

static struct clk_hw_omap gfx_3d_fck_hw = {
	.hw = {
		.clk = &gfx_3d_fck,
	},
	.clksel		= gfx_fck_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(GFX_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP_CLKSEL_GFX_MASK,
	.enable_reg	= OMAP_CM_REGADDR(GFX_MOD, CM_FCLKEN),
	.enable_bit	= OMAP24XX_EN_3D_SHIFT,
	.clkdm_name	= "gfx_clkdm",
};

DEFINE_STRUCT_CLK(gfx_3d_fck, gfx_2d_fck_parent_names, dsp_fck_ops);

static struct clk gfx_ick;

static const char *gfx_ick_parent_names[] = {
	"core_l3_ck",
};

static struct clk_hw_omap gfx_ick_hw = {
	.hw = {
		.clk = &gfx_ick,
	},
	.enable_reg	= OMAP_CM_REGADDR(GFX_MOD, CM_ICLKEN),
	.enable_bit	= OMAP_EN_GFX_SHIFT,
	.clkdm_name	= "gfx_clkdm",
};

DEFINE_STRUCT_CLK(gfx_ick, gfx_ick_parent_names, aes_ick_ops);

static struct clk gpios_fck;

static const char *gpios_fck_parent_names[] = {
	"func_32k_ck",
};

static struct clk_hw_omap gpios_fck_hw = {
	.hw = {
		.clk = &gpios_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(WKUP_MOD, CM_FCLKEN),
	.enable_bit	= OMAP24XX_EN_GPIOS_SHIFT,
	.clkdm_name	= "wkup_clkdm",
};

DEFINE_STRUCT_CLK(gpios_fck, gpios_fck_parent_names, aes_ick_ops);

static struct clk wu_l4_ick;

static const char *wu_l4_ick_parent_names[] = {
	"sys_ck",
};

static struct clk_hw_omap wu_l4_ick_hw = {
	.hw = {
		.clk = &wu_l4_ick,
	},
};

DEFINE_STRUCT_CLK(wu_l4_ick, wu_l4_ick_parent_names, core_ck_ops);

static struct clk gpios_ick;

static const char *gpios_ick_parent_names[] = {
	"wu_l4_ick",
};

static struct clk_hw_omap gpios_ick_hw = {
	.hw = {
		.clk = &gpios_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(WKUP_MOD, CM_ICLKEN),
	.enable_bit	= OMAP24XX_EN_GPIOS_SHIFT,
	.clkdm_name	= "wkup_clkdm",
};

DEFINE_STRUCT_CLK(gpios_ick, gpios_ick_parent_names, aes_ick_ops);

static struct clk gpmc_fck;

static const char *gpmc_fck_parent_names[] = {
	"core_l3_ck",
};

static struct clk_hw_omap gpmc_fck_hw = {
	.hw = {
		.clk = &gpmc_fck,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN3),
	.enable_bit	= OMAP24XX_AUTO_GPMC_SHIFT,
	.flags		= ENABLE_ON_INIT,
	.clkdm_name	= "core_l3_clkdm",
};

DEFINE_STRUCT_CLK(gpmc_fck, gpmc_fck_parent_names, apll54_ck_ops);

static const struct clksel_rate gpt_alt_rates[] = {
	{ .div = 1, .val = 2, .flags = RATE_IN_24XX },
	{ .div = 0 }
};

static const struct clksel omap24xx_gpt_clksel[] = {
	{ .parent = &func_32k_ck, .rates = gpt_32k_rates },
	{ .parent = &sys_ck, .rates = gpt_sys_rates },
	{ .parent = &alt_ck, .rates = gpt_alt_rates },
	{ .parent = NULL },
};

static const char *gpt10_fck_parent_names[] = {
	"func_32k_ck",
	"sys_ck",
	"alt_ck",
};

static struct clk gpt10_fck;

static struct clk_hw_omap gpt10_fck_hw = {
	.hw = {
		.clk = &gpt10_fck,
	},
	.clksel		= omap24xx_gpt_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_CLKSEL2),
	.clksel_mask	= OMAP24XX_CLKSEL_GPT10_MASK,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP24XX_EN_GPT10_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(gpt10_fck, gpt10_fck_parent_names, dss1_fck_ops);

static struct clk gpt10_ick;

static const char *gpt10_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap gpt10_ick_hw = {
	.hw = {
		.clk = &gpt10_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP24XX_EN_GPT10_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(gpt10_ick, gpt10_ick_parent_names, aes_ick_ops);

static struct clk gpt11_fck;

static struct clk_hw_omap gpt11_fck_hw = {
	.hw = {
		.clk = &gpt11_fck,
	},
	.clksel		= omap24xx_gpt_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_CLKSEL2),
	.clksel_mask	= OMAP24XX_CLKSEL_GPT11_MASK,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP24XX_EN_GPT11_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(gpt11_fck, gpt10_fck_parent_names, dss1_fck_ops);

static struct clk gpt11_ick;

static const char *gpt11_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap gpt11_ick_hw = {
	.hw = {
		.clk = &gpt11_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP24XX_EN_GPT11_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(gpt11_ick, gpt11_ick_parent_names, aes_ick_ops);

static struct clk gpt12_fck;

static struct clk_hw_omap gpt12_fck_hw = {
	.hw = {
		.clk = &gpt12_fck,
	},
	.clksel		= omap24xx_gpt_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_CLKSEL2),
	.clksel_mask	= OMAP24XX_CLKSEL_GPT12_MASK,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP24XX_EN_GPT12_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(gpt12_fck, gpt10_fck_parent_names, dss1_fck_ops);

static struct clk gpt12_ick;

static const char *gpt12_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap gpt12_ick_hw = {
	.hw = {
		.clk = &gpt12_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP24XX_EN_GPT12_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(gpt12_ick, gpt12_ick_parent_names, aes_ick_ops);

static struct clk gpt1_fck;

static const struct clk_ops gpt1_fck_ops = {
	.init		= &omap2_init_clk_clkdm,
	.enable		= &omap2_dflt_clk_enable,
	.disable	= &omap2_dflt_clk_disable,
	.recalc_rate	= &omap2_clksel_recalc,
	.set_rate	= &omap2_clksel_set_rate,
	.round_rate	= &omap2_clksel_round_rate,
	.get_parent	= &omap2_init_clksel_parent,
	.set_parent	= &omap2_clksel_set_parent,
};

static struct clk_hw_omap gpt1_fck_hw = {
	.hw = {
		.clk = &gpt1_fck,
	},
	.clksel		= omap24xx_gpt_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(WKUP_MOD, CM_CLKSEL1),
	.clksel_mask	= OMAP24XX_CLKSEL_GPT1_MASK,
	.enable_reg	= OMAP_CM_REGADDR(WKUP_MOD, CM_FCLKEN),
	.enable_bit	= OMAP24XX_EN_GPT1_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(gpt1_fck, gpt10_fck_parent_names, gpt1_fck_ops);

static struct clk gpt1_ick;

static const char *gpt1_ick_parent_names[] = {
	"wu_l4_ick",
};

static struct clk_hw_omap gpt1_ick_hw = {
	.hw = {
		.clk = &gpt1_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(WKUP_MOD, CM_ICLKEN),
	.enable_bit	= OMAP24XX_EN_GPT1_SHIFT,
	.clkdm_name	= "wkup_clkdm",
};

DEFINE_STRUCT_CLK(gpt1_ick, gpt1_ick_parent_names, aes_ick_ops);

static struct clk gpt2_fck;

static struct clk_hw_omap gpt2_fck_hw = {
	.hw = {
		.clk = &gpt2_fck,
	},
	.clksel		= omap24xx_gpt_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_CLKSEL2),
	.clksel_mask	= OMAP24XX_CLKSEL_GPT2_MASK,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP24XX_EN_GPT2_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(gpt2_fck, gpt10_fck_parent_names, dss1_fck_ops);

static struct clk gpt2_ick;

static const char *gpt2_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap gpt2_ick_hw = {
	.hw = {
		.clk = &gpt2_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP24XX_EN_GPT2_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(gpt2_ick, gpt2_ick_parent_names, aes_ick_ops);

static struct clk gpt3_fck;

static struct clk_hw_omap gpt3_fck_hw = {
	.hw = {
		.clk = &gpt3_fck,
	},
	.clksel		= omap24xx_gpt_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_CLKSEL2),
	.clksel_mask	= OMAP24XX_CLKSEL_GPT3_MASK,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP24XX_EN_GPT3_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(gpt3_fck, gpt10_fck_parent_names, dss1_fck_ops);

static struct clk gpt3_ick;

static const char *gpt3_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap gpt3_ick_hw = {
	.hw = {
		.clk = &gpt3_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP24XX_EN_GPT3_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(gpt3_ick, gpt3_ick_parent_names, aes_ick_ops);

static struct clk gpt4_fck;

static struct clk_hw_omap gpt4_fck_hw = {
	.hw = {
		.clk = &gpt4_fck,
	},
	.clksel		= omap24xx_gpt_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_CLKSEL2),
	.clksel_mask	= OMAP24XX_CLKSEL_GPT4_MASK,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP24XX_EN_GPT4_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(gpt4_fck, gpt10_fck_parent_names, dss1_fck_ops);

static struct clk gpt4_ick;

static const char *gpt4_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap gpt4_ick_hw = {
	.hw = {
		.clk = &gpt4_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP24XX_EN_GPT4_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(gpt4_ick, gpt4_ick_parent_names, aes_ick_ops);

static struct clk gpt5_fck;

static struct clk_hw_omap gpt5_fck_hw = {
	.hw = {
		.clk = &gpt5_fck,
	},
	.clksel		= omap24xx_gpt_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_CLKSEL2),
	.clksel_mask	= OMAP24XX_CLKSEL_GPT5_MASK,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP24XX_EN_GPT5_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(gpt5_fck, gpt10_fck_parent_names, dss1_fck_ops);

static struct clk gpt5_ick;

static const char *gpt5_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap gpt5_ick_hw = {
	.hw = {
		.clk = &gpt5_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP24XX_EN_GPT5_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(gpt5_ick, gpt5_ick_parent_names, aes_ick_ops);

static struct clk gpt6_fck;

static struct clk_hw_omap gpt6_fck_hw = {
	.hw = {
		.clk = &gpt6_fck,
	},
	.clksel		= omap24xx_gpt_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_CLKSEL2),
	.clksel_mask	= OMAP24XX_CLKSEL_GPT6_MASK,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP24XX_EN_GPT6_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(gpt6_fck, gpt10_fck_parent_names, dss1_fck_ops);

static struct clk gpt6_ick;

static const char *gpt6_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap gpt6_ick_hw = {
	.hw = {
		.clk = &gpt6_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP24XX_EN_GPT6_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(gpt6_ick, gpt6_ick_parent_names, aes_ick_ops);

static struct clk gpt7_fck;

static struct clk_hw_omap gpt7_fck_hw = {
	.hw = {
		.clk = &gpt7_fck,
	},
	.clksel		= omap24xx_gpt_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_CLKSEL2),
	.clksel_mask	= OMAP24XX_CLKSEL_GPT7_MASK,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP24XX_EN_GPT7_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(gpt7_fck, gpt10_fck_parent_names, dss1_fck_ops);

static struct clk gpt7_ick;

static const char *gpt7_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap gpt7_ick_hw = {
	.hw = {
		.clk = &gpt7_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP24XX_EN_GPT7_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(gpt7_ick, gpt7_ick_parent_names, aes_ick_ops);

static struct clk gpt8_fck;

static struct clk_hw_omap gpt8_fck_hw = {
	.hw = {
		.clk = &gpt8_fck,
	},
	.clksel		= omap24xx_gpt_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_CLKSEL2),
	.clksel_mask	= OMAP24XX_CLKSEL_GPT8_MASK,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP24XX_EN_GPT8_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(gpt8_fck, gpt10_fck_parent_names, dss1_fck_ops);

static struct clk gpt8_ick;

static const char *gpt8_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap gpt8_ick_hw = {
	.hw = {
		.clk = &gpt8_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP24XX_EN_GPT8_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(gpt8_ick, gpt8_ick_parent_names, aes_ick_ops);

static struct clk gpt9_fck;

static struct clk_hw_omap gpt9_fck_hw = {
	.hw = {
		.clk = &gpt9_fck,
	},
	.clksel		= omap24xx_gpt_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_CLKSEL2),
	.clksel_mask	= OMAP24XX_CLKSEL_GPT9_MASK,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP24XX_EN_GPT9_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(gpt9_fck, gpt10_fck_parent_names, dss1_fck_ops);

static struct clk gpt9_ick;

static const char *gpt9_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap gpt9_ick_hw = {
	.hw = {
		.clk = &gpt9_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP24XX_EN_GPT9_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(gpt9_ick, gpt9_ick_parent_names, aes_ick_ops);

static struct clk hdq_fck;

static const char *hdq_fck_parent_names[] = {
	"func_12m_ck",
};

static struct clk_hw_omap hdq_fck_hw = {
	.hw = {
		.clk = &hdq_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP24XX_EN_HDQ_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(hdq_fck, hdq_fck_parent_names, aes_ick_ops);

static struct clk hdq_ick;

static const char *hdq_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap hdq_ick_hw = {
	.hw = {
		.clk = &hdq_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP24XX_EN_HDQ_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(hdq_ick, hdq_ick_parent_names, aes_ick_ops);

static struct clk i2c1_fck;

static const char *i2c1_fck_parent_names[] = {
	"func_12m_ck",
};

static struct clk_hw_omap i2c1_fck_hw = {
	.hw = {
		.clk = &i2c1_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP2420_EN_I2C1_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(i2c1_fck, i2c1_fck_parent_names, aes_ick_ops);

static struct clk i2c1_ick;

static const char *i2c1_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap i2c1_ick_hw = {
	.hw = {
		.clk = &i2c1_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP2420_EN_I2C1_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(i2c1_ick, i2c1_ick_parent_names, aes_ick_ops);

static struct clk i2c2_fck;

static const char *i2c2_fck_parent_names[] = {
	"func_12m_ck",
};

static struct clk_hw_omap i2c2_fck_hw = {
	.hw = {
		.clk = &i2c2_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP2420_EN_I2C2_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(i2c2_fck, i2c2_fck_parent_names, aes_ick_ops);

static struct clk i2c2_ick;

static const char *i2c2_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap i2c2_ick_hw = {
	.hw = {
		.clk = &i2c2_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP2420_EN_I2C2_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(i2c2_ick, i2c2_ick_parent_names, aes_ick_ops);

static struct clk iva1_ifck;

static struct clk_hw_omap iva1_ifck_hw = {
	.hw = {
		.clk = &iva1_ifck,
	},
	.clksel		= dsp_fck_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(OMAP24XX_DSP_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP2420_CLKSEL_IVA_MASK,
	.enable_reg	= OMAP_CM_REGADDR(OMAP24XX_DSP_MOD, CM_FCLKEN),
	.enable_bit	= OMAP2420_EN_IVA_COP_SHIFT,
	.clkdm_name	= "iva1_clkdm",
};

DEFINE_STRUCT_CLK(iva1_ifck, dsp_fck_parent_names, dsp_fck_ops);

static struct clk iva1_mpu_int_ifck;

static const char *iva1_mpu_int_ifck_parent_names[] = {
	"iva1_ifck",
};

static const struct clk_ops iva1_mpu_int_ifck_ops = {
	.init		= &omap2_init_clk_clkdm,
	.enable		= &omap2_dflt_clk_enable,
	.disable	= &omap2_dflt_clk_disable,
	.recalc_rate	= &omap_fixed_divisor_recalc,
};

static struct clk_hw_omap iva1_mpu_int_ifck_hw = {
	.hw = {
		.clk = &iva1_mpu_int_ifck,
	},
	.enable_reg	= OMAP_CM_REGADDR(OMAP24XX_DSP_MOD, CM_FCLKEN),
	.enable_bit	= OMAP2420_EN_IVA_MPU_SHIFT,
	.clkdm_name	= "iva1_clkdm",
	.fixed_div	= 2,
};

DEFINE_STRUCT_CLK(iva1_mpu_int_ifck, iva1_mpu_int_ifck_parent_names, iva1_mpu_int_ifck_ops);

static struct clk mailboxes_ick;

static const char *mailboxes_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap mailboxes_ick_hw = {
	.hw = {
		.clk = &mailboxes_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP24XX_EN_MAILBOXES_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mailboxes_ick, mailboxes_ick_parent_names, aes_ick_ops);

static const struct clksel_rate common_mcbsp_96m_rates[] = {
	{ .div = 1, .val = 0, .flags = RATE_IN_24XX },
	{ .div = 0 }
};

static const struct clksel_rate common_mcbsp_mcbsp_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_24XX },
	{ .div = 0 }
};

static const struct clksel mcbsp_fck_clksel[] = {
	{ .parent = &func_96m_ck, .rates = common_mcbsp_96m_rates },
	{ .parent = &mcbsp_clks, .rates = common_mcbsp_mcbsp_rates },
	{ .parent = NULL },
};

static const char *mcbsp1_fck_parent_names[] = {
	"func_96m_ck",
	"mcbsp_clks",
};

static struct clk mcbsp1_fck;

static struct clk_hw_omap mcbsp1_fck_hw = {
	.hw = {
		.clk = &mcbsp1_fck,
	},
	.clksel		= mcbsp_fck_clksel,
	.clksel_reg	= OMAP242X_CTRL_REGADDR(OMAP2_CONTROL_DEVCONF0),
	.clksel_mask	= OMAP2_MCBSP1_CLKS_MASK,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP24XX_EN_MCBSP1_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mcbsp1_fck, mcbsp1_fck_parent_names, dss1_fck_ops);

static struct clk mcbsp1_ick;

static const char *mcbsp1_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap mcbsp1_ick_hw = {
	.hw = {
		.clk = &mcbsp1_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP24XX_EN_MCBSP1_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mcbsp1_ick, mcbsp1_ick_parent_names, aes_ick_ops);

static struct clk mcbsp2_fck;

static struct clk_hw_omap mcbsp2_fck_hw = {
	.hw = {
		.clk = &mcbsp2_fck,
	},
	.clksel		= mcbsp_fck_clksel,
	.clksel_reg	= OMAP242X_CTRL_REGADDR(OMAP2_CONTROL_DEVCONF0),
	.clksel_mask	= OMAP2_MCBSP2_CLKS_MASK,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP24XX_EN_MCBSP2_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mcbsp2_fck, mcbsp1_fck_parent_names, dss1_fck_ops);

static struct clk mcbsp2_ick;

static const char *mcbsp2_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap mcbsp2_ick_hw = {
	.hw = {
		.clk = &mcbsp2_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP24XX_EN_MCBSP2_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mcbsp2_ick, mcbsp2_ick_parent_names, aes_ick_ops);

static struct clk mcspi1_fck;

static const char *mcspi1_fck_parent_names[] = {
	"func_48m_ck",
};

static struct clk_hw_omap mcspi1_fck_hw = {
	.hw = {
		.clk = &mcspi1_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP24XX_EN_MCSPI1_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mcspi1_fck, mcspi1_fck_parent_names, aes_ick_ops);

static struct clk mcspi1_ick;

static const char *mcspi1_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap mcspi1_ick_hw = {
	.hw = {
		.clk = &mcspi1_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP24XX_EN_MCSPI1_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mcspi1_ick, mcspi1_ick_parent_names, aes_ick_ops);

static struct clk mcspi2_fck;

static const char *mcspi2_fck_parent_names[] = {
	"func_48m_ck",
};

static struct clk_hw_omap mcspi2_fck_hw = {
	.hw = {
		.clk = &mcspi2_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP24XX_EN_MCSPI2_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mcspi2_fck, mcspi2_fck_parent_names, aes_ick_ops);

static struct clk mcspi2_ick;

static const char *mcspi2_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap mcspi2_ick_hw = {
	.hw = {
		.clk = &mcspi2_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP24XX_EN_MCSPI2_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mcspi2_ick, mcspi2_ick_parent_names, aes_ick_ops);

static struct clk mmc_fck;

static const char *mmc_fck_parent_names[] = {
	"func_96m_ck",
};

static struct clk_hw_omap mmc_fck_hw = {
	.hw = {
		.clk = &mmc_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP2420_EN_MMC_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mmc_fck, mmc_fck_parent_names, aes_ick_ops);

static struct clk mmc_ick;

static const char *mmc_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap mmc_ick_hw = {
	.hw = {
		.clk = &mmc_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP2420_EN_MMC_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mmc_ick, mmc_ick_parent_names, aes_ick_ops);

static const struct clksel_rate mpu_core_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_24XX },
	{ .div = 2, .val = 2, .flags = RATE_IN_24XX },
	{ .div = 4, .val = 4, .flags = RATE_IN_242X },
	{ .div = 6, .val = 6, .flags = RATE_IN_242X },
	{ .div = 8, .val = 8, .flags = RATE_IN_242X },
	{ .div = 0 }
};

static const struct clksel mpu_clksel[] = {
	{ .parent = &core_ck, .rates = mpu_core_rates },
	{ .parent = NULL },
};

static const char *mpu_ck_parent_names[] = {
	"core_ck",
};

static struct clk mpu_ck;

static struct clk_hw_omap mpu_ck_hw = {
	.hw = {
		.clk = &mpu_ck,
	},
	.clksel		= mpu_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(MPU_MOD, CM_CLKSEL),
	.clksel_mask	= OMAP24XX_CLKSEL_MPU_MASK,
};

DEFINE_STRUCT_CLK(mpu_ck, mpu_ck_parent_names, core_l3_ck_ops);

static struct clk mpu_wdt_fck;

static const char *mpu_wdt_fck_parent_names[] = {
	"func_32k_ck",
};

static struct clk_hw_omap mpu_wdt_fck_hw = {
	.hw = {
		.clk = &mpu_wdt_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(WKUP_MOD, CM_FCLKEN),
	.enable_bit	= OMAP24XX_EN_MPU_WDT_SHIFT,
	.clkdm_name	= "wkup_clkdm",
};

DEFINE_STRUCT_CLK(mpu_wdt_fck, mpu_wdt_fck_parent_names, aes_ick_ops);

static struct clk mpu_wdt_ick;

static const char *mpu_wdt_ick_parent_names[] = {
	"wu_l4_ick",
};

static struct clk_hw_omap mpu_wdt_ick_hw = {
	.hw = {
		.clk = &mpu_wdt_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(WKUP_MOD, CM_ICLKEN),
	.enable_bit	= OMAP24XX_EN_MPU_WDT_SHIFT,
	.clkdm_name	= "wkup_clkdm",
};

DEFINE_STRUCT_CLK(mpu_wdt_ick, mpu_wdt_ick_parent_names, aes_ick_ops);

static struct clk mspro_fck;

static const char *mspro_fck_parent_names[] = {
	"func_96m_ck",
};

static struct clk_hw_omap mspro_fck_hw = {
	.hw = {
		.clk = &mspro_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP24XX_EN_MSPRO_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mspro_fck, mspro_fck_parent_names, aes_ick_ops);

static struct clk mspro_ick;

static const char *mspro_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap mspro_ick_hw = {
	.hw = {
		.clk = &mspro_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP24XX_EN_MSPRO_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(mspro_ick, mspro_ick_parent_names, aes_ick_ops);

static struct clk omapctrl_ick;

static const char *omapctrl_ick_parent_names[] = {
	"wu_l4_ick",
};

static struct clk_hw_omap omapctrl_ick_hw = {
	.hw = {
		.clk = &omapctrl_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(WKUP_MOD, CM_ICLKEN),
	.enable_bit	= OMAP24XX_EN_OMAPCTRL_SHIFT,
	.flags		= ENABLE_ON_INIT,
	.clkdm_name	= "wkup_clkdm",
};

DEFINE_STRUCT_CLK(omapctrl_ick, omapctrl_ick_parent_names, aes_ick_ops);

static struct clk pka_ick;

static const char *pka_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap pka_ick_hw = {
	.hw = {
		.clk = &pka_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, OMAP24XX_CM_ICLKEN4),
	.enable_bit	= OMAP24XX_EN_PKA_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(pka_ick, pka_ick_parent_names, aes_ick_ops);

static struct clk rng_ick;

static const char *rng_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap rng_ick_hw = {
	.hw = {
		.clk = &rng_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, OMAP24XX_CM_ICLKEN4),
	.enable_bit	= OMAP24XX_EN_RNG_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(rng_ick, rng_ick_parent_names, aes_ick_ops);

static struct clk sdma_fck;

static const char *sdma_fck_parent_names[] = {
	"core_l3_ck",
};

static struct clk_hw_omap sdma_fck_hw = {
	.hw = {
		.clk = &sdma_fck,
	},
};

DEFINE_STRUCT_CLK(sdma_fck, sdma_fck_parent_names, core_ck_ops);

static struct clk sdma_ick;

static const char *sdma_ick_parent_names[] = {
	"core_l3_ck",
};

static struct clk_hw_omap sdma_ick_hw = {
	.hw = {
		.clk = &sdma_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN3),
	.enable_bit	= OMAP24XX_AUTO_SDMA_SHIFT,
	.clkdm_name	= "core_l3_clkdm",
};

DEFINE_STRUCT_CLK(sdma_ick, sdma_ick_parent_names, apll54_ck_ops);

static struct clk sdrc_ick;

static const char *sdrc_ick_parent_names[] = {
	"core_l3_ck",
};

static struct clk_hw_omap sdrc_ick_hw = {
	.hw = {
		.clk = &sdrc_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN3),
	.enable_bit	= OMAP24XX_AUTO_SDRC_SHIFT,
	.flags		= ENABLE_ON_INIT,
	.clkdm_name	= "core_l3_clkdm",
};

DEFINE_STRUCT_CLK(sdrc_ick, sdrc_ick_parent_names, apll54_ck_ops);

static struct clk sha_ick;

static const char *sha_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap sha_ick_hw = {
	.hw = {
		.clk = &sha_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, OMAP24XX_CM_ICLKEN4),
	.enable_bit	= OMAP24XX_EN_SHA_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(sha_ick, sha_ick_parent_names, aes_ick_ops);

static struct clk ssi_l4_ick;

static const char *ssi_l4_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap ssi_l4_ick_hw = {
	.hw = {
		.clk = &ssi_l4_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN2),
	.enable_bit	= OMAP24XX_EN_SSI_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(ssi_l4_ick, ssi_l4_ick_parent_names, aes_ick_ops);

static const struct clksel_rate ssi_ssr_sst_fck_core_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_24XX },
	{ .div = 2, .val = 2, .flags = RATE_IN_24XX },
	{ .div = 3, .val = 3, .flags = RATE_IN_24XX },
	{ .div = 4, .val = 4, .flags = RATE_IN_24XX },
	{ .div = 6, .val = 6, .flags = RATE_IN_242X },
	{ .div = 8, .val = 8, .flags = RATE_IN_242X },
	{ .div = 0 }
};

static const struct clksel ssi_ssr_sst_fck_clksel[] = {
	{ .parent = &core_ck, .rates = ssi_ssr_sst_fck_core_rates },
	{ .parent = NULL },
};

static const char *ssi_ssr_sst_fck_parent_names[] = {
	"core_ck",
};

static struct clk ssi_ssr_sst_fck;

static struct clk_hw_omap ssi_ssr_sst_fck_hw = {
	.hw = {
		.clk = &ssi_ssr_sst_fck,
	},
	.clksel		= ssi_ssr_sst_fck_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_CLKSEL1),
	.clksel_mask	= OMAP24XX_CLKSEL_SSI_MASK,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, OMAP24XX_CM_FCLKEN2),
	.enable_bit	= OMAP24XX_EN_SSI_SHIFT,
	.clkdm_name	= "core_l3_clkdm",
};

DEFINE_STRUCT_CLK(ssi_ssr_sst_fck, ssi_ssr_sst_fck_parent_names, dsp_fck_ops);

static struct clk sync_32k_ick;

static const char *sync_32k_ick_parent_names[] = {
	"wu_l4_ick",
};

static struct clk_hw_omap sync_32k_ick_hw = {
	.hw = {
		.clk = &sync_32k_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(WKUP_MOD, CM_ICLKEN),
	.enable_bit	= OMAP24XX_EN_32KSYNC_SHIFT,
	.flags		= ENABLE_ON_INIT,
	.clkdm_name	= "wkup_clkdm",
};

DEFINE_STRUCT_CLK(sync_32k_ick, sync_32k_ick_parent_names, aes_ick_ops);

static const struct clksel_rate common_clkout_src_core_rates[] = {
	{ .div = 1, .val = 0, .flags = RATE_IN_24XX },
	{ .div = 0 }
};

static const struct clksel_rate common_clkout_src_sys_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_24XX },
	{ .div = 0 }
};

static const struct clksel_rate common_clkout_src_96m_rates[] = {
	{ .div = 1, .val = 2, .flags = RATE_IN_24XX },
	{ .div = 0 }
};

static const struct clksel_rate common_clkout_src_54m_rates[] = {
	{ .div = 1, .val = 3, .flags = RATE_IN_24XX },
	{ .div = 0 }
};

static const struct clksel common_clkout_src_clksel[] = {
	{ .parent = &core_ck, .rates = common_clkout_src_core_rates },
	{ .parent = &sys_ck, .rates = common_clkout_src_sys_rates },
	{ .parent = &func_96m_ck, .rates = common_clkout_src_96m_rates },
	{ .parent = &func_54m_ck, .rates = common_clkout_src_54m_rates },
	{ .parent = NULL },
};

static const char *sys_clkout_src_parent_names[] = {
	"core_ck",
	"sys_ck",
	"func_96m_ck",
	"func_54m_ck",
};

static struct clk sys_clkout_src;

static struct clk_hw_omap sys_clkout_src_hw = {
	.hw = {
		.clk = &sys_clkout_src,
	},
	.clksel		= common_clkout_src_clksel,
	.clksel_reg	= OMAP2420_PRCM_CLKOUT_CTRL,
	.clksel_mask	= OMAP24XX_CLKOUT_SOURCE_MASK,
	.enable_reg	= OMAP2420_PRCM_CLKOUT_CTRL,
	.enable_bit	= OMAP24XX_CLKOUT_EN_SHIFT,
	.clkdm_name	= "wkup_clkdm",
};

DEFINE_STRUCT_CLK(sys_clkout_src, sys_clkout_src_parent_names, gpt1_fck_ops);

static const struct clksel_rate common_clkout_rates[] = {
	{ .div = 1, .val = 0, .flags = RATE_IN_24XX },
	{ .div = 2, .val = 1, .flags = RATE_IN_24XX },
	{ .div = 4, .val = 2, .flags = RATE_IN_24XX },
	{ .div = 8, .val = 3, .flags = RATE_IN_24XX },
	{ .div = 16, .val = 4, .flags = RATE_IN_24XX },
	{ .div = 0 }
};

static const struct clksel sys_clkout_clksel[] = {
	{ .parent = &sys_clkout_src, .rates = common_clkout_rates },
	{ .parent = NULL },
};

static const char *sys_clkout_parent_names[] = {
	"sys_clkout_src",
};

static struct clk sys_clkout;

static struct clk_hw_omap sys_clkout_hw = {
	.hw = {
		.clk = &sys_clkout,
	},
	.clksel		= sys_clkout_clksel,
	.clksel_reg	= OMAP2420_PRCM_CLKOUT_CTRL,
	.clksel_mask	= OMAP24XX_CLKOUT_DIV_MASK,
};

DEFINE_STRUCT_CLK(sys_clkout, sys_clkout_parent_names, core_l3_ck_ops);

static struct clk sys_clkout2_src;

static struct clk_hw_omap sys_clkout2_src_hw = {
	.hw = {
		.clk = &sys_clkout2_src,
	},
	.clksel		= common_clkout_src_clksel,
	.clksel_reg	= OMAP2420_PRCM_CLKOUT_CTRL,
	.clksel_mask	= OMAP2420_CLKOUT2_SOURCE_MASK,
	.enable_reg	= OMAP2420_PRCM_CLKOUT_CTRL,
	.enable_bit	= OMAP2420_CLKOUT2_EN_SHIFT,
	.clkdm_name	= "wkup_clkdm",
};

DEFINE_STRUCT_CLK(sys_clkout2_src, sys_clkout_src_parent_names, gpt1_fck_ops);

static const struct clksel sys_clkout2_clksel[] = {
	{ .parent = &sys_clkout2_src, .rates = common_clkout_rates },
	{ .parent = NULL },
};

static const char *sys_clkout2_parent_names[] = {
	"sys_clkout2_src",
};

static struct clk sys_clkout2;

static struct clk_hw_omap sys_clkout2_hw = {
	.hw = {
		.clk = &sys_clkout2,
	},
	.clksel		= sys_clkout2_clksel,
	.clksel_reg	= OMAP2420_PRCM_CLKOUT_CTRL,
	.clksel_mask	= OMAP2420_CLKOUT2_DIV_MASK,
};

DEFINE_STRUCT_CLK(sys_clkout2, sys_clkout2_parent_names, core_l3_ck_ops);

static struct clk uart1_fck;

static const char *uart1_fck_parent_names[] = {
	"func_48m_ck",
};

static struct clk_hw_omap uart1_fck_hw = {
	.hw = {
		.clk = &uart1_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP24XX_EN_UART1_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(uart1_fck, uart1_fck_parent_names, aes_ick_ops);

static struct clk uart1_ick;

static const char *uart1_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap uart1_ick_hw = {
	.hw = {
		.clk = &uart1_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP24XX_EN_UART1_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(uart1_ick, uart1_ick_parent_names, aes_ick_ops);

static struct clk uart2_fck;

static const char *uart2_fck_parent_names[] = {
	"func_48m_ck",
};

static struct clk_hw_omap uart2_fck_hw = {
	.hw = {
		.clk = &uart2_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP24XX_EN_UART2_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(uart2_fck, uart2_fck_parent_names, aes_ick_ops);

static struct clk uart2_ick;

static const char *uart2_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap uart2_ick_hw = {
	.hw = {
		.clk = &uart2_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP24XX_EN_UART2_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(uart2_ick, uart2_ick_parent_names, aes_ick_ops);

static struct clk uart3_fck;

static const char *uart3_fck_parent_names[] = {
	"func_48m_ck",
};

static struct clk_hw_omap uart3_fck_hw = {
	.hw = {
		.clk = &uart3_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, OMAP24XX_CM_FCLKEN2),
	.enable_bit	= OMAP24XX_EN_UART3_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(uart3_fck, uart3_fck_parent_names, aes_ick_ops);

static struct clk uart3_ick;

static const char *uart3_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap uart3_ick_hw = {
	.hw = {
		.clk = &uart3_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN2),
	.enable_bit	= OMAP24XX_EN_UART3_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(uart3_ick, uart3_ick_parent_names, aes_ick_ops);

static struct clk usb_fck;

static const char *usb_fck_parent_names[] = {
	"func_48m_ck",
};

static struct clk_hw_omap usb_fck_hw = {
	.hw = {
		.clk = &usb_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, OMAP24XX_CM_FCLKEN2),
	.enable_bit	= OMAP24XX_EN_USB_SHIFT,
	.clkdm_name	= "core_l3_clkdm",
};

DEFINE_STRUCT_CLK(usb_fck, usb_fck_parent_names, aes_ick_ops);

static const struct clksel_rate usb_l4_ick_core_l3_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_24XX },
	{ .div = 2, .val = 2, .flags = RATE_IN_24XX },
	{ .div = 4, .val = 4, .flags = RATE_IN_24XX },
	{ .div = 0 }
};

static const struct clksel usb_l4_ick_clksel[] = {
	{ .parent = &core_l3_ck, .rates = usb_l4_ick_core_l3_rates },
	{ .parent = NULL },
};

static const char *usb_l4_ick_parent_names[] = {
	"core_l3_ck",
};

static struct clk usb_l4_ick;

static struct clk_hw_omap usb_l4_ick_hw = {
	.hw = {
		.clk = &usb_l4_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.clksel		= usb_l4_ick_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_CLKSEL1),
	.clksel_mask	= OMAP24XX_CLKSEL_USB_MASK,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN2),
	.enable_bit	= OMAP24XX_EN_USB_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(usb_l4_ick, usb_l4_ick_parent_names, dsp_fck_ops);

static struct clk virt_prcm_set;

static const char *virt_prcm_set_parent_names[] = {
	"mpu_ck",
};

static const struct clk_ops virt_prcm_set_ops = {
	.recalc_rate	= &omap2_table_mpu_recalc,
	.set_rate	= &omap2_select_table_rate,
	.round_rate	= &omap2_round_to_table_rate,
};

static struct clk_hw_omap virt_prcm_set_hw = {
	.hw = {
		.clk = &virt_prcm_set,
	},
};

DEFINE_STRUCT_CLK(virt_prcm_set, virt_prcm_set_parent_names, virt_prcm_set_ops);

static const struct clksel_rate vlynq_fck_96m_rates[] = {
	{ .div = 1, .val = 0, .flags = RATE_IN_242X },
	{ .div = 0 }
};

static const struct clksel_rate vlynq_fck_core_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_242X },
	{ .div = 2, .val = 2, .flags = RATE_IN_242X },
	{ .div = 3, .val = 3, .flags = RATE_IN_242X },
	{ .div = 4, .val = 4, .flags = RATE_IN_242X },
	{ .div = 6, .val = 6, .flags = RATE_IN_242X },
	{ .div = 8, .val = 8, .flags = RATE_IN_242X },
	{ .div = 9, .val = 9, .flags = RATE_IN_242X },
	{ .div = 12, .val = 12, .flags = RATE_IN_242X },
	{ .div = 16, .val = 16, .flags = RATE_IN_242X },
	{ .div = 18, .val = 18, .flags = RATE_IN_242X },
	{ .div = 0 }
};

static const struct clksel vlynq_fck_clksel[] = {
	{ .parent = &func_96m_ck, .rates = vlynq_fck_96m_rates },
	{ .parent = &core_ck, .rates = vlynq_fck_core_rates },
	{ .parent = NULL },
};

static const char *vlynq_fck_parent_names[] = {
	"func_96m_ck",
	"core_ck",
};

static struct clk vlynq_fck;

static struct clk_hw_omap vlynq_fck_hw = {
	.hw = {
		.clk = &vlynq_fck,
	},
	.clksel		= vlynq_fck_clksel,
	.clksel_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_CLKSEL1),
	.clksel_mask	= OMAP2420_CLKSEL_VLYNQ_MASK,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP2420_EN_VLYNQ_SHIFT,
	.clkdm_name	= "core_l3_clkdm",
};

DEFINE_STRUCT_CLK(vlynq_fck, vlynq_fck_parent_names, dss1_fck_ops);

static struct clk vlynq_ick;

static const char *vlynq_ick_parent_names[] = {
	"core_l3_ck",
};

static struct clk_hw_omap vlynq_ick_hw = {
	.hw = {
		.clk = &vlynq_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP2420_EN_VLYNQ_SHIFT,
	.clkdm_name	= "core_l3_clkdm",
};

DEFINE_STRUCT_CLK(vlynq_ick, vlynq_ick_parent_names, aes_ick_ops);

static struct clk wdt1_ick;

static const char *wdt1_ick_parent_names[] = {
	"wu_l4_ick",
};

static struct clk_hw_omap wdt1_ick_hw = {
	.hw = {
		.clk = &wdt1_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(WKUP_MOD, CM_ICLKEN),
	.enable_bit	= OMAP24XX_EN_WDT1_SHIFT,
	.clkdm_name	= "wkup_clkdm",
};

DEFINE_STRUCT_CLK(wdt1_ick, wdt1_ick_parent_names, aes_ick_ops);

static struct clk wdt1_osc_ck;

static const char *wdt1_osc_ck_parent_names[] = {
	"osc_ck",
};

static struct clk_hw_omap wdt1_osc_ck_hw = {
	.hw = {
		.clk = &wdt1_osc_ck,
	},
};

DEFINE_STRUCT_CLK(wdt1_osc_ck, wdt1_osc_ck_parent_names, core_ck_ops);

static struct clk wdt3_fck;

static const char *wdt3_fck_parent_names[] = {
	"func_32k_ck",
};

static struct clk_hw_omap wdt3_fck_hw = {
	.hw = {
		.clk = &wdt3_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP2420_EN_WDT3_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(wdt3_fck, wdt3_fck_parent_names, aes_ick_ops);

static struct clk wdt3_ick;

static const char *wdt3_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap wdt3_ick_hw = {
	.hw = {
		.clk = &wdt3_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP2420_EN_WDT3_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(wdt3_ick, wdt3_ick_parent_names, aes_ick_ops);

static struct clk wdt4_fck;

static const char *wdt4_fck_parent_names[] = {
	"func_32k_ck",
};

static struct clk_hw_omap wdt4_fck_hw = {
	.hw = {
		.clk = &wdt4_fck,
	},
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_FCLKEN1),
	.enable_bit	= OMAP24XX_EN_WDT4_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(wdt4_fck, wdt4_fck_parent_names, aes_ick_ops);

static struct clk wdt4_ick;

static const char *wdt4_ick_parent_names[] = {
	"l4_ck",
};

static struct clk_hw_omap wdt4_ick_hw = {
	.hw = {
		.clk = &wdt4_ick,
	},
	.allow_idle	= &omap2_clkt_iclk_allow_idle,
	.deny_idle	= &omap2_clkt_iclk_deny_idle,
	.enable_reg	= OMAP_CM_REGADDR(CORE_MOD, CM_ICLKEN1),
	.enable_bit	= OMAP24XX_EN_WDT4_SHIFT,
	.clkdm_name	= "core_l4_clkdm",
};

DEFINE_STRUCT_CLK(wdt4_ick, wdt4_ick_parent_names, aes_ick_ops);

