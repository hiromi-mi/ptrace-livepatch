#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

static int func3() __attribute__((noinline,noclone));
static int func3() {
//   write(1, "func3\n", 5);
   puts("func3! injected code!!!");
   return 300;
}

int main() {
   func3();
   return 0;
}
