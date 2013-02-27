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

/* Readability Defines */
#define READ 0
#define WRITE 1
#define CHILD 0

void parse_input(int argc, char** argv, int* workerId, int* sleepTime, int* msgID, int* shmID, int* semID);

#endif 
    
