#include "usb.h"

// Write a character to stdout.
int kputchar(char byte_to_send) {

    int res = -1;
    // Wait for buffer to be ready
    if (!usb_wait_for_flush()) {
        return res;
    }

    // Ensure there is room in fifo buffer
    if (usb_fifo_full()) {
        usb_fifo_flush();

        if (!usb_wait_for_flush()) {
            return res;
        }
    }

    usb_write_byte(byte_to_send);
    return 1;
}
