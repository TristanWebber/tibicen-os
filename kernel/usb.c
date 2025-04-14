#include "usb.h"
#include <stdint.h>

/////////////////////////////////////////
// Functions for USB interface         //
/////////////////////////////////////////

// Check if the USB serial fifo buffer is full
bool usb_fifo_full() {
    return (REG_RW(USB_SERIAL, USB_SERIAL_JTAG_EP1_CONF_REG) & BIT(1)) == 0;
}

// Force a flush of the USB serial fifo buffer
void usb_fifo_flush() {
    REG_RW(USB_SERIAL, USB_SERIAL_JTAG_EP1_CONF_REG) |= BIT(0);
}

// Wait for the usb fifo buffer to drain (blocking)
bool usb_wait_for_flush() {
    uint32_t timeout_iterations = 10000;
    while (timeout_iterations--) {
        if (!usb_fifo_full()) {
            return true;
        }
    }
    return false;
}

// Push byte onto the usb serial fifo buffer
void usb_write_byte(uint8_t byte_to_send) {
    REG_RW(USB_SERIAL, USB_SERIAL_JTAG_EP1_REG) = byte_to_send;
}

// Print a cstring to the USB serial device
int usb_print(char *bytes_to_send) {
    int res = -1;
    // Wait for buffer to be ready
    if (!usb_wait_for_flush()) {
        return res;
    }

    // Write each byte to the buffer
    for (int i = 0; bytes_to_send[i] != '\0'; i++) {
        if (usb_fifo_full()) {
            usb_fifo_flush();

            if (!usb_wait_for_flush()) {
                return res;
            }
        }
        usb_write_byte(bytes_to_send[i]);
        res = i;
    }

    // Force a flush to ensure all characters are processed
    usb_fifo_flush();
    return res;
}

// Print the binary representation of a 32bit value
void usb_print_reg_bits(uint32_t reg_val) {
    usb_print("Register values: 0b");
    char reg_char[33] = {'1'};
    reg_char[32] = '\0';
    for(uint32_t i = 32; i > 0; i--) {
        reg_char[i - 1] = '0' + (uint8_t)(reg_val & 0x1);
        reg_val >>= 1;
    }
    usb_print(reg_char);
    usb_print("\r\n");
}
