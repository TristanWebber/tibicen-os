/* ## Linkfile for ESP32-C3 ## */

/* SRAM1 contains general purpose memory addressable by both the instruction bus (as iram) and the data bus (as dram). The address space of the two buses is offset by 0x70_0000, but they are in the same memory, so care must be taken to prevent the iram and dram colliding */

/* SRAM1 can be accessed by instruction bus (0x4038_000 to 0x403C_AE00) */
iram_seg_start = 0x40380000;
/* SRAM1 can be accessed by data bus (0x3FC8_000 to 0x3FCC_AE00) */
dram_seg_start = 0x3FC80000;

/* User code can be placed beyond kernel dram */
user_iram_seg_start = 0x40390000;
user_dram_seg_start = 0x3FC90000;

/* dram immediately follows iram, so offset the start of the dram space by LENGTH(iram_seg)*/

MEMORY
{
  iram_seg (RWX) :                  org = iram_seg_start, len = 32k
  dram_seg (RW) :                   org = dram_seg_start + LENGTH(iram_seg), len = 32k
  user_iram_seg (RWX) :             org = user_iram_seg_start, len = 32k
  user_dram_seg (RW) :              org = user_dram_seg_start + LENGTH(user_iram_seg), len = 32k
}

ENTRY(_start)

SECTIONS {
  .text : {
    KEEP(*(.entry))
    . = ALIGN(16);
    *(.text)
    *(.text*)
  } > iram_seg

  .data : {
    . = ALIGN(16);
    _sdata = .;
    *(.data)
    *(.data*)
    *(.sdata)
    *(.sdata*)
    *(.srodata)
    *(.srodata*)
    *(.rodata)
    *(.rodata*)
    *(.gnu.linkonce.r.*)
    *(.rodata1)
    *(.riscv.*)
    . = ALIGN(16);
    _edata = .;
  } > dram_seg

  .bss (NOLOAD): {
    . = ALIGN(16);
    _sbss = .;
    *(.bss)
    *(.bss*)
    *(.sbss)
    *(.sbss*)
    *(COMMON)
    . = ALIGN(16);
    _ebss = .;
  } > dram_seg

  /* Set a stack for the kernel immediately following bss */
  .kstack (NOLOAD) : {
    . = ALIGN(16);
    /* Provide 1024 bytes to the scheduler */
    _kstack_start = .;
    . = . + 1024;
    _kstack_end = .;
    ppstack = .;
    /* Provide 1024 bytes in the kernel for each user task */
    . = . + 4096;
    _ppstack_end = .;
  } > dram_seg

  .user_text : {
    . = ALIGN(16);
    *(.user_text)
    *(.user_text*)
  } > user_iram_seg

  .user_data : {
    . = ALIGN(16);
    _sudata = .;
    *(.user_data)
    *(.user_data*)
    *(.user_sdata)
    *(.user_sdata*)
    *(.user_srodata)
    *(.user_srodata*)
    *(.user_rodata)
    *(.user_rodata*)
    *(.user_gnu.linkonce.r.*)
    *(.user_rodata1)
    *(.user_riscv.*)
  } > user_dram_seg

  .user_bss (NOLOAD): {
    . = ALIGN(16);
    _subss = .;
    *(.bss)
    *(.bss*)
    *(.sbss)
    *(.sbss*)
    *(COMMON)
    . = ALIGN(16);
    _eubss = .;
  } > user_dram_seg

  /* Set a stack for tasks immediately following ubss */
  .ustack (NOLOAD) : {
    . = ALIGN(16);
    task_stacks = .;
    . = . + 4096;
    _ustack_end = .;
  } > user_dram_seg

  PROVIDE(end = .);
  PROVIDE(_end = .);
  PROVIDE(_eiram = iram_seg_start + LENGTH(iram_seg));
  PROVIDE(_euiram = user_iram_seg_start + LENGTH(user_iram_seg));
  PROVIDE(_eudram = _euiram + LENGTH(user_dram_seg));
}
