###############################################################################
# Author: Julia Nelson
# Date: June 17, 2021
# Pledge: "I pledge my honor that I have abided by the Stevens Honor System."
# Description: Makefile for mtsieve.c
################################################################################ 
CC     = gcc
C_FILE = $(wildcard *.c)
TARGET = $(patsubst %.c,%,$(C_FILE))
CFLAGS = -g -Wall -Werror -pedantic-errors -pthread

all:
	$(CC) $(CFLAGS) $(C_FILE) -o $(TARGET) -lm 
clean:
	rm -f $(TARGET) $(TARGET).exe
