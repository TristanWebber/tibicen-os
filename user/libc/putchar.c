#include "user.h"
#include <stdint.h>

// Write a character to stdout.
int putchar(char byte_to_send) {
    sys_write(&byte_to_send, 1);
    return 1;
}
