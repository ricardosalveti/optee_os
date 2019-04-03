/* SPDX-License-Identifier: BSD-2-Clause */
/**
 * @copyright 2018 NXP
 *
 * @file    crypto_extension.h
 *
 * @brief   This is the Cryptographic API extension.
 */

#ifndef __CRYPTO_EXTENSION_H
#define __CRYPTO_EXTENSION_H

#include <tee_api_types.h>
#include <libimxcrypt.h>

#ifdef _CFG_CRYPTO_WITH_MP
/**
 * @brief   MP Signature Curve enumerate
 */
enum imxcrypt_mp_id {
	MP_P256 = 0,       ///< P256
	MP_P384,           ///< P384
	MP_P521           ///< P521
};

/**
 * @brief   MP Signature data
 */
struct imxcrypt_mp_sign {
	struct imxcrypt_buf message;    ///< Message to sign
	struct imxcrypt_buf signature;  ///< Signature of the message
};

/**
 * @brief   Export the MP Public Key
 *
 * @param[out] pubkey    MP Public key structure
 *
 * @retval TEE_SUCCESS                 Success
 * @retval TEE_ERROR_NOT_IMPLEMENTED   Algorithm is not implemented
 * @retval TEE_ERROR_BAD_PARAMETERS    Bad parameters
 */
TEE_Result crypto_mp_export_pubkey(struct imxcrypt_buf *pubkey);

/**
 * @brief   Export the MPMR content
 *
 * @param[out] mpmr_reg                MPMR register
 *
 * @retval TEE_SUCCESS                 Success
 * @retval TEE_ERROR_NOT_IMPLEMENTED   Algorithm is not implemented
 * @retval TEE_ERROR_BAD_PARAMETERS    Bad parameters
 */
TEE_Result crypto_mp_export_mpmr(struct imxcrypt_buf *mpmr_reg);

/**
 * @brief   Sign a message with the MP Private key\n
 *          and returns the signature.
 *
 * @param[in/out] sdata      MP Signature structure
 *
 * @retval TEE_SUCCESS                 Success
 * @retval TEE_ERROR_NOT_IMPLEMENTED   Algorithm is not implemented
 * @retval TEE_ERROR_BAD_PARAMETERS    Bad parameters
 */
TEE_Result crypto_mp_sign(struct imxcrypt_mp_sign *sdata);

/**
 * @brief   Generation of the Hardware Unique Key (HUK)
 *
 * @param[out] huk  HUK key generated
 *
 * @retval TEE_SUCCESS                 Success
 * @retval TEE_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval TEE_ERROR_BAD_PARAMETERS    Bad parameters
 * @retval TEE_ERROR_NOT_IMPLEMENTED   Algorithm is not implemented
 * @retval TEE_ERROR_GENERIC           Generic error
 */
TEE_Result crypto_generate_huk(struct imxcrypt_buf *huk);
#endif // _CFG_CRYPTO_WITH_MP

#ifdef _CFG_CRYPTO_WITH_BLOB
/**
 * @brief Encapsulates input data.
 *  Resulting blob is the input length + 48 bytes
 *
 * @param[in] type       Type of blob
 * @param[in] key        Key derivation (must be 128 bits length)
 * @param[in] payload    Data to encapsulate
 * @param[in] blob_len   Length of the blob buffer in bytes (size >= data_len +
 *                       48 bytes)
 *
 * @param[in/out] blob   Resulting blob. The maximum length of the blob
 *                       buffer in bytes must be given as input
 *                       (size >= data_len + 48 bytes)
 *
 * @retval TEE_SUCCESS                 Success
 * @retval TEE_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval TEE_ERROR_BAD_PARAMETERS    Bad parameters
 * @retval TEE_ERROR_NOT_IMPLEMENTED   Algorithm is not implemented
 * @retval TEE_ERROR_SHORT_BUFFER      Output buffer too short
 * @retval TEE_ERROR_GENERIC           Generic error
 */
TEE_Result blob_encapsulate(enum blob_type type,
		const uint8_t *key,
		const struct imxcrypt_buf *payload,
		struct imxcrypt_buf *blob);

/**
 * @brief Decapsulates input blob.
 *  Resulting data is the blob length - 48 bytes
 *
 * @param[in] type         Type of blob
 * @param[in] key          Key derivation (must be 128 bits length)
 * @param[in] blob         Resulting blob.
 *
 * @param[in/out] payload  Data to decapsulate. The maximum length of the
 *                         buffer in bytes must be given as input
 *                         (payload length >= blob length - 48 bytes)
 *
 * @retval TEE_SUCCESS                 Success
 * @retval TEE_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval TEE_ERROR_BAD_PARAMETERS    Bad parameters
 * @retval TEE_ERROR_NOT_IMPLEMENTED   Algorithm is not implemented
 * @retval TEE_ERROR_SHORT_BUFFER      Output buffer too short
 * @retval TEE_ERROR_GENERIC           Generic error
 */
TEE_Result blob_decapsulate(enum blob_type type,
		const uint8_t *key,
		struct imxcrypt_buf *payload,
		const struct imxcrypt_buf *blob);
#endif // _CFG_CRYPTO_WITH_BLOB

#endif /* __CRYPTO_EXTENSION_H */