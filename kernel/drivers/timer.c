#include "timer.h"
#include "riscv.h"
#include "task.h"

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
    REG_RW(SYSTEM_TIMER, SYSTIMER_UNIT0_OP_REG) = BIT(30);
    // Wait for systicks to be available
    while((REG_RW(SYSTEM_TIMER, SYSTIMER_UNIT0_OP_REG) & BIT(29)) == 0) {
        asm volatile("nop");
    }
    // Upper bits from HI_REG, lower bits from LO_REG
    return ((uint64_t) REG_RW(SYSTEM_TIMER, SYSTIMER_UNIT0_VALUE_HI_REG) << 32) | REG_RW(SYSTEM_TIMER, SYSTIMER_UNIT0_VALUE_LO_REG);
}

// Convert 16MHz systicks to microseconds
uint64_t timer_uptime_us(void) {
    return timer_systick() >> 4;
}

// Initialise systick interrupts
void timer_systick_init(void) {

    // Set period for alarm (1.6e4 = 1000Hz) and set to period mode
    REG_RW(SYSTEM_TIMER, SYSTIMER_TARGET0_CONF_REG) |= (BIT(30) | 16000);

    // Force COMP0 reload
    REG_RW(SYSTEM_TIMER, SYSTIMER_COMP0_LOAD_REG) = BIT(0);

    // Enable comparator COMP0
    REG_RW(SYSTEM_TIMER, SYSTIMER_CONF_REG) |= BIT(24);

    // Enable interrupts on TARGET0
    REG_RW(SYSTEM_TIMER, SYSTIMER_INT_ENA_REG) |= BIT(0);

    // Clear interrupt source
    REG_RW(SYSTEM_TIMER, SYSTIMER_INT_CLR_REG) |= BIT(0);

    // Save and clear global interrupt enable (bit 4)
    uint32_t prev_mstatus = CSR_READ(mstatus);
    CSR_WRITE(mstatus, prev_mstatus |= ~(0x8));

    // Allocate interrupt to slot 1 at priority 1
    REG_RW(INTERRUPT, INTERRUPT_CORE0_CPU_INT_ENABLE_REG) |= BIT(1);
    REG_RW(INTERRUPT, INTERRUPT_CORE0_CPU_INT_PRI_1_REG) |= 1;

    // Wait for pending write instructions to complete
    asm volatile ("fence");

    // Restore previous global interrupt state
    CSR_WRITE(mstatus, prev_mstatus);

    // Enable CPU IRQ for SYSTIMER TARGET0
    REG_RW(INTERRUPT, INTERRUPT_CORE0_INTR_STATUS_1_REG) |= BIT(6);

    // Map TARGET0 interrupt to int1
    REG_RW(INTERRUPT, INTERRUPT_CORE0_SYSTIMER_TARGET0_INT_MAP_REG) = 1;
}

// temp interrupt handler for systicks
// NOTE: interrupts do not fire if cpu is already in a handler
void systick_handler(void) {

    // Clear interrupt source
    REG_RW(SYSTEM_TIMER, SYSTIMER_INT_CLR_REG) |= BIT(0);

    // Wait for pending write instructions to complete
    asm volatile ("fence");

    // Yield to the scheduler
    task_yield();
}
