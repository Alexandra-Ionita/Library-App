CC=gcc
CFLAGS=-I.

client: client.c requests.c helpers.c buffer.c command_handler.c parson.c
	$(CC) -o client client.c requests.c helpers.c buffer.c command_handler.c parson.c -Wall

run: client
	./client

clean:
	rm -f *.o client
