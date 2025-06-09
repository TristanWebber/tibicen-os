#include "usb.h"

// Write a string, followed by a newline, to stdout.
int kputs(char *bytes_to_send) {
    int bytes_written = usb_print(bytes_to_send);
    bytes_written += usb_print("\r\n");
    usb_fifo_flush();
    return bytes_written;
}
