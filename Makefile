# Michael Cueno 
# Makefile for ipcoms

CC=g++
MV=/bin/mv
RM=/bin/rm
CP=/bin/cp

LIBRARIES=  

CFLAGS=-g -Wall

MASTER=master

WORKER=worker

WOBJECTS=worker.o 

MOBJECTS=master.o

WSOURCES=worker.cpp 

MSOURCES=master.cpp

all: $(MASTER) $(WORKER) globals.h

$(MASTER): $(MOBJECTS)
	$(CC) $(CFLAGS) -o $(MASTER) $(MOBJECTS) $(LIBRARIES)

$(WORKER): $(WOBJECTS)
	$(CC) $(CFLAGS) -o $(WORKER) $(WOBJECTS) $(LIBRARIES)

update: clean
	$(CP) * 

clean:
	$(RM) $(MASTER) $(WORKER) $(MOBJECTS) $(WOBJECTS) 

