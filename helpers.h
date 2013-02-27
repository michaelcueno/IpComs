/**
 * Copyright: Michael Cueno @ 2013  (mcueno2)
 *
 * This is my implementation of a basic linux shell program. It was assigned
 * as a project for my Operating Systems class at UIC. This file contains the
 * prototypes of all functions used in the program (in files statsh.cpp, helpers.cpp)
 * This program uses a Makefile to comile the executable statsh
 *
 * Contact: mcueno2@uic.edu
 */
#ifndef _STATSH_H
#define _STATSH_H
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/time.h>
#include <vector>
#include <fcntl.h>
using namespace std;

/* Helper defines for remembering which constant is read/write */
#define IN 0   // read end
#define OUT 1  // write end

/** Takes in an argument vector (array of char pointers) and execs argv[0] with arguments argv
 * Returns pid of forked proccess or -1 if failed 
 *   @param in: the file descriptor that the command should use as it's input
 */
int launch_command(int in, int out, char** argv);

/** Waits for the specifed pid and adds the usage stats to hist */
void wait_for(int pid, char* name ); 

/* check for statsh built-in commands like stats, or exit. Returns true if built in fuction was entered */
bool built_in_command(char* line);

/* Helper test method for printing the arg array */
void print_char(char** x);

#endif
