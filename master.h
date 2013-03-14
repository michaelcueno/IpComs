/**
 * Michael Cueno 
 * UIN: 676808583
 * Contact: mcueno2@uic.edu
 *
 * This is the header for the master program, the main program for Homework 2 in operating systems at UIC. 

 * The purpose of this program is to illustrate how race conditions affect shared memory buffers and how 
 *  they can be circumvented with the use of semaphores. 
 *
 * The program makes full use of inter-process communications and this program allocates a message queue, 
 * 	a segment of shared memory and semephores in the kernel. 
 *
 * See the .cpp for command line arguments and descriptions 
 *
 */
#ifndef _MASTER_H
#define _MASTER_H
  
#include "globals.h"

using namespace std;

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

int init_semaphores(int nBuffers);
   
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

/** Destroys the message queue identified by @param msgID. If error, it reports and exits */
void clean_up(int msgID, int shmID, int semID, bool lock); 

/** Forks off the worker processes feeding them the corresponding command line arguments.
	Each worker process pid is stored in the @param pids array. */
void fork_workers(int* pids, int nBuffers, int count, int* sleepTime, int msgID, int shmID, int semID);

/* Reads all messages from @param msgID and prints to stdout */
void read_messages(int msgID);

/** Takes in an argument vector (array of char pointers) and execs argv[0] with arguments argv
 * Returns pid of forked proccess or -1 if failed 
 *   @param in: the file descriptor that the command should use as it's input
 */
int launch_worker(int workerID, int nBuffers, int sleepTime, int msgID, int shmID, int semID);

/** Waits for the specifed pids, the size of the pid array is specified by count */
void wait_for(int* pids, int count); 

/* Runs through shared memory pointed to by mem, and prints out contents upto nBuffers
	(treats memory as ints) */
void print_memory(int* mem, int nBuffers);

/* Checks memory for errors and reports offenders */
void inspect_memory(int* mem, int nBuffers, int nWorkers);

#endif 
    
