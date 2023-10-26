CC=g++

CFLAGS=$(shell pkg-config --cflags opencv)
LIBS=$(shell pkg-config --libs opencv)

OBJS= main.o TASK3.o SIMPLESOCKET.o
DEMOTARGET=main server client

client.o:	client.cpp
	$(CC) -c $<  -std=c++11

server.o:	server.cpp
	$(CC) -c $<  -std=c++11

SIMPLESOCKET.o:	SIMPLESOCKET.cpp
	$(CC) -c $<  -std=c++11

TASK3.o:	TASK3.cpp
	$(CC) -c $<  -std=c++11

main.o:	main.cpp
	$(CC) -c $<  -std=c++11



main:	$(OBJS)
	$(CC) -o $@ $^ -L/usr/lib/x86_64-linux-gnu -ldl -lstdc++  -std=c++11 -lpthread $(LIBS)

server:	server.o
	$(CC) -o server server.o TASK3.o SIMPLESOCKET.o -L/usr/lib/x86_64-linux-gnu -ldl -lstdc++  -std=c++11

client:	client.o
	$(CC) -o client client.o TASK3.o SIMPLESOCKET.o -L/usr/lib/x86_64-linux-gnu -ldl -lstdc++  -std=c++11

clean:
	-rm -r -f   $(DEMOTARGET) *.o DOXYGENDOC  *.txt

doc:
	doxygen Doxyfile


all:	$(DEMOTARGET)
	make clean  && make main && make server && make client

run:	main
	./main

