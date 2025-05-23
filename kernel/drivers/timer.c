#include "timer.h"
#include "riscv.h"
#include <stdint.h>

/////////////////////////////////////////
// Functions for timers                //
/////////////////////////////////////////

// Set TIMG0 as digital single stage WDT with timeout_ms timer
void timer_init_wdt(uint32_t timeout_ms) {

    // Disable write protect
    REG_RW(TIMER_GROUP_0, 0x0064) = 0x50D83AA1;

    // Disable WDT and WDT stages (clear bits 23 to 31)
    REG_RW(TIMER_GROUP_0, 0x48) &= ~(0x1FF << 23);
    // Force register update (set bit 22)
    REG_RW(TIMER_GROUP_0, 0x48) |= BIT(22);

    // Set values
    // Increase reset signal pulses to max duration (set bits 15 to 20)
    REG_RW(TIMER_GROUP_0, 0x48) |= (0x3F << 0xF);

    // Set clock source as XTAL (set bit 21)
    REG_RW(TIMER_GROUP_0, 0x48) |= BIT(21);

    // Enable the clock (set bit 29)
    REG_RW(TIMER_GROUP_0, 0xFC) |= BIT(29);

    // Set the Stage 0 WDT to reset system after timeout (set bits 29 and 30)
    REG_RW(TIMER_GROUP_0, 0x48) |= (3 << 29);

    // Set the prescaler
    REG_RW(TIMER_GROUP_0, 0x4C) |= (MWDT_PRESCALER << 16);

    // Set the timeout value to timeout_ms in MWDT ticks
    REG_RW(TIMER_GROUP_0, 0x50) = (timeout_ms * 1000 / MWDT_TICKS_PER_US);

    // Enable the MWDT timer (set bit 31)
    REG_RW(TIMER_GROUP_0, 0x48) |= BIT(31);

    // Force register update (set bit 22)
    REG_RW(TIMER_GROUP_0, 0x48) |= BIT(22);

    // Feed WDT (reset counter and stage to 0)
    REG_RW(TIMER_GROUP_0, 0x0060) = 1;

    // Enable write protect
    REG_RW(TIMER_GROUP_0, 0x0064) = 1;
}

// Feed TIMG0 WDT
void timer_feed_wdt(void) {
    // Disable write protect
    REG_RW(TIMER_GROUP_0, 0x0064) = 0x50D83AA1;
    // Feed WDT
    REG_RW(TIMER_GROUP_0, 0x0060) = 1;
    // Enable write protect
    REG_RW(TIMER_GROUP_0, 0x0064) = 1;
}

// Get system ticks from system timer
uint64_t timer_systick(void) {
    // Request systicks (TRM 10.5)
    REG_RW(SYSTEM_TIMER, 0x4) = BIT(30);
    // Wait for systicks to be available
    while((REG_RW(SYSTEM_TIMER, 0x4) & BIT(29)) == 0) {
        asm volatile("nop");
    }
    // Upper bits from offset 0x0040, lower bits from 0x0044
    return ((uint64_t) REG_RW(SYSTEM_TIMER, 0x40) << 32) | REG_RW(SYSTEM_TIMER, 0x44);
}

// Convert 16MHz systicks to microseconds
uint64_t timer_uptime_us(void) {
    return timer_systick() >> 4;
}
