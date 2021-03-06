// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2014-2019, Linaro Limited
 */

#include <assert.h>
#include <crypto/crypto.h>
#include <crypto/crypto_impl.h>
#include <stdlib.h>
#include <tee_api_types.h>
#include <tomcrypt.h>
#include <util.h>

#include "des2_key.h"

struct ltc_cbc_ctx {
	struct crypto_cipher_ctx ctx;
	int cipher_idx;
	bool des3;
	int (*update)(const unsigned char *src, unsigned char *dst,
		      unsigned long len, symmetric_CBC *cbc);
	symmetric_CBC state;
};

static const struct crypto_cipher_ops ltc_cbc_ops;

static struct ltc_cbc_ctx *to_cbc_ctx(struct crypto_cipher_ctx *ctx)
{
	assert(ctx && ctx->ops == &ltc_cbc_ops);

	return container_of(ctx, struct ltc_cbc_ctx, ctx);
}

static TEE_Result ltc_cbc_init(struct crypto_cipher_ctx *ctx,
			       TEE_OperationMode mode, const uint8_t *key1,
			       size_t key1_len, const uint8_t *key2 __unused,
			       size_t key2_len __unused,
			       const uint8_t *iv, size_t iv_len)
{
	struct ltc_cbc_ctx *c = to_cbc_ctx(ctx);
	uint8_t tmp[24] = { 0 };
	const uint8_t *k = key1;
	size_t kl = key1_len;

	if ((int)iv_len != cipher_descriptor[c->cipher_idx]->block_length)
		return TEE_ERROR_BAD_PARAMETERS;

	if (mode == TEE_MODE_ENCRYPT)
		c->update = cbc_encrypt;
	else
		c->update = cbc_decrypt;

	if (c->des3)
		get_des2_key(&k, &kl, tmp);

	if (cbc_start(c->cipher_idx, iv, k, kl, 0, &c->state) == CRYPT_OK)
		return TEE_SUCCESS;
	else
		return TEE_ERROR_BAD_STATE;
}

static TEE_Result ltc_cbc_update(struct crypto_cipher_ctx *ctx,
				 bool last_block __unused,
				 const uint8_t *data, size_t len, uint8_t *dst)
{
	struct ltc_cbc_ctx *c = to_cbc_ctx(ctx);

	if (c->update && c->update(data, dst, len, &c->state) == CRYPT_OK)
		return TEE_SUCCESS;
	else
		return TEE_ERROR_BAD_STATE;
}

static void ltc_cbc_final(struct crypto_cipher_ctx *ctx)
{
	cbc_done(&to_cbc_ctx(ctx)->state);
}

static void ltc_cbc_free_ctx(struct crypto_cipher_ctx *ctx)
{
	free(to_cbc_ctx(ctx));
}

static void ltc_cbc_copy_state(struct crypto_cipher_ctx *dst_ctx,
			       struct crypto_cipher_ctx *src_ctx)
{
	struct ltc_cbc_ctx *src = to_cbc_ctx(src_ctx);
	struct ltc_cbc_ctx *dst = to_cbc_ctx(dst_ctx);

	assert(src->cipher_idx == dst->cipher_idx);
	dst->update = src->update;
	dst->state = src->state;
}

static const struct crypto_cipher_ops ltc_cbc_ops = {
	.init = ltc_cbc_init,
	.update = ltc_cbc_update,
	.final = ltc_cbc_final,
	.free_ctx = ltc_cbc_free_ctx,
	.copy_state = ltc_cbc_copy_state,
};

static TEE_Result ltc_cbc_alloc_ctx(struct crypto_cipher_ctx **ctx_ret,
				    int cipher_idx, bool des3)
{
	struct ltc_cbc_ctx *c = NULL;

	if (cipher_idx < 0)
		return TEE_ERROR_NOT_SUPPORTED;

	c = calloc(1, sizeof(*c));
	if (!c)
		return TEE_ERROR_OUT_OF_MEMORY;

	c->ctx.ops = &ltc_cbc_ops;
	c->cipher_idx = cipher_idx;
	c->des3 = des3;
	*ctx_ret = &c->ctx;

	return TEE_SUCCESS;
}

#if defined(_CFG_CORE_LTC_AES) && !defined(CFG_CORE_SE05X)
TEE_Result crypto_aes_cbc_alloc_ctx(struct crypto_cipher_ctx **ctx)
{
	return ltc_cbc_alloc_ctx(ctx, find_cipher("aes"), false);
}
#endif

#if defined(_CFG_CORE_LTC_DES)
#if !defined(CFG_CORE_SE05X)
TEE_Result crypto_des_cbc_alloc_ctx(struct crypto_cipher_ctx **ctx)
{
	return ltc_cbc_alloc_ctx(ctx, find_cipher("des"), false);
}
#endif
TEE_Result crypto_des3_cbc_alloc_ctx(struct crypto_cipher_ctx **ctx)
{
	return ltc_cbc_alloc_ctx(ctx, find_cipher("3des"), true);
}
#endif
