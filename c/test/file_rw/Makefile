.PHONY:clean

OBJECTS=write.o read.o
all:write read

write:write.o
	gcc -o write write.o
write.o:write.c 
	gcc -c write.c 
read:read.o
	gcc -o read read.o
read.o:read.c
	gcc -c read.c 
clean:
	rm  $(OBJECTS) write read
