#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>

// Program B

static int func3_internal() __attribute__((noinline,noclone));
int func3_internal() {
   return 200;
}

static int func3() __attribute__((noinline,noclone));
static int func3() {
   puts("func3! injected code!!!");
   system("ls");
   return func3_internal();
}

int main() {
   func3();
   return 0;
}
