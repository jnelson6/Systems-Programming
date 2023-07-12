# Author: Julia Nelson
# Date: June 17, 2021
# Pledge: "I pledge my honor that I have abided by the Stevens Honor System."
# Description: Lab 9 - threads


CC     = gcc
C_FILE = $(wildcard *.c)
TARGET = $(patsubst %.c,%,$(C_FILE))
CFLAGS = -g -Wall -Werror -pedantic-errors -pthread

all:
	$(CC) $(CFLAGS) $(C_FILE) -o $(TARGET) 
clean:
	rm -f $(TARGET) $(TARGET).exe
