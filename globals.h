/**
 * Michael Cueno 
 * UIN: 676808583
 * Contact: mcueno2@uic.edu
 *
 * This file contains some global definitions that should prove to be useful throught the program
 * As well as the include statements that both the worker and the master programs need. 
 */

/* Readability Defines */
#define READ 0
#define WRITE 1
#define CHILD 0
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>
#include <limits.h> 
#include <time.h>
#include <sys/wait.h>

// Included from the handout 
#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED) 
/* union semun is defined by including <sys/sem.h> */ 
#else 
/* according to X/OPEN we have to define it ourselves */ 
union semun { 
      int val;                  /* value for SETVAL */ 
      struct semid_ds *buf;     /* buffer for IPC_STAT, IPC_SET */ 
      unsigned short *array;    /* array for GETALL, SETALL */ 
                                /* Linux specific part: */ 
      struct seminfo *__buf;    /* buffer for IPC_INFO */ 
}; 
#endif 

#define CONT_MAX 200   // Maximum content length for char arrays 

/* Holds the message buffer for reading/writing to the message queue */
struct msgcontent {
	long int mtype; 
	char content[CONT_MAX]; 
}; 
