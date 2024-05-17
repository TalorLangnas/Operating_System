all: client server 

server: server.o
	gcc server.o -o server -pthread

client: client.o
	gcc client.o -o client -pthread

server.o: server.c
	gcc -c server.c -o server.o -pthread

client.o: client.c
	gcc -c client.c -o client.o -pthread

.PHONY: all clean

clean: 
	rm -f *.o *.a *.so client server