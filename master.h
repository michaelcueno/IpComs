/**
  * Master program for homework assignment 2
  * Copyright: Michael Cueno @ 2013 (mcueno2)
  *   */
#ifndef _MASTER_H
#define _MASTER_H
  
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Readability Defines */
#define READ 0
#define WRITE 1
#define CHILD 0

/** Checks the input for the correct command line arguments. If argv is anything other than 
	expected, the program exits in this function. Also checks if there is a randSeed and lock
	argument passed in and sets params randSeed and lock accordingly */
void error_check_and_parse(int argc, char** argv, int* randSeed, bool* lock );

/** Part one of assignment. Creates two pipes, forks off a child and execs sort -nr and links the 
	pipes up accordingly (setting up comunication from parent to sort). Then creates random numbers 
	and passes them into the sort processes one by one. After all numbers are passed into the 
	process, the write end that feeds to the process is closed, thus allowing sort to fininish.
	Sort then returns its results via the second pipe to the parent. */
void fill_rand_sorted_ints(int *nums, int count, int randSeed);
   
int create_message_queue(); 

int* fork_workers();

#endif 
    
