#ifndef TINYAES_H
#define TINYAES_H

#include <stdint.h>
#include <stddef.h>

// #define the macros below to 1/0 to enable/disable the mode of operation.
//
// TINYAES_ENABLE_CBC enables AES encryption in CBC-mode of operation.
// TINYAES_ENABLE_CTR enables encryption in counter-mode.
// TINYAES_ENABLE_ECB enables the basic ECB 16-byte block algorithm. All can be enabled simultaneously.

// The #ifndef-guard allows it to be configured before #include'ing or at compile time.
#ifndef TINYAES_ENABLE_CBC
	#define TINYAES_ENABLE_CBC 1
#endif

#ifndef TINYAES_ENABLE_ECB
	#define TINYAES_ENABLE_ECB 1
#endif

#ifndef TINYAES_ENABLE_CTR
	#define TINYAES_ENABLE_CTR 1
#endif

#define TINYAES_ENABLE_AES128 1
// #define TINYAES_ENABLE_AES192 1
// #define TINYAES_ENABLE_AES256 1

#define TINYAES_AES_BLOCKLEN 16 // Block length in bytes - AES is 128b block only

#if defined(TINYAES_ENABLE_AES256) && (TINYAES_ENABLE_AES256 == 1)
	#define TINYAES_AES_KEYLEN 32
	#define TINYAES_AES_keyExpSize 240
#elif defined(TINYAES_ENABLE_AES192) && (TINYAES_ENABLE_AES192 == 1)
	#define TINYAES_AES_KEYLEN 24
	#define TINYAES_AES_keyExpSize 208
#else
	#define TINYAES_AES_KEYLEN 16   // Key length in bytes
	#define TINYAES_AES_keyExpSize 176
#endif

struct AES_ctx {
	uint8_t RoundKey[TINYAES_AES_keyExpSize];
#if (defined(TINYAES_ENABLE_CBC) && (TINYAES_ENABLE_CBC == 1)) || (defined(TINYAES_ENABLE_CTR) && (TINYAES_ENABLE_CTR == 1))
	uint8_t Iv[TINYAES_AES_BLOCKLEN];
#endif
};

void AES_init_ctx(struct AES_ctx *ctx, const uint8_t *key);

#if (defined(TINYAES_ENABLE_CBC) && (TINYAES_ENABLE_CBC == 1)) || (defined(TINYAES_ENABLE_CTR) && (TINYAES_ENABLE_CTR == 1))

void AES_init_ctx_iv(struct AES_ctx *ctx, const uint8_t *key, const uint8_t *iv);

void AES_ctx_set_iv(struct AES_ctx *ctx, const uint8_t *iv);

#endif

#if defined(TINYAES_ENABLE_ECB) && (TINYAES_ENABLE_ECB == 1)

// buffer size is exactly TINYAES_AES_BLOCKLEN bytes;
// you need only AES_init_ctx as IV is not used in ECB 
// NB: ECB is considered insecure for most uses
void AES_ECB_encrypt(const struct AES_ctx *ctx, uint8_t *buf);

void AES_ECB_decrypt(const struct AES_ctx *ctx, uint8_t *buf);

#endif // #if defined(TINYAES_ENABLE_ECB) && (TINYAES_ENABLE_ECB == 1)


#if defined(TINYAES_ENABLE_CBC) && (TINYAES_ENABLE_CBC == 1)

// buffer size MUST be a multiple of TINYAES_AES_BLOCKLEN;
// Suggest https://en.wikipedia.org/wiki/Padding_(cryptography)#PKCS7 for padding scheme
// NOTES: you need to set IV in ctx via AES_init_ctx_iv() or AES_ctx_set_iv()
//        no IV should ever be reused with the same key 
void AES_CBC_encrypt_buffer(struct AES_ctx *ctx, uint8_t *buf, size_t length);

void AES_CBC_decrypt_buffer(struct AES_ctx *ctx, uint8_t *buf, size_t length);

#endif // #if defined(TINYAES_ENABLE_CBC) && (TINYAES_ENABLE_CBC == 1)


#if defined(TINYAES_ENABLE_CTR) && (TINYAES_ENABLE_CTR == 1)

// Same function for encrypting as for decrypting. 
// IV is incremented for every block, and used after encryption as XOR-compliment for output
// Suggesting https://en.wikipedia.org/wiki/Padding_(cryptography)#PKCS7 for padding scheme
// NOTES: you need to set IV in ctx with AES_init_ctx_iv() or AES_ctx_set_iv()
//        no IV should ever be reused with the same key 
void AES_CTR_xcrypt_buffer(struct AES_ctx *ctx, uint8_t *buf, size_t length);

#endif // #if defined(TINYAES_ENABLE_CTR) && (TINYAES_ENABLE_CTR == 1)


#endif // TINYAES_H
