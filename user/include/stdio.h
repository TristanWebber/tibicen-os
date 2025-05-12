#ifndef STDIO_H
#define STDIO_H

// Write a string, followed by a newline, to stdout.
int puts(char *bytes_to_send);

// Write a character to stdout.
int putchar(char byte_to_send);

// Write formatted output to stdout (supports %c, %s, %i, %d, %u, %x, %p, %o)
int printf(const char *fstring, ...);

#endif /* STDIO_H */
