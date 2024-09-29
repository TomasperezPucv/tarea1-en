CC=gcc
CFLAGS= -O2 -Wall

all: server client broker

server: server.c
client: client.c
broker: broker.c

clean: 
	rm  server client broker


.PHONY: all clean
