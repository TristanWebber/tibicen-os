#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

// Macros for normal register and DMA operations
#define BIT(x) ((uint32_t) 1U << (x))
#define REG_RW(base, offset) (*(volatile uint32_t *) ((base) + (offset)))

#define TIMER_GROUP_0 0x6001F000
#define TIMER_GROUP_1 0x60020000

#define MWDT_TICKS_PER_US 500
#define MWDT_PRESCALER    20000

// Set TIMG0 as digital single stage WDT with timeout_ms timer
void init_wdt(uint32_t timeout_ms);

// Feed TIMG0 WDT
void feed_wdt(void);

#endif /* TIMER_H */
