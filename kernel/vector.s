# All 32 entries jump to same label
.balign 0x100
.global _vector_table
_vector_table:
    .option push
    .option norvc
    j trap_handler
    .rept 31
    j _interrupt_handler
    .endr
    .option pop
    .size _vector_table, .-_vector_table

# Do nothing
.global _interrupt_handler
_interrupt_handler:
    # Machine exception program counter stores the calling instruction. Adding
    # one word to the mepc returns to the following instruction
    csrr a0, mepc
    addi a0, a0, 4
    csrw mepc, a0
    mret
