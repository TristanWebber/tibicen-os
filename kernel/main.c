#include "timer.h"
#include "usb.h"

#include <stdint.h>

extern uint32_t _sbss, _ebss;

static void main_clear_bss(void) {
    volatile uint32_t *this_word = &_sbss;
    while (this_word < &_ebss) {
        *this_word = 0;
        this_word++;
    }
}

int main(void) {

    main_clear_bss();
    init_wdt(1000);

    int delay = 1000000;
    while (delay--) {}

    usb_print("\r\n");
    usb_print("Hello from tibicen-os!\r\n");
    usb_print("\r\n");
    usb_print("Goodbye from tibicen-os!\r\n");
    usb_print("\r\n");

    while(1) {
        feed_wdt();
    }

    return 0;
}
