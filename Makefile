CFLAGS=-Wall -g

all:
	cc -Wall -g *.c -o game

clean:
	rm -f game

