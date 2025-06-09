#include "stdio.h"
#include "user.h"

// Write a string, followed by a newline, to stdout.
int puts(char *bytes_to_send) {

    int i = 0;
    while (bytes_to_send[i] != '\0') {
        i++;
    }

    int res = sys_write(bytes_to_send, i);
    res += putchar('\r');
    res += putchar('\n');
    sys_flush();
    return res;
}
