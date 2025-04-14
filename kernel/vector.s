# All 32 entries jump to same label
.balign 0x100
.global _vector_table
_vector_table:
    .option push
    .option norvc
    .rept 32
    j _interrupt_handler
    .endr
    .option pop
    .size _vector_table, .-_vector_table

# Do nothing
.global _interrupt_handler
_interrupt_handler:
    mret
