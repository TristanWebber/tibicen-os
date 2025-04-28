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

# Collect include directories recursively
KERNEL_INC  := $(shell find $(KERNEL_DIR) -type d)
USER_INC    := $(shell find $(USER_DIR) -type d)
INC         := $(KERNEL_INC) $(USER_INC)

# Separate the C and Assembly source files
KERNEL_C_SRCS := $(filter %.c,$(KERNEL_SRCS))
USER_C_SRCS   := $(filter %.c,$(USER_SRCS))
KERNEL_S_SRCS := $(filter %.s,$(KERNEL_SRCS))
USER_S_SRCS   := $(filter %.s,$(USER_SRCS))

# Object files for kernel and user code
KERNEL_OBJS := $(patsubst $(KERNEL_DIR)/%.c, $(BUILD_DIR)/%.o, $(KERNEL_C_SRCS)) \
               $(patsubst $(KERNEL_DIR)/%.s, $(BUILD_DIR)/%.o, $(KERNEL_S_SRCS))

USER_OBJS   := $(patsubst $(USER_DIR)/%.c, $(BUILD_DIR)/%.o, $(USER_C_SRCS)) \
               $(patsubst $(USER_DIR)/%.s, $(BUILD_DIR)/%.o, $(USER_S_SRCS))

CFLAGS      += $(addprefix -I, $(INC))

build: $(BUILD_DIR)/firmware.bin

$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c
	@mkdir -p $(dir $@)
	$(TOOLCHAIN)-gcc $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(USER_DIR)/%.c
	@mkdir -p $(dir $@)
	$(TOOLCHAIN)-gcc $(CFLAGS) -c $< -o $@
	./rename_user_sections.sh $@ $@ $(TOOLCHAIN)

$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.s
	@mkdir -p $(dir $@)
	$(TOOLCHAIN)-gcc $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(USER_DIR)/%.s
	@mkdir -p $(dir $@)
	$(TOOLCHAIN)-gcc $(CFLAGS) -c $< -o $@
	./rename_user_sections.sh $@ $@ $(TOOLCHAIN)

$(BUILD_DIR)/firmware.elf: $(KERNEL_OBJS) $(USER_OBJS)
	$(TOOLCHAIN)-gcc $(CFLAGS) $(KERNEL_OBJS) $(USER_OBJS) $(LINKFLAGS) -o $@

$(BUILD_DIR)/firmware.bin: $(BUILD_DIR)/firmware.elf
	esptool.py --chip esp32c3 elf2image $<

flash:
	esptool.py -p $(PORT) $(FLASHFLAGS) $(BUILD_DIR)/firmware.bin

monitor:
	cu -s 115200 -l $(PORT)

clean:
	rm -rf $(BUILD_DIR)

erase-flash:
	esptool.py -p $(PORT) -b 460800 --before default_reset --after hard_reset \
		--chip esp32c3 erase_flash

disass: $(BUILD_DIR)/firmware.S

$(BUILD_DIR)/firmware.S: $(BUILD_DIR)/firmware.elf
	$(TOOLCHAIN)-objdump -D $(BUILD_DIR)/firmware.elf > $(BUILD_DIR)/firmware.S
