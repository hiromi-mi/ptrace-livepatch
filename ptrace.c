// http://0xcc.net/blog/archives/000077.html

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/user.h>

const long long load_addr = 0x555555520000;

int set_data(pid_t target_pid, void* addr, void* to);
int set_data_2(pid_t target_pid, void* addr, char* to, size_t len);
void* target_alloc(pid_t target_pid, long size);

int read_file(pid_t target_pid, char* filename) {
   FILE *fp;
   struct stat st;
   
   if (stat(filename, &st) < 0) {
      perror("stat");
      return 0;
   }

   fp = fopen(filename, "r");
   if (fp == NULL) {
      perror("file open");
      return 0;
   }

   char data[100000];
   if (fread(data, st.st_size, 1, fp) == 0) {
      perror("fread");
   }

   fclose(fp);

   void* addr = target_alloc(target_pid, (long long)st.st_size);
   printf("%p %ld addr\n", addr, st.st_size);
   if (addr == 0) {
      return 0;
   }
   
   if (set_data_2(target_pid, addr, data, st.st_size) < 0) {
   }
   return 0;

   //set_memvar(

}

int stack_push(pid_t target_pid, struct user_regs_struct *regs, void* v) {
   // update stack
   regs->rsp -= 8;

   if (ptrace(PTRACE_POKEDATA, target_pid, regs->rsp, v) < 0) {
      perror("stack_push ptrace pokedata stack_push");
   }
   return 0;
}

int set_data_2(pid_t target_pid, void* addr, char* to, size_t len) {
   size_t x = len / sizeof(void *);
   size_t mod = len % sizeof(void *);
   long long lv;

      if (set_data(target_pid, (void*)load_addr, (void*)20) < 0) {
         puts("error");
         return -1;
      }
   for (size_t i=0;i<x;i ++) {
      memcpy(&lv, &to[i*8], 8);
      printf("addr: %p, %llx,data: %lx -> %llx log\n", addr+i, load_addr + i*8, ptrace(PTRACE_PEEKDATA, target_pid, addr+i, 0), lv);
      if (set_data(target_pid,(void*)( load_addr + i*8), (void*)lv) < 0) {
         return -1;
      }
   }
   if (mod > 0) {
      lv = ptrace(PTRACE_PEEKDATA, target_pid, addr+x, 0);
      memcpy(&lv, &to[x*8], mod);
      set_data(target_pid, addr+x, (void*)lv);
   }

   return 0;
}
int set_data(pid_t target_pid, void* addr, void* to) {
   //printf("%lx log\n", ptrace(PTRACE_PEEKDATA, target_pid, addr, 0));

   if (ptrace(PTRACE_POKEDATA, target_pid, addr, to) < 0) {
      perror("set_data ptrace pokedata set_data");
      return -1;
   }

   return 0;
}

// func:
// jmp <func_new>
// func_new:
// jmp rel32 0xe9 <rel32>
// jmp rel64; rel32 の範囲で収まりきらないjmp
// rax に8bitを入れて 間接jumpをすればいい

int set_jump(pid_t target_pid, void* addr, void* to) {
   char jmpbuf[8] = {0xe9, 0, 0, 0, 0, 0, 0, 0};
   int jump_relative;
   jmpbuf[0] = 0xe9; // jmp rel32
   jump_relative = to - addr + 5; // 5 = jmp + rel32
   memcpy(jmpbuf+1, &jump_relative, sizeof(int));
   set_data(target_pid, addr, to);

   return 0;
}

void* target_alloc(pid_t target_pid, long size) {
   struct user_regs_struct regs, original_regs;


   //memcpy(&lv, code, 8);
   void* lv = (void*)0x050f;

   if (ptrace(PTRACE_GETREGS, target_pid, NULL, &original_regs) < 0) {
      perror("ptrace getregs");
      return 0;
   }
   regs = original_regs;
   // regs.rsp -= sizeof(size_t); // ????


   // rsp, rsp+1, rsp+2 
//   regs.rip = regs.rsp; // 0x80
 //  raddr = regs.rsp + 2;
   // fd = -1
   // option: read, write, not exec
   // map: why?
   // anonymous -> offset:0, fd: -1
   // call mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)
   // on TARGET process
   /*
   stack_push(target_pid, &regs, 0);
   stack_push(target_pid, &regs, -1);
   stack_push(target_pid, &regs, MAP_PRIVATE | MAP_ANONYMOUS);
   stack_push(target_pid, &regs, PROT_READ | PROT_WRITE);
   stack_push(target_pid, &regs, size);
   stack_push(target_pid, &regs, NULL);
   */
   regs.rax = 9;
   regs.rdi = load_addr;
   regs.rsi = size;
   regs.rdx = PROT_READ | PROT_WRITE | PROT_EXEC;
   regs.r10 = MAP_PRIVATE | MAP_ANONYMOUS;
   regs.r8 = -1;
   regs.r9 = 0;


   // firstt argument of 0x80
   //regs.rax = SYS_mmap; // system call number

   //regs.rax = SYS_mmap; // system call number
  /*
  // write system call
   regs.rax = 1;
   regs.rdi = 1;
   regs.rsi = 0x5555555592a0;
   regs.rdx = 6;
   */

   puts("get registers part 0");
   if (ptrace(PTRACE_SETREGS, target_pid, NULL, &regs) < 0) {
      perror("ptrace setregs");
      return 0;
   }

   void* raddr = ptrace(PTRACE_PEEKDATA, target_pid, regs.rip, NULL);

   if (ptrace(PTRACE_POKEDATA, target_pid, regs.rip, lv) < 0) {
      perror("ptrace poke code");
      return 0;
   }
   if (ptrace(PTRACE_SINGLESTEP, target_pid, NULL,NULL) < 0) {
      perror("ptrace cont");
      return 0;
   }
   puts("get registers part 1");
   wait(NULL);
   // TODO MAP_FAILED check
   if (ptrace(PTRACE_GETREGS, target_pid, NULL, &regs) < 0) {
      perror("ptrace getregs");
      return 0;
   }
   puts("get registers part 2");
   if ((void*)(regs.rax) == MAP_FAILED) {
      puts("mmap failed");
      return 0;
   }
   if (ptrace(PTRACE_SETREGS, target_pid, NULL, &original_regs) < 0) {
      perror("ptrace setregs (restore)");
      return 0;
   }
   puts("get registers part 3");
   if (ptrace(PTRACE_POKEDATA, target_pid, original_regs.rip, raddr) < 0) {
      perror("ptrace pokedata (restore)");
      return 0;
   }

   return (void*)regs.rax;
}

int rewrite_func(pid_t target_pid,void* from_addr, void* to_addr) {
   int jmp_relative = (long long)to_addr - (long long)from_addr  -5;
   // write を試す
   char code[8];
   //code[0] = 0x90;
   code[0] = 0xe8;
   printf("jmp_relative: %d\n", jmp_relative);
   memcpy(code+1, &jmp_relative, sizeof(int));
      code[7] = 0x90;
      code[6] = 0x90;
      code[5] = 0xc3;
      for (int i =0;i<8;i++) {
         printf("%d\n", code[i]);
      }
   size_t ptr;
   memcpy(&ptr, code, 8);
   printf("%lx -> %lx\n", ptrace(PTRACE_PEEKDATA, target_pid, from_addr, 0), ptr);
   ptrace(PTRACE_POKEDATA, target_pid, from_addr, ptr);
}

int main(int argc, const char* argv[]) {

   if (argc != 4) {
      fputs("Usage: ./ptrace pid addr to\n", stderr);
      return 1;
   }
   pid_t target_pid;
   target_pid = atoi(argv[1]);
   void* addr = (void*)strtoll(argv[2], NULL, 0);
// data
   void* to = (void*)strtoll(argv[3], NULL, 0);

   if (ptrace(PTRACE_ATTACH, target_pid, NULL, NULL) < 0) {
      perror("ptrace attach");
      exit(1);
   }

   wait(NULL);

   set_data(target_pid, addr, to);

   // func1: bf
   // func2: a4
   // func4: 89
   read_file(target_pid, "/home/hiromimi/ptrace/lib");
   //rewrite_func(target_pid, 0x5555555551a4, 0x555555555189);
   //rewrite_func(target_pid, 0x55555555519b, 0x555555555169);

   //rewrite_func(target_pid, 0x555555520000 + 0x10fd, 0x5555555551c4);
   // .a
   //rewrite_func(target_pid, 0x5555555551a4, 0x555555520000 + 0x40);
   
   // .so
   rewrite_func(target_pid, 0x5555555551bb, load_addr + 0x00001149);

   if (ptrace(PTRACE_CONT, target_pid, NULL, NULL) < 0) {
      perror("ptrace continue (restore)");
      return 0;
   }
   /*
   if (ptrace(PTRACE_DETACH, target_pid, NULL, NULL) < 0) {
      perror("ptrace detach");
      exit(1);
   }
   */
   return 0;
}
