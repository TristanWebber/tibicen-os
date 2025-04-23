# Source and destination parent dirs
KERNEL_DIR  := kernel
USER_DIR    := user
BUILD_DIR   := build

# Toolchain and flags
CFLAGS      ?= -Wall -Wextra -Werror=all -march=rv32imc_zicsr -mabi=ilp32 \
               -Og -ffunction-sections -fdata-sections -ffreestanding \
               $(EXTRA_CFLAGS)
LINKFLAGS   ?= -T$(KERNEL_DIR)/link.ld -nostdlib $(EXTRA_LINKFLAGS)
FLASHFLAGS  ?= --baud 460800 --before default_reset --after hard_reset \
               write_flash -z --flash_mode dio --flash_freq 80m --flash_size \
               4MB 0x0
TOOLCHAIN   ?= riscv32-esp-elf

# Collect source files recursively
KERNEL_SRCS := $(shell find $(KERNEL_DIR) -name '*.c' -or -name '*.s')
USER_SRCS   := $(shell find $(USER_DIR) -name '*.c' -or -name '*.s')
SRCS        := $(KERNEL_SRCS) $(USER_SRCS) $(SOURCES)

# Collect header files recursively
KERNEL_INC  := $(shell find $(KERNEL_DIR) -name '*.h' -exec dirname {} \; | sort -u)
USER_INC    := $(shell find $(USER_DIR) -name '*.h' -exec dirname {} \; | sort -u)
INC         := $(KERNEL_INC) $(USER_INC)

# Collect include directories recursively
#KERNEL_INC  := $(shell find $(KERNEL_DIR) -type d -name 'include')
#INC         := $(KERNEL_INC)

CFLAGS      += $(addprefix -I, $(INC))

build: $(BUILD_DIR)/firmware.bin

$(BUILD_DIR)/firmware.elf: $(SRCS)
	$(TOOLCHAIN)-gcc  $(CFLAGS) $(SRCS) $(LINKFLAGS) -o $@

$(BUILD_DIR)/firmware.bin: $(BUILD_DIR)/firmware.elf
	esptool.py --chip esp32c3 elf2image $<

flash:
	esptool.py -p $(PORT) $(FLASHFLAGS) $(BUILD_DIR)/firmware.bin

monitor:
	cu -s 115200 -l $(PORT)

clean:
	@rm -rf $(BUILD_DIR)/*.{bin,elf,s,S} $(BUILD_DIR)/firmware*

erase-flash:
	esptool.py -p $(PORT) -b 460800 --before default_reset --after hard_reset \
		--chip esp32c3 erase_flash

disass: $(BUILD_DIR)/firmware.S

$(BUILD_DIR)/firmware.S: $(BUILD_DIR)/firmware.elf
	$(TOOLCHAIN)-objdump -D $(BUILD_DIR)/firmware.elf > $(BUILD_DIR)/firmware.S
