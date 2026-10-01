PROJECT    := display-bootloader
FLASH_FREQ := 8000
FLASH_HELP := build + program the bootloader (internal flash, first 256 KiB)
# Public key of the image signing key, compiled into command.c
PUB_KEY_INC := ../keys/include/fw_update_sig_pk.c_inc

include ../tools/common.mk

#  add PUB_KEY_INC as prerequisites for build target of common.mk
build: $(PUB_KEY_INC)

$(PUB_KEY_INC):
	@echo "==> $(PUB_KEY_INC) missing: run 'make pubkey' in the root directory (or: python tools/imgtool.py pubkey)" >&2
	@echo "    (PUB_KEY=<production public key.pem> for production; see 'make help' there)" >&2
	@exit 1
