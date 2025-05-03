compile: main.out

main.out: main.c
	gcc -g -Wall -pedantic-errors main.c input.c -o run_TM.out

clean:
	rm -f *.out