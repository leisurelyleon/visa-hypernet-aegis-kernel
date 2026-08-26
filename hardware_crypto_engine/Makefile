# ============================================================================
# VISA HYPERNET-AEGIS: Bare-Metal Crypto Engine Build Script
# ============================================================================

CC = gcc
CFLAGS = -O3 -march=native -Wall -Wextra -I./src
AS = gcc
ASFLAGS = -c

SRCS_C = src/pos_terminal_handshake.c src/hardware_rng.c
SRCS_ASM = src/aes_gcm_avx512.S
OBJS = $(SRCS_C:.c=.o) $(SRCS_ASM:.S=.o)
TARGET = visa_crypto_engine

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "========================================================="
	@echo " [SUCCESS] Bare-Metal Crypto Engine Compiled"
	@echo "========================================================="

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -f src/*.o $(TARGET)
	@echo "[CLEAN] Crypto engine artifacts removed."
