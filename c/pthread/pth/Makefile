.PHONY: clean 

all:server client 

server:server.c 
	gcc -o server server.c  -lm -pthread
client:client.c 
	gcc -o client client.c  -lm  -pthread

clean:
	rm server client
