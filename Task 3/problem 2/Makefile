all: server client

ifdef OSX
CC      = gcc
CCFLAGS = -Wall
LIBS    = 

else
CC      = gcc
CCFLAGS = -Wall
LIBS    = -lrt -lpthread
endif

server: server.c shm.h shm.o sem.h sem.o
	$(CC) $(CCFLAGS) shm.o sem.o -o $@ $(LIBS) $<

client: client.c shm.h shm.o sem.h sem.o
	$(CC) $(CCFLAGS) shm.o sem.o -o $@ $(LIBS) $<

shm.o: shm.c	
	$(CC) $(CCFLAGS) -c $(LIBS) $<

sem.o: sem.c	
	$(CC) $(CCFLAGS) -c $(LIBS) $<

clean:
	$(RM) *.o
