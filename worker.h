/**
 * Michael Cueno 
 * UIN: 676808583
 * Contact: mcueno2@uic.edu
 *
 * This is the header file for the worker program that should only be launched via the master program 
 * never called on it's own. It uses shared memory, message queues and semephores that are all 
 * allocated by the master program. 
 */

#ifndef _WORKER_H
#define _WORKER_H
  
#include "globals.h"

/** Sets variables and checks for errors in input */
void parse_input(int argc, char** argv, int* workerId, 
										int* nBuffers, 
										int* sleepTime, 
										int* msgID,
										int* shmID, 
										int* semID);

/** Sends a message (contained in buf) to the message queue pointed at by msgID */
bool write_to_msg(int msgID, char* buf); 

/** Used to check for read differences in the circular reading step 
	If an error is found (first != second) then this function writes a read error message to 
	the message queue specified by @param msgID */
void error_check(int read1, int read2, int msgID, int offset, int workerID);

#endif 
    
