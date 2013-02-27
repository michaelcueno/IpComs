# Makefile for ipcoms

CC=g++
MV=/bin/mv
RM=/bin/rm
CP=/bin/cp

LIBRARIES=  

CFLAGS=-g -Wall

MASTER=master

WORKER=worker

WOBJECTS=worker.o helpers.o

MOBJECTS=master.o helpers.o

WSOURCES=worker.cpp helpers.cpp

MSOURCES=master.cpp helpers.cpp 

all: $(MASTER) $(WORKER)

$(MASTER): $(MOBJECTS)
	$(CC) $(CFLAGS) -o $(MASTER) $(MOBJECTS) $(LIBRARIES)

$(WORKER): $(WOBJECTS)
	$(CC) $(CFLAGS) -o $(WORKER) $(WOBJECTS) $(LIBRARIES)

clean:
	$(RM) $(MASTER) $(WORKER) $(MOBJECTS) $(WOBJECTS) 

