all: server client

ifdef OSX
CC      = gcc
CCFLAGS = -Wall
LIBS    = -lrt

else
CC      = gcc
CCFLAGS = -Wall
LIBS    = -lrt
endif

server: server.c shm.o
	$(CC) $(CCFLAGS) $^ $(LIBS) -o $@

client: client.c shm.o
	$(CC) $(CCFLAGS) $^ $(LIBS) -o $@

shm.o: shm.c shm.h	
	$(CC) $(CCFLAGS) $(LIBS) -c $<

clean:
	$(RM) *.out


#make -B
