#ifndef USB_H
#define USB_H

#include <stdbool.h>
#include <stdint.h>

// Register base addresses
#define USB_SERIAL    0x60043000

// USB register offsets
#define USB_SERIAL_JTAG_EP1_REG      0x0000
#define USB_SERIAL_JTAG_EP1_CONF_REG 0x0004

/////////////////////////////////////////
// Functions for USB interface         //
/////////////////////////////////////////

// Check if the USB serial fifo buffer is full
bool usb_fifo_full();

// Force a flush of the USB serial fifo buffer
void usb_fifo_flush();

// Wait for the usb fifo buffer to drain (blocking)
bool usb_wait_for_flush();

// Push byte onto the usb serial fifo buffer
void usb_write_byte(uint8_t byte_to_send);

// Print a cstring to the USB serial device
int usb_print(char *bytes_to_send);

// Print the binary representation of a 32bit value
void usb_print_reg_bits(uint32_t reg_val);

#endif /* USB_H */
