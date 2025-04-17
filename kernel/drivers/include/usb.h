#ifndef USB_H
#define USB_H

#include <stdbool.h>
#include <stdint.h>

#define MAX_IRQ           32
#define MWDT_TICKS_PER_US 500
#define MWDT_PRESCALER    20000

// Register base addresses
#define GPIO          0x60004000
#define LOW_POWER_MGT 0x60008000
#define IO_MUX        0x60009000
#define TIMER_GROUP_0 0x6001F000
#define TIMER_GROUP_1 0x60020000
#define SYSTEM_TIMER  0x60023000
#define USB_SERIAL    0x60043000
#define INTERRUPT     0x600C2000

// USB register offsets
#define USB_SERIAL_JTAG_EP1_REG      0x0000
#define USB_SERIAL_JTAG_EP1_CONF_REG 0x0004

// Gpio numbers
#define GPIO_NUM_2 2
#define GPIO_NUM_3 3

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
