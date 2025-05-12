#ifndef KSTDIO_H
#define KSTDIO_H

// Write a string, followed by a newline, to stdout.
int kputs(char *bytes_to_send);

// Write a character to stdout.
int kputchar(char byte_to_send);

// Write formatted output to stdout (supports %c, %s, %i, %d, %u, %x, %p, %o)
int kprintf(const char *fstring, ...);

#endif /* KSTDIO_H */
