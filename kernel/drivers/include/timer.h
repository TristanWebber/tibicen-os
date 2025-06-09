#ifndef TIMER_H
#define TIMER_H

#include "kstdio.h"

#include <stdint.h>

#define TIMER_GROUP_0 0x6001F000
#define TIMER_GROUP_1 0x60020000
#define SYSTEM_TIMER  0x60023000

// Todo move all interrupt functionality to separate file
#define INTERRUPT     0x600C2000
#define INTERRUPT_CORE0_SYSTIMER_TARGET0_INT_MAP_REG 0x0094
#define INTERRUPT_CORE0_CPU_INT_ENABLE_REG 0x0104
#define INTERRUPT_CORE0_CPU_INT_PRI_1_REG  0x0118
#define INTERRUPT_CORE0_INTR_STATUS_1_REG  0x00FC

#define SYSTIMER_CONF_REG           0x0000
#define SYSTIMER_UNIT0_OP_REG       0x0004
#define SYSTIMER_TARGET0_CONF_REG   0x0034
#define SYSTIMER_UNIT0_VALUE_HI_REG 0x0040
#define SYSTIMER_UNIT0_VALUE_LO_REG 0x0044
#define SYSTIMER_COMP0_LOAD_REG     0x0050
#define SYSTIMER_INT_ENA_REG        0x0064
#define SYSTIMER_INT_CLR_REG        0x006C

#define MWDT_TICKS_PER_US 500
#define MWDT_PRESCALER    20000

// Set TIMG0 as digital single stage WDT with timeout_ms timer
void timer_init_wdt(uint32_t timeout_ms);

// Feed TIMG0 WDT
void timer_feed_wdt(void);

// Convert 16MHz systicks to microseconds
uint64_t timer_uptime_us(void);

// Initialise systick interrupts
void timer_systick_init(void);

#endif /* TIMER_H */
