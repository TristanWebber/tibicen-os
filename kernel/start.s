.section .entry
.global _start

_start:
    # Setup 4kB stack for C
    la sp, _kstack_end

    # Clear registers of any values used by ROM bootloader
    li gp, 0
    li tp, 0
    li t0, 0
    li t1, 0
    li t2, 0
    li fp, 0
    li a0, 0
    li a1, 0
    li a2, 0
    li a3, 0
    li a4, 0
    li a5, 0
    li a6, 0
    li a7, 0
    li s1, 0
    li s2, 0
    li s3, 0
    li s4, 0
    li s5, 0
    li s6, 0
    li s7, 0
    li s8, 0
    li s9, 0
    li s10, 0
    li s11, 0
    li t3, 0
    li t4, 0
    li t5, 0
    li t6, 0

    # Only use hart 0 (ESP32C3 is single core)
    csrr a0, mhartid
    bnez a0, spin

    # Load default vector table
    la t0, _vector_table
    ori t0, t0, 1
    csrrw zero, mtvec, t0

    # Jump to user main
    j main

spin:
    j spin
