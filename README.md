# tibicen-os

A multi-tasking monolithic OS kernel for low-cost RISC-V hardware. Created for educational purposes. The initial implementation uses a ESP32-C3 microcontroller.

## Vision

This project has been created primarily for purposes of personal training. The kernel is intended to be run on real, low-cost hardware rather than a virtual machine. The following features will be implemented, in roughly this order:

### Short-term
- [x] Print to a host terminal
- [x] Multitasking
- [ ] Hardware interrupts
- [x] Userspace and syscalls
- [ ] Memory protection (IN PROGRESS)
- [ ] Crontab-like scheduler for user tasks
- [ ] IO drivers and userspace SDK
- [ ] Heap memory
- [ ] Basic file system
- [ ] Sharing data between threads

### Mid-term
- [ ] Extend to other hardware
- [ ] Multicore
- [ ] Paging
- [ ] Virtual memory

## Hardware

There's something magical about bringing up a physical device. The standard practice of developing educational OS kernels for use on an emulator is irrefutably an excellent way to fearlessly test and iterate bare metal software but particularly in the early stages, it doesn't 'feel' very bare-metal. My approach to this project has been to start with a very low cost device that allows for all testing to occur on a physical device, and is unlikely to be too upsetting if it ends up bricked in the process. Based on the cost and features, I have started with the Espressif ESP32-C3 microcontroller. This device has the feature set and power to implement all of the short-term goals of this project on something that is readily available for less than $10.

A reality of educational kernels is that they start small and very rarely grow to the point where they need genuinely substantial processing power, memory or storage. I don't anticipate that this kernel will outgrow the features of the ESP32-C3, however there are some kernel features I would like to implement that require features that this particular microcontroller doesn't have. To implement paging, virtual memory and support of multiple cores, the project will ultimately need to be ported from a microcontroller to a microprocessor. This will use either a Milk-V Duo or Sipeed LicheeRV Nano, both of which use the Sophgo SG2002 SoC with two RISC-V cores and are available for less than $30.

## Tools

Development has been done on a Linux machine with a gcc cross-compiler toolchain. ESP32-C3 development uses the ESP-IDF toolchain because it provides a straightforward way to get a RISC-V cross-compiler toolchain.

## Building

### Pre-requisites

- ESP-IDF toolchain installed
- `riscv32-esp-elf` added to PATH. This can be achieved by running `$HOME/esp/esp-idf/export.sh` or equivalent.
- Device upload port in PORT. e.g. `export PORT=/dev/ttyACM0`
- `cu` installed for serial monitoring
- `rename_user_sections.sh` is executable by the user, eg. `chmod +x rename_user_sections.sh`

Makefile commands are currently defined as follows:

`build`: Builds the firmware.bin target
`flash`: Flashes to chip using esptool.py
`monitor`: Serial monitor via cu
`clean`: Removes files from build directory
`erase-flash`: Clears flash using esptool.py
`disass`: Disassemble .elf to assembly using objdump

## API

The API is unstable and is not intended to be reliable in any way.

### syscalls

#### sys_write(char *bytes_to_send, int len)

User requests kernel to write `len` characters from `bytes_to_send` to stdout. This will be extended to arbitrary file descriptors for a unix-like interface.

#### sys_task_create(void *fn)

Create a statically allocated task from function pointer `fn`. Adds it to the array of tasks and will be run when the cooperative round-robin scheduler reaches the task. Returns false if there are no available task slots. Task is allocated 1kB of stack.

#### sys_task_delete(void)

Removes the currently running task from the array of tasks and yields to the scheduler. Any task that ends must call task_delete - Task functions cannot return.

#### sys_task_yield(void)

Yields the currently running task to the scheduler and places it in READY state. Task will be run again as soon as the scheduler reaches that task again. sys_task_yield or sys_task_delay must be called by some task at least once per second to prevent the watchdog timer from expiring.

#### sys_task_delay(uint64_t delay_us)

Yields the currently running task to the scheduler and will not run again until at least `delay_us` have elapsed. If this is the only running task, the scheduler will enter a busy loop waiting for the timer to elapse. Since the scheduler is cooperative, timing cannot be guaranteed.

### stdlib-like functions - `stdio.h`

The following functions are available in userspace

#### `int putchar(char byte_to_send)`

Write a character to stdout. Currently uses the USB driver on the ESP32-C3. Returns the number of characters sent, or -1 for error.

#### `int puts(char *bytes_to_send)`

Write a null-terminated cstring to stdout, followed by carriage return and newline. Currently uses the USB driver on the ESP32-C3. Returns the number of characters sent, or -1 for error.

### Multitasking guide

Usercode goes in `user` directory and must start at a function named `user_main(void)`. A user can define up to 4 tasks (including user_main), to be placed in a round-robin cooperative scheduler. It is the user's responsibility to call `sys_task_create` on any tasks that are required to run. It is the user's responsibility to ensure tasks yield control to the scheduler at least once per second by using `sys_task_yield()` or `sys_task_delay(us)`. Any tasks that terminate will need to call `sys_task_delete()` - that is, tasks cannot return.

## Memory layout

### ESP32-C3
The SRAM memory is accessed via both the instruction bus and data bus. The buses use different address spaces, offset by `0x70_0000`. Instructions (IRAM segment) extend from the address `0x4038_0000` to (in this case) `0x4038_8000`. Data (DRAM segment) extends from (in this case) `0x3FC8_8000`. The kernel reserves the first 64kB of memory and userspace reserves the next 64kB.

DRAM is laid out as follows:

```
/**
 * DRAM Memory Layout
 *
 * 0x3fc80000 ------------------> 0x40380000 = iram_seg_start
 *            |               |
 *            |               |   0. kernel .text section
 *            |               |
 * 0x3fc88000 ------------------> _sdata = dram_seg_start
 *            |               |
 *            |               |   1. kernel .data section
 *            |               |
 * _sdata+len(.data) -----------> _edata
 *            |               |
 *            |               |   2. kernel .bss section (incl. task_stacks)
 *            |               |
 * _edata+len(.bss) ------------> _ebss, _kstack_start
 *            |               |
 *            |               |   3. kernel stack grows from _end
 *            |               |
 * _ebss+0x1000 -----------------> _kstack_end
 *            |               |
 *            |               |   4. kernel heap (FUTURE)
 *            |               |
 * 0x3fc90000 ------------------> _kernel_end = 0x40390000 = user_iram_seg_start
 *            |               |
 *            |               |   5. user .text section
 *            |               |
 * 0x3fc98000 ------------------> user_dram_seg
 *            |               |
 *            |               |   6. user data, bss section
 *            |               |
 * 0x3fce0000 ------------------> _data_end_interface
 */
```

## The name

The name 'tibicen' has been chosen for the Australian Magpie (Gymnorhina tibicen). It is an Australian native bird present in all states and territories and has adapted well to the urban environment. The Australian Magpie tends to be gregarious, curious, intelligent, territorial and lazy, and a small percentage of adult males are known for swooping humans in breeding season. Birds of this species have rich, complex calls that are well known to all Australians. Many wild birds choose to interact with humans with behaviours ranging from foraging freshly-turned soil in gardens, to thieving food from patrons in outdoor seating areas of restaurants and cafes.

