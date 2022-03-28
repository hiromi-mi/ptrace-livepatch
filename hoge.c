#include <stdio.h>
#include <sys/ptrace.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Program A

static int func2() __attribute__((noinline,noclone));
static int func2() {
   puts("func2");
   system("echo");
   return 2000;
}

int main()
{
  char *x = "Running Program A....";
        long long n = 12;
        while (1) {
                n++;
                sleep(1);
                func2();
                printf("%s\n", x);
        }
        return 0;
}
