.PHONY:clean

OBJECTS= *.o

main: main.o  queue.o btree.o
	gcc -o main main.o btree.o queue.o -g 
main.o:main.c btree.h 
	gcc -c  main.c 

btree.o:btree.c btree.h 
	gcc -c  btree.c 

queue.o:queue.c queue.h btree.h
clean:
	rm $(OBJECTS) main
