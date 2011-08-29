CC=gcc
LIBS=-lncurses
FLAGS=-O2

all:
	${CC} binclock.c -o binclock ${LIBS} ${FLAGS}

install:
	cp binclock /usr/bin -v
