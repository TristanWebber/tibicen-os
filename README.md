# tibicen-os

Multi-tasking monolithic OS kernel for low-cost RISC-V hardware. Created for educational purposes. Initial implementation uses a ESP32-C3 microcontroller.

## Vision

This project has been created primarily for purposes of personal development. The kernel is intended to be run on real, low-cost hardware rather than a virtual machine. The following features will be implemented, in roughly this order:

### Short-term
- Print to a host terminal
- Multitasking
- Hardware interrupts
- Userspace and syscalls
- Memory protection
- Crontab-like scheduler for user tasks
- IO drivers and userspace SDK
- Heap memory
- Basic file system
- Sharing data between threads

### Mid-term
- Extend to other hardware
- Multicore
- Paging
- Virtual memory

## Hardware

There's something magical about bringing up a physical device. The standard practice of developing educational OS kernels for use on an emulator is irrefutably an excellent way to fearlessly test and iterate bare metal software but particularly in the early stages, it doesn't 'feel' very bare-metal. My approach to this project has been to start with a very low cost device that allows for all testing to occur on a physical device, and is unlikely to be too upsetting if it ends up bricked in the process. Based on the cost and features, I have started with the Espressif ESP32-C3 microcontroller. This device has the feature set and power to implement all of the short-term goals of this project on something that is readily available for less than $10.

A reality of educational kernels is that they start small and very rarely grow to the point where they need genuinely substantial processing power, memory or storage. I don't anticipate that this kernel will outgrow the features of the ESP32-C3, however there are some kernel features I would like to implement that require features that this particular microcontroller doesn't have. To implement paging, virtual memory and support of multiple cores, the project will ultimately need to be ported from a microcontroller to a microprocessor. This will use either a Milk-V Duo or Sipeed LicheeRV Nano, both of which use the Sophgo SG2002 SoC with two RISC-V cores and are available for less than $30.

## Tools

Development has been done on a Linux machine with a gcc cross-compiler toolchain. ESP32-C3 development uses the ESP-IDF toolchain because it provides a straightforward way to get a RISC-V cross-compiler toolchain.

## Building

Makefile commands will be documented here.

## API

Userspace API will be documented here.

## The name

The name 'tibicen' has been chosen for the Australian Magpie (Gymnorhina tibicen). It is an Australian native bird present in all states and territories and has adapted well to the urban environment. The Australian Magpie tends to be gregarious, curious, intelligent, territorial and lazy, and a small percentage of adult males are known for swooping humans in breeding season. Birds of this species have rich, complex calls that are well known to all Australians. Many wild birds choose to interact with humans with behaviours ranging from foraging freshly-turned soil in gardens, to thieving food from patrons in outdoor seating areas of restaurants and cafes.
