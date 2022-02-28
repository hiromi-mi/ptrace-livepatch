all:
	rm -f ptrace
	gcc -Wall -Wextra ~/ptrace/ptrace.c -o ~/ptrace/ptrace
	sudo setcap CAP_SYS_PTRACE+ep ~/ptrace/ptrace
	gcc -Wall -Wextra ~/ptrace/rewrite.c -o ~/ptrace/rewrite
	sudo setcap CAP_SYS_PTRACE+ep ~/ptrace/rewrite
	gcc -g -Wall -Wextra hoge.c -o hoge
	gcc -g -Wall -Wextra lib.c -o lib
	#musl-gcc -g -Wall -Wextra lib.c -o lib_musl -static

test:
	./test.sh
