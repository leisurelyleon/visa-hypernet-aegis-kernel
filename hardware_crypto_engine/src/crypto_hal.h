#ifndef CRYPTO_HAL_H
#define CRYPTO_HAL_H

#include <stdint.h>
#include <stddef.h>

// Hooks directly into the CPU's thermal noise generator
int generate_hardware_entropy(uint8_t *buffer, size_t length);

// Extern linkage to our hand-tuned x86-64 AVX-512 Assembly file
extern void aes_gcm_encrypt_avx512(const uint8_t *plaintext, size_t length,
                                   const uint8_t *key, const uint8_t *iv,
                                   uint8_t *ciphertext, uint8_t *mac);

// Point-of-Sale Authorization Protocol
void execute_pos_handshake(uint32_t terminal_id);

#endif // CRYPTO_HAL_H
