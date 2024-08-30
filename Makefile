all: wish

clean: 
	rm -f wish

wish: wish.c
	gcc -o wish wish.c execute.c parse.c command.c -Wall