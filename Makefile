CC=gcc
LIBS=-lncurses
FLAGS=-O2 -std=c99 -pedantic

all:
	${CC} binclock.c -o binclock ${LIBS} ${FLAGS}

install:
	cp binclock /usr/bin -v
