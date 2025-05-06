#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

// Absolute maximum addressable value. Far exceeds physically reachable values
#define MEMORY_END 0xFFFFFFFF

// Memory regions defined in link.ld
// User and kernel bss
extern uint32_t _sbss, _ebss, _subss, _eubss;
// User and kernel memory regions for PMP
extern uint32_t _sudata, _eudram, _eiram, _euiram;

// Incomplete specification - Only including members for the values that are
// currently in use
typedef enum pmp_config {
    TOR_PROTECTED = 0x8,
    TOR_RWX = 0xF,
} pmp_config_t;

// Clear the kernel bss region
void mem_clear_bss(void);

// Setup physical memory protection
void mem_init(void);

#endif /* MEMORY_H */
