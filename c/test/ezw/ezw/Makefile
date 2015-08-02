.PHONY:clean

OBJECTS=ezw.o list.o matrix2d.o fifo.o
OBJECT=unezw.o list.o matrix2d.o fifo.o
all:ezw unezw
ezw:$(OBJECTS)
	gcc -o ezw $(OBJECTS) -lm
ezw.o:ezw.c list.h matrix2d.h ezw.h fifo.h
	gcc -c  ezw.c 
unezw:$(OBJECT)
	gcc -o unezw $(OBJECT) -lm
unezw.o:unezw.c list.h matrix2d.h ezw.h fifo.h
	gcc -c  unezw.c 
list.o:list.c list.h
	gcc -c  list.c 
matrix2d.o:matrix2d.c matrix2d.h
	gcc -c  matrix2d.c 
fifo.o:fifo.c fifo.h 
	gcc -c fifo.c 



clean:
	rm *.o ezw unezw 
