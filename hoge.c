#include <stdio.h>
#include <sys/ptrace.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
static int func2() __attribute__((noinline,noclone));
static int func2() {
   //printf("This is func2: arg c*2 is: %d\n", c*2);
   puts("func2");
   system("echo");
   return 2000;
}

int main()
{
//  printf("func2: %p\n",func2);
  char *x = "hello, world! My name is hiromi_mi.";
        long long n = 12;
        while (1) {
                n++;
                sleep(1);
                printf("%p:%s: %llx, %d\n", x, x, n, func2());
        }
        return 0;
}
