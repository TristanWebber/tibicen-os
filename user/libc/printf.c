#include "stdio.h"
#include "user.h"

#include <stdbool.h>
#include <stdarg.h>
#include <stdint.h>

const char *upper_intchars = "0123456789ABCDEF";
const char *lower_intchars = "0123456789abcdef";

typedef enum int_base {
    BASE_8 = 8,
    BASE_10 = 10,
    BASE_16 = 16,
} int_base_t;

// Print up to 32bit signed decimal integers
int printint(int d) {
    const int BASE = 10;
    int res = 0;
    char buf[10];

    // Print '-' sign
    if (d < 0) {
        res += putchar('-');
        d = -d;
    }

    // Set the characters from right to left
    int i = 0;
    do {
        buf[i++] = (d % BASE) + '0';
    } while ((d /= BASE) != 0);

    // Print nonzero values from the buffer, left to right
    while (--i >= 0) {
        res += putchar(buf[i]);
    }

    return res;
}

// Print up to 32bit unsigned integers as oct, dec or hex
int printuint(uint32_t uint_arg, int_base_t base) {
    int res = 0;
    char buf[11];

    // Set the characters from right to left
    int i = 0;
    do {
        buf[i++] = lower_intchars[uint_arg % base];
    } while ((uint_arg /= base) != 0);

    // Print nonzero values from the buffer, left to right
    while (--i >= 0) {
        res += putchar(buf[i]);
    }

    return res;
}

// Handles only '%c', '%s', '%d', '%u', %p, %x and %o
int printf(const char *fstring, ...) {
    va_list ap;
    va_start(ap, fstring);

    int res = 0;
    bool err = false;
    char err_code = '\0';

    while(*fstring) {
 
        // Handle format codes - initially only short
        if (*fstring == '%') {
            char fcode = *++fstring;
            switch(fcode) {
                // Print a single char
                case 'c': {
                    char c = va_arg(ap, int);
                    res += putchar(c);
                    break;
                }
                // Print all characters in a string
                case 's': {
                    char *s = va_arg(ap, char*);
                    while (*s) {
                        res += putchar(*s++);
                    }
                    break;
                }
                // Print signed ints as decimal up to 32bit
                case 'i':
                case 'd': {
                    int d = va_arg(ap, int);
                    res += printint(d);
                    break;
                }
                // Print unsigned ints as decimal up to 32bit
                case 'u': {
                    uint32_t d = va_arg(ap, uint32_t);
                    res += printuint(d, BASE_10);
                    break;
                }
                // Print unsigned ints (incl. *) as lowercase hex up to 32bit
                case 'p':
                case 'x': {
                    uint32_t d = va_arg(ap, uint32_t);
                    res += printuint(d, BASE_16);
                    break;
                }
                // Print unsigned ints as octal up to 32bit
                case 'o': {
                    uint32_t d = va_arg(ap, uint32_t);
                    res += printuint(d, BASE_8);
                    break;
                }
                // Double % to escape and print a %
                case '%':
                    res += putchar('%');
                    break;
                // Something went wrong -> Print the format code
                // This causes issues in the variadic args
                default:
                    res += putchar('%');
                    res += putchar(fcode);
                    err_code = fcode;
                    err = true;
                    break;
            }
        } else {
            res += putchar(*fstring);
        }

        // Advance to next character
        fstring++;
    }

    if (err) {
        char *err_msg = "0: ERROR Unexpected format code.";
        err_msg[0] = err_code;
        puts(err_msg);
    }

    va_end(ap);

    sys_flush();

    return res;
}
