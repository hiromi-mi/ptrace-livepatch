all:
	rm -f ptrace
	gcc -Wall -Wextra ptrace.c -o ptrace
	sudo setcap CAP_SYS_PTRACE+ep ptrace
	gcc -Wall -Wextra rewrite.c -o rewrite
	sudo setcap CAP_SYS_PTRACE+ep rewrite
	gcc -g -Wall -Wextra hoge.c -o hoge
	gcc -g -Wall -Wextra lib.c -o B
	gcc -g -Wall -Wextra lib.c -o lib 
	gcc -g -Wall -Wextra hoge.c -o A

test:
	./test.sh
