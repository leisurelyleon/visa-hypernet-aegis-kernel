#include "crypto_hal.h"
#include <immintrin.h>
#include <stdio.h>

// Fetches true cryptographic entropy directly from the CPU hardware (RdRand)
int generate_hardware_entropy(uint8_t *buffer, size_t length) {
    size_t generated = 0;
    uint64_t random_val;

    printf("[HW-RNG] Engaging CPU RdRand instruction for true entropy...\n");

    while (generated < length) {
        // _rdrand64_step utilizes the CPU's on-chip thermal noise generator
        // This is mathematically superior to software-based pseudorandom generators like /dev/urandom
        if (_rdrand64_step((unsigned long long*)&random_val)) {
            size_t chunk = (length - generated < 8) ? (length - generated) : 8;
            for (size_t i = 0; i < chunk; ++i) {
                buffer[generated + i] = (random_val >> (i * 8)) & 0xFF;
            }
            generated += chunk;
        } else {
            // Hardware RNG is temporarily exhausted (can happen under extreme load)
            return -1;
        }
    }
    return 0;
}
