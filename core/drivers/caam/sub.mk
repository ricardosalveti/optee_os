incdirs-y += include

subdirs-$(CFG_MX6)$(CFG_MX7)$(CFG_MX7ULP) += hal/imx_6_7
subdirs-$(CFG_MX8M)$(CFG_MX8MM) += hal/imx_8m
subdirs-y += utils

srcs-y += caam_ctrl.c
srcs-y += caam_jr.c
srcs-y += caam_rng.c
srcs-$(CFG_CRYPTO_HASH_HW)   += caam_hash.c
subdirs-$(CFG_CRYPTO_CIPHER_HW) += cipher
