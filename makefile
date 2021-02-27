CC=gcc
CFLAGS=-std=gnu89 -g -Wall

all:	linkedlist.o battleship_client battleship_server

linkedlist.o: linkedlist.c linkedlist.h
	$(CC) $(CFLAGS) -c -o linkedlist.o linkedlist.c

battleship_client:	battleship_client.c
	$(CC) $(CFLAGS) -pthread -o battleship_client battleship_client.c

battleship_server:	battleship_server.c linkedlist.o linkedlist.h 
	$(CC) $(CFLAGS) -pthread -o battleship_server linkedlist.o battleship_server.c

test:
	./battleship_server &

	./battleship_client -i LUKS -j -x 3 -y 9
	./battleship_client -i JOHN -j -x 6 -y 10

	./battleship_client -i LUKS -b -x 3 -y 1
	./battleship_client -i JOHN -b -x 4 -y 9

	./battleship_client -i MARK -j -x 4 -y 3

	./battleship_client -i MARK -b -x 5 -y 10

	./battleship_client -i LUKS -b -x 4 -y 1
	./battleship_client -i MARK -b -x 2 -y 9
	./battleship_client -i JOHN -b -x 3 -y 9
	./battleship_client -i MARK -b -x 6 -y 1
	./battleship_client -i JOHN -b -x 4 -y 3
	killall -s SIGINT battleship_server 2>/dev/null || true
	cat ./battleship_server.log

clean:
	rm -f *.o all
	rm -f battleship_client
	rm -f battleship_server
	rm -f srv_socket
	rm -f cli_socket_*
	rm -f battleship_server.log
