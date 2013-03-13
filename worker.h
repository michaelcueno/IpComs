/**
  * Worker program for homework assignment 2
  * Copyright: Michael Cueno @ 2013 (mcueno2)
  *   */
#ifndef _WORKER_H
#define _WORKER_H
  
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include "globals.h"

/** Sets variables and checks for errors in input */
void parse_input(int argc, char** argv, int* workerId, int* nBuffers, int* sleepTime, int* msgID, int* shmID, int* semID);

/** Sends a message (contained in buf) to the message queue pointed at by msgID */
bool write_to_msg(int msgID, char* buf); 

#endif 
    
