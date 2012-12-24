CC?= gcc
LIBS= -lncurses
FLAGS?= -O2 -std=c99 -pedantic -Wall

all:
	${CC} binclock.c -o binclock ${LIBS} ${FLAGS}

install:
	cp binclock /usr/bin -v
	cp binclock.1 /usr/share/man/man1/

uninstall:
	rm /usr/bin/binclock
	rm /usr/share/man/man1/binclock.1
