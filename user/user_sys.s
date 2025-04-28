# Write n bytes from char *b to stdout.
#
# int sys_write(char *b, int n);
.global sys_write
sys_write:
    li a7, 1
    ecall
    ret
