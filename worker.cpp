/**
  * Worker declaration file 
  * copyright: Michael Cueno @ 2013 (mcueno2)
  */

#include "worker.h"

/** Command line arguments: workerID sleepTime msgID shmID [ semID ] workerID: Unique identifier starting at 1 and increasing. Given by parent whom forked this worker
* nBuffers: TODO NOT SURE WHAT THIS DOES 
* sleepTime: time the workers will spend speeping in each work cycle, generated by parent randomly, given in descending order
* msgID/shmID/semID: id for message queue, shared memory block, and semaphore block respectively
*/
int main(int argc, char** argv){
	// Variables 
	int read1;
	int read2; 

	// Initializations 
	int workerID; int nBuffers; int sleepTime; int msgID; int shmID; int semID;
	parse_input(argc, argv, &workerID, &nBuffers, &sleepTime, &msgID, &shmID, &semID);
	int offset = sizeof(int)*(workerID-1);
	int signature = 1 << (workerID-1);

	// Part two 
	char content[CONT_MAX];

	// Send start up message 
	sprintf(content, "Worker %d starting up", workerID);
	write_to_msg(msgID, content);

	// Modify shared memory
	int *mem = (int*)shmat(shmID, NULL, 0);
	if(*mem == -1){
		fprintf(stderr, "shmat failed: %s", strerror(errno)); 
	}

	// Begin read/write loop 
	for(int i = 0; i < (3*nBuffers); i++){
		offset = (offset + i) % nBuffers; 
		// First read 
		read1 = *(mem + offset);

		usleep(sleepTime); 

		// Second Read
		read2 = *(mem + offset);

		error_check(read1, read2, msgID);

		// write operation 
		read1 = *(mem + offset); 
		*(mem + offset) = read1 + signature; 
	}
	printf("After loop\n", NULL);


	// Send finished messege
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

void error_check(int first, int second, int msgID){

	if(first != second){
		printf("Fucking we got a problem over here billy\n", NULL);
	}

}


