#include "stdio.h"

#include <stdbool.h>
#include <stdarg.h>
#include <wchar.h>

// Print up to 32bit signed decimal integers
int printint(int d) {
    int BASE = 10;
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

// Handles only '%s' and '%d'
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
                // Print all characters in a string
                case 's': {
                    char *s = va_arg(ap, char*);
                    while (*s) {
                        res += putchar(*s++);
                    }
                    break;
                }
                // Print signed ints up to 32bit
                case 'd': {
                    int d = va_arg(ap, int);
                    res += printint(d);
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

    return res;
}
