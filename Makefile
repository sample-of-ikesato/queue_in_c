a.out: main.c queue.c
	gcc -Wall -g -o $@ $+

clean:
	rm -f a.out
