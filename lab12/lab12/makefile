###############################################################################
# Author: Julia Nelson
# Date: 06/24/2021
# Pledge: "I pledge my honor that I have abided by the Stevens Honor System."
# Description: makefile for lab12
###############################################################################
CC     = gcc
CFLAGS = -g -Wall -Werror -pedantic-errors

all: main libsum.so

# TODO: build main

main: main.c
	$(CC) $(CFLAGS) -o main main.c -ldl

# TODO: build libsum.so

libsum.so: sum.c sum.h
	$(CC) $(CFLAGS) -shared -fPIC -o libsum.so sum.c


clean:
	rm -f libsum.so main main.exe
