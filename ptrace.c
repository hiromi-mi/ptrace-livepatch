#include <stdio.h>
#include <errno.h>
#include <sys/ptrace.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, const char* argv[]) {
        pid_t target_pid;
        target_pid = atoi(argv[1]);
        void* addr = (void*)strtoll(argv[2], NULL, 0);
        void* to = (void*)strtoll(argv[3], NULL, 0);

        if (ptrace(PTRACE_ATTACH, target_pid, NULL, NULL) < 0) {
                perror("ptrace attach");
                exit(1);
        }

        wait(NULL);

        if (ptrace(PTRACE_POKEDATA, target_pid, addr, to) < 0) {
                perror("ptrace pokedata");
        }

        if (ptrace(PTRACE_DETACH, target_pid, NULL, NULL) < 0) {
                perror("ptrace detach");
                exit(1);
        }
        return 0;
}