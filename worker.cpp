/**
 * Michael Cueno 
 * UIN: 676808583
 * Contact: mcueno2@uic.edu
 *
 * This file contains the worker program launched via the master program. For more description see worker.h
 */

#include "worker.h"

/** Command line arguments: workerID sleepTime msgID shmID [ semID ] workerID: Unique identifier starting at 1 and increasing. Given by parent whom forked this worker
* nBuffers: TODO NOT SURE WHAT THIS DOES 
* sleepTime: time the workers will spend speeping in each work cycle, generated by parent randomly, given in descending order
* msgID/shmID/semID: id for message queue, shared memory block, and semaphore block respectively
*/
int main(int argc, char** argv){
										// Initializations 
	int read1;							// For storing read operations 
	int read2; 
	int read3; 
	char content[CONT_MAX];				// Buf for writing to message queue
	int workerID; int nBuffers; int sleepTime; int msgID; int shmID; int semID;

	parse_input(argc, argv, &workerID, &nBuffers, &sleepTime, &msgID, &shmID, &semID);

	int signature = 1 << (workerID-1);			// Bit that the worker adds to the buffer 
	int offset; 								// For indexing purposes 

	struct sembuf wait, signal;					// Semaphore operation buffer
	wait.sem_num = 0;     					    // This will be set to the current buffer (0 - nBuffers) 
	wait.sem_op = -1;      						// Wait until sem_value == 1 (if sem_value == 0 then we wait)
	wait.sem_flg = 0;

	signal.sem_num = 0;    
	signal.sem_op = 1;   						// Increment sem_value by 1
	signal.sem_flg = 0;

	// Send start up message 
	sprintf(content, "Worker %d starting up", workerID);
	write_to_msg(msgID, content);

	// Attach local address space for shared memory 
	int *mem = (int*)shmat(shmID, NULL, 0);
	if(*mem == -1){
		fprintf(stderr, "shmat failed: %s", strerror(errno)); 
	}

	// Begin read/write loop 
	for(int i = 0; i < (3*nBuffers); i++){
		// Get the buffer offset for accessing the shared memory segment
		offset = (i * workerID) % nBuffers;   						// offset could be named buffer_number

		if(semID != -1){											// If -lock specified 
			int sem_val = semctl(semID, offset, GETVAL, 0);			// get the value of the semaphore at offset
			wait.sem_num = offset; 									// Set the semaphore to operate on 
			semop(semID, &wait, 1); 								// Wait for that semaphore 
		}

		if(i%3==0 || i%3==1){   								    // First and second read operation 
			read1 = *(mem + offset);								// read operation using pointer arithmetic 
			usleep(sleepTime); 										
			read2 = *(mem + offset);
			error_check(read1, read2, msgID, offset, workerID);
		}else{														// Write operation 
			read3 = *(mem + offset); 
			usleep(sleepTime);
			*(mem + offset) = read3 + signature; 
		}
		if(semID != -1){											// Signal appropriate semaphore 
			signal.sem_num = offset;
			semop(semID, &signal, 1); 
		}
	}

	// Send worker finished messege
	sprintf(content, "Worker %d done", workerID);
	write_to_msg(msgID, content);
	return 0;
}

void parse_input(int argc, char** argv, int* workerId, int* nBuffers, int* sleepTime, int* msgID, int* shmID, int* semID){
	if(argc < 6 || argc > 7){
		fprintf(stderr, "%s", "input error in worker! Usage: workerID nBuffers sleepTime msgID shmID [ semID ]\n");
		exit(0);
	}
	*workerId = atoi(argv[1]);
	*nBuffers = atoi(argv[2]);
	*sleepTime = atoi(argv[3]); 
	*msgID = atoi(argv[4]);
	*shmID = atoi(argv[5]);
	if(argc == 7){
		*semID = atoi(argv[6]); 
	}
}

bool write_to_msg(int msgID, char* buf){
	struct msgcontent msgp; 
	strncpy(msgp.content, buf, CONT_MAX);
	int flags = 0; 
	if(msgsnd(msgID, &msgp, sizeof(msgp), flags )<0){
		//fprintf(stderr, "Message send failed with error: %s\n", strerror(errno)); 
		perror("Message Send failure: ");
		exit(-1); 
	} 
}

void error_check(int read1, int read2, int msgID, int offset, int workerID){
	char buf[CONT_MAX]; 
	int result; 							// Will hold the bitwise difference 
	int	mask = 1;			 				// For getting the least significant bit
	int is_offender; 						// For determining what worker messed with the buffer in a race condition

	result = read1 ^ read2;  		 		// Bitwise or 
	if(result != 0){						// If result == 0 then buffer had no write errors 
		int j = 0;
		while(result){						// DANGER! Infinite loop if msb == 1
			if(j!=0)result = result >> 1;	// Shift over (Get next worker) (Dont shift if this is the first iteration)
			is_offender = mask & result; 	// Lets get the lsb
			if(is_offender){
				sprintf(buf, "RACE COND READ ERROR: Worker %d in buffer %d. initial: %d | final: %d | corrupted by worker %d",
														 workerID,    offset,       read1,    read2,    	    j+1);
				write_to_msg(msgID, buf);	 
			}
			j++;
		}
	}
}