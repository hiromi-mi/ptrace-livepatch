#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
static double func5(double a) __attribute__((noinline,noclone));
static double func5(double a) {
   return a + 1;
   
}


static int func3() __attribute__((noinline,noclone));
static int func3() {
   //write(1, "func3\n", 5);
   puts("func3! injected code!!!");
   system("ls");
   return func5(300);
}

int main() {
   func3();
   return 0;
}
