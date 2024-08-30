all: wish

clean: 
	rm -f wish

wish: wish.c
	gcc -o wish wish.c -Wall