
# stop ASLR

sysctl -w kernel.randomize_va_space=0


gcc -Wall -Wextra ~/ptrace/ptrace.c -o ~/ptrace/ptrace && sudo setcap CAP_SYS_PTRACE+ep ~/ptrace/ptrace
