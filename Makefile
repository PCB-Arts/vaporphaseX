# Repository-wide targets: builds all projects and turns them into the signed
# update files. Images, signing and keys are done by tools/imgtool.py,
# which also runs on its own (e.g. on Windows after a build in CubeIDE). The
# project Makefiles (build, flash, generate) also work on their own.

BUILD_CONFIG ?= Debug
# Needs tools/requirements.txt installed
PYTHON ?= uv run python
IMGTOOL := $(PYTHON) tools/imgtool.py
# Private image signing key: 'make privkey' creates it, 'make sign' uses it
PRIV_KEY ?= keys/key.pem
# Optional: public key (PEM) to compile into display-bootloader instead of the
# public part of PRIV_KEY, e.g. the production key's without its secret
PUB_KEY  ?=

PROJECTS := core-bootloader core-fw display-bootloader display-fw
# Projects the update images are cut from (see IMAGES in tools/imgtool.py)
IMAGE_PROJECTS := core-fw display-fw

.PHONY: all build $(PROJECTS) images sign hash privkey pubkey clean help

all: build

build: $(PROJECTS)

$(PROJECTS):
	$(MAKE) -C $@ build BUILD_CONFIG=$(BUILD_CONFIG)

images: $(IMAGE_PROJECTS)
	$(IMGTOOL) images --config $(BUILD_CONFIG)

sign: $(IMAGE_PROJECTS)
	$(IMGTOOL) sign --config $(BUILD_CONFIG) --key $(PRIV_KEY)

# Zero signature: only flashed by a display-bootloader whose core reports third
# party software as allowed (THIRD_PARTY pin low)
hash: $(IMAGE_PROJECTS)
	$(IMGTOOL) hash --config $(BUILD_CONFIG)

# Refuses to overwrite an existing key, since a new key invalidates every image
# signed with the old one
privkey:
	$(IMGTOOL) privkey --key $(PRIV_KEY)

# Public key include for display-bootloader, from PUB_KEY or PRIV_KEY
pubkey:
	$(IMGTOOL) pubkey --key $(or $(PUB_KEY),$(PRIV_KEY))

clean:
	for p in $(PROJECTS); do $(MAKE) -C $$p clean || exit 1; done
	rm -rf out .cubeide-ws

help:
	@echo "make, make build  build all projects: $(PROJECTS)"
	@echo "make <project>    build one project"
	@echo "make images       build + derive the unsigned images"
	@echo "make sign         images + sign them with PRIV_KEY -> out/$(BUILD_CONFIG)/signed/"
	@echo "make hash         images + hash only, no key -> out/$(BUILD_CONFIG)/hash-only/"
	@echo "                  (needs the third party jumper on the core board)"
	@echo "make privkey      create PRIV_KEY (refuses to overwrite)"
	@echo "make pubkey       public key of PUB_KEY or PRIV_KEY -> keys/include/"
	@echo "                  (compiled into display-bootloader)"
	@echo "make clean        clean all projects and out/"
	@echo
	@echo "Per project (build, flash, generate, ...): make -C <project> help"
	@echo "Without make: python tools/imgtool.py --help"
	@echo
	@echo "BUILD_CONFIG=Release"
	@echo "                  select the Release configuration (now $(BUILD_CONFIG))"
	@echo "PYTHON=<path>     now $(PYTHON)"
	@echo "PRIV_KEY=<file>   now $(PRIV_KEY)"
	@echo "PUB_KEY=<file>    now $(or $(PUB_KEY),unset: public part of PRIV_KEY)"
