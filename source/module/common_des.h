#ifndef COMMON_dES_H__
#define COMMON_dES_H__

#include "hal_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * The DES function
 * input: 64 bit message
 * key: 64 bit key for encryption/decryption
 * mode: 'e' = encryption; 'd' = decryption
 */
	uint64_t des(uint64_t input, uint64_t key, uint8_t mode);


#ifdef __cplusplus
}
#endif

#endif
