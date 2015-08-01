.PHONY:clean 

objects = object.o 
object:$(objects) 
	gcc -o object $(objects)
object.o:object.c 

clean:
	rm object $(objects)
