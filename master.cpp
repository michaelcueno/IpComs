/** 
  * Master delcarations file 
  * Copyright: Michael Cueno @ 2013 (mcueno)
  */
#include "master.h"

/** command line args: nBuffers nWorkers sleepMin sleepMax [randSeed] [-lock | -nolock] 
	nBuffers: must be positive prime! 
	nWorkers: number of child processes that exec worker, must be < nBuffers
	sleepMin/sleepMax: positive reals, max > min, define range of sleep times for worker processes
	randSeed: optional, seeds random num generator, if not specified, generator supplied with time(NULL)
	-lock/-nolock: lock turns on semaphores, default is -nolock (w/out semaphors)
	*/

int main(int argc, char** argv){
	int randSeed; 			// optional, seeds random num generator, if not specified, generator supplied with time(NULL)
	bool lock;     							// lock turns on semaphores, default is -nolock (w/out semaphors)
	error_check_and_parse(argc, argv, &randSeed, &lock);
	int nBuffers = atoi(argv[1]); 			// must be positive prime! 
	int nWorkers = atoi(argv[2]); 			// number of child processes that exec worker, must be < nBuffers
	int sleepMin = atoi(argv[3]); 			//
	int sleepMax = atoi(argv[4]); 			// positive reals, max > min, define range of sleep times for worker processes
	int sleepTimes[nWorkers];
	int* mem = NULL;			 			// Array of ints for shared memory 

	fill_rand_sorted_ints(sleepTimes, nWorkers, randSeed);

	int msgID = create_message_queue();

	int shmID = create_shared_memory(nBuffers);
	init_shared_memory(&mem, shmID, nBuffers);

	int pids[nWorkers];
	fork_workers(pids, nBuffers, nWorkers, sleepTimes, msgID, shmID, 0); 

	wait_for(pids, nWorkers); 

	read_message(msgID, nWorkers*2);

	print_memory(mem, nBuffers);

	clean_up(msgID, shmID); 
}
	
void fill_rand_sorted_ints(int *nums, int count, int randSeed){
	// Create two pipes, pipe1 and pipe2
	int pipe1[2]; int pipe2[2];
 	if( (pipe(pipe1)<0) || (pipe(pipe2)<0) ){ fprintf(stderr, "Could not create pipe!\n"); exit(0); }

 	switch(fork()){
	case -1: 
		fprintf(stderr, "Could not fork child!\n"); exit(0);
	case CHILD:
		// Set up pipes such that we read from the parent via pipe1 and write to the parent via pipe2
		close(pipe1[WRITE]); close(pipe2[READ]);
		dup2(pipe1[READ], READ); dup2(pipe2[WRITE], WRITE);
		close(pipe1[READ]); close(pipe2[WRITE]);

		if(execlp( "sort", "sort", "-nr", NULL )){ 			// Child executes 'sort -nr' 
			fprintf(stderr, "Child should not have returned! Something went wrong in execvp\n"); 
			exit(0); 
		}
	default: 
		FILE* to_sort = fdopen(pipe1[WRITE], "w");			// Open pipe for writing 
		dup2(pipe2[READ], READ);
		close(pipe1[READ]); close(pipe2[WRITE]);    		// Close pipe1[write] to finish child process
		srand(randSeed); 									// Set random seed 
		for(int i=0; i < count; i++){
			fprintf(to_sort, "%d\n", rand());
		}
		fclose(to_sort);

		int size = (int)log10(INT_MAX)+3;   				// Number of digits in INT_MAX plus room for truncation + newline
		char buf[10];
		for(int i = 0; i<count; i++){
			fgets(buf, size, stdin);
			nums[i] = atoi(buf); 
		}
	} 
}

void error_check_and_parse(int argc, char** argv, int* randSeed, bool* lock ){
	// Defaults 
	*lock = false; 
	*randSeed = time(NULL); 

	// Bounds checking 
	if(argc > 7 || argc < 4){
		printf("%s", "Usage: nBuffers nWorkers sleepMin sleepMax [randSeed] [-lock|-nolock]\n");
		exit(0);
	}
	// Logic checking 
	if(atoi(argv[3]) >= atoi(argv[4])){
		printf("%s", "sleepMin must be strictly less than sleepMax\n");
		exit(0);
	}
	if(atoi(argv[1]) < atoi(argv[2])){
		printf("%s", "nBuffers must be larger than nWorkers!\n");
		exit(0);
	}
	// check for flag or randseed 
	if(argc==6){
		if(strcmp(argv[5], "-lock")==0)
			*lock = true;
		else if(strcmp(argv[5], "-nolock")==0)   // For completeness 
			*lock = false; 
		else{
			*randSeed = atoi(argv[5]); 			 // Doesn't check if argv[5] is numerical... Ok for now 
		}
	} else if(argc==7){
		if(strcmp(argv[6], "-lock")==0)
			*lock = true;
		else if(strcmp(argv[6], "-nolock")==0)
			*lock = false; 
		else {
			fprintf(stderr, "Incorrect Input."); exit(0); 
		}
	}
}

int create_message_queue(){
	int id = msgget( IPC_PRIVATE, 0600 | IPC_CREAT);
	if(id<0){
		fprintf(stderr, "Could not create message queue! %s\n", strerror( errno )); exit(0);
	}
	return id; 
}

int create_shared_memory(int nBuffers){
	int size = sizeof(int)*nBuffers;
	int flags = 0600 | IPC_CREAT;
	int id = shmget( IPC_PRIVATE, size, flags); 
	if(id<0){
		fprintf(stderr, "Could not create message queue! %s\n", strerror( errno )); exit(0);
	}
	return id; 
}

void init_shared_memory(int** mem, int shmID, int nBuffers){
	// Attach shared memory to local address space
	*mem = (int*)shmat(shmID, NULL, 0);
	if(*mem == (int* )(-1)){
		fprintf(stderr, "shmat failed: %s", strerror(errno)); 
	}
	/** Initialize the buf to zeros. This for loop works by using pointer arithmetic to 
		loop through and initialize the buffer */
	for(int i = 0; i < nBuffers; i++){
		* (*mem + (sizeof(int)*i)) = 0; 
	}
}

void clean_up(int msgID, int shmID){
	struct msqid_ds info;
	if(msgctl(msgID, IPC_RMID, &info)<0){
		fprintf(stderr, "Could not remove msg %d. errno: %s\n", msgID, strerror( errno )); 
		exit(-2); 
	}
	if(shmctl(shmID, IPC_RMID, NULL)){
		fprintf(stderr, "Could not remove msg %d. errno: %s\n", msgID, strerror( errno )); 
		exit(-3); 
	}
}

void fork_workers(int* pids, int nBuffers, int count, int* sleepTime, int msgID, int shmID, int semID){
	for(int i = 0; i < count; i++){
		pids[i] = launch_worker(i+1, nBuffers, sleepTime[i], msgID, shmID, semID);
	}
}

void read_message(int msgID, int n){
	struct msgcontent msgp;
	size_t size = sizeof(struct msgcontent); 
	long int type = 0;
	for(int i =0; i<n; i++){
		if(msgrcv(msgID, &msgp, size, type, IPC_NOWAIT)<0){
			perror("Couldn't read message: ");
			exit(-1); 
		}
		printf("Parent got: %s\n", msgp.content);
	}
}

int launch_worker(int workerID, int nBuffers, int sleepTime, int msgID, int shmID, int semID){
  /* Fork of a process and exec */
  int size = (int)log10(INT_MAX)+3;                   // Max size of int plus some padding for newlines 
  char arg1[size]; snprintf(arg1, size, "%d", workerID);
  char arg2[size]; snprintf(arg2, size, "%d", nBuffers);
  char arg3[size]; snprintf(arg3, size, "%d", sleepTime);
  char arg4[size]; snprintf(arg4, size, "%d", msgID);
  char arg5[size]; snprintf(arg5, size, "%d", shmID);
  char arg6[size]; snprintf(arg6, size, "%d", semID);

  int pid;
  if((pid = fork())<0){ printf("%s", "Couldn't fork"); }
  if(pid==0){                        
    if(execlp("./worker", "./worker", arg1, arg2, arg3, arg4, arg5, arg6, NULL )<0){
      printf("%s", "Error in exec()\n");
      exit(0);
    }
  }
  return pid;
}

void wait_for(int* pid, int count){
    for(int i=0; i<count; i++){
    int status;
    struct rusage usage;
    if(wait4(pid[i], &status, 0, &usage)<0)
      perror("Couldn't wait for child: "); 
  }
}

/* For testing purposes */
void print_char(char** x){
	printf("Now printing char array.. \n", NULL);
    for(int i = 0 ; x[i]; i++){
	    printf("Buffer %d: %s", i, x[i]);
  }
}

void print_memory(int* mem, int nBuffers){
	for(int i=0; i < nBuffers; i++){
		printf("Buffer %d: ", i, NULL);
		for(int j=0; j<sizeof(int); j++){
			printf("%d", mem[j + (i*sizeof(int))]);
		}
		printf("\n", NULL);
	}
}
