
.PHONY:clean

objects= main.o sem.o 
ldflags= -lm 

main:main.o sem.o 
	gcc -o main  $(ldflags) main.o sem.o
main.o:main.c sem.h head.h
sem.o:sem.c sem.h head.h
	gcc -c sem.c
clean:
	rm $(objects) main
