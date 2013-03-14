/**
  * Master program for homework assignment 2
  * Copyright: Michael Cueno @ 2013 (mcueno2)
  *   */
#ifndef _MASTER_H
#define _MASTER_H
  
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
#include "globals.h"

using namespace std;

#define SHM_BUF_MAX 40; 	// size of char array at each buffer in shared memory 

/** Checks the input for the correct command line arguments. If argv is anything other than 
	expected, the program exits in this function. Also checks if there is a randSeed and lock
	argument passed in and sets params randSeed and lock accordingly */
void error_check_and_parse(int argc, char** argv, int* randSeed, bool* lock );

/** Part one of assignment. Creates two pipes, forks off a child and execs sort -nr and links the 
	pipes up accordingly (setting up comunication from parent to sort). Then creates random numbers 
	and passes them into the sort processes one by one. After all numbers are passed into the 
	process, the write end that feeds to the process is closed, thus allowing sort to fininish.
	Sort then returns its results via the second pipe to the parent. */
void fill_rand_sorted_ints(int *nums, int count, int randSeed, int sleepMin, int sleepMax);
   
/** Instantiates a message queue and returns the message ID obtained from msgget().
	!Important: Must destroy_message_queue(msgID) for any message queue created with this 
	method. Other wise the queue will remain in the system, potentailly using all resources */
int create_message_queue(); 

/** Wrapper around shmget() that adds error checking. After this call, a shared memory segment of 
	size @param size will be created. Metod returns id to shared memory location. */
int create_shared_memory(int size); 

/** Allocates local adress space for the shared memory. Sets ptr to the the start of the shared memory. 
	We have three pointers because ptr is a pointer to a char* array of buffers. */
void init_shared_memory(int** ptr, int shmid, int nBuffers); 

/* Destroys the message queue identified by @param msgID. If error, it reports and exits */
void clean_up(int msgID, int shmID, int semID, bool lock); 

/* Creates the child processes and fucks everything up */
void fork_workers(int* pids, int nBuffers, int count, int* sleepTime, int msgID, int shmID, int semID);

/* Reads messages from @param msgID */
void read_messages(int msgID);

/** Takes in an argument vector (array of char pointers) and execs argv[0] with arguments argv
 * Returns pid of forked proccess or -1 if failed 
 *   @param in: the file descriptor that the command should use as it's input
 */
int launch_worker(int workerID, int nBuffers, int sleepTime, int msgID, int shmID, int semID);

/** Waits for the specifed pids, the size of the pid array is specified by count */
void wait_for(int* pid, int count); 

/* Helper test method for printing the arg array */
void print_char(char** x);

/* Runs through shared memory pointed to by mem, and prints out contents upto nBuffers
	(treats memory as ints) */
void print_memory(int* mem, int nBuffers);

/* Checks memory for errors and reports offenders */
void inspect_memory(int* mem, int nBuffers, int nWorkers);

#endif 
    
