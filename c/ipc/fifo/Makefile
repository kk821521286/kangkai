
.PHONY:clean

objects=fifos.o fifoc.o

all:fifos fifoc

fifos:fifos.c fifo_cs.h 
	gcc -o fifos fifos.c
fifoc:fifoc.c fifo_cs.h
	gcc -o fifoc fifoc.c

clean:
	rm $(objects) fifos fifoc
