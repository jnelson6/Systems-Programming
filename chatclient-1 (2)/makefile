#*******************************************************************************
# * Name        : makefile
# * Author      : Julia Nelson
# * Date        : July 07, 2021
# * Description : makefile for chatclient.c, util.h
# * Pledge      : "I pledge my honor that I have abided by the Stevens Honor System."
# ******************************************************************************/

CC     = gcc
CFLAGS = -g -Wall -Werror -pedantic-errors

all: chatclient 
chatclient: chatclient.c util.h
	$(CC) $(CFLAGS) chatclient.c -o chatclient
util.o: util.h
	$(CC) $(CFLAGS) -c util.h
clean: 
	rm -f *.o *.a chatclient chatclient.exe