/** 
  * Master delcarations file 
  * Copyright: Michael Cueno @ 2013 (mcueno)
  */
#include "master.h"
#include "helpers.h"

void error_check_input(int argc, char** input); 

/** command line args: nBuffers nWorkers sleepMin sleepMax [randSeed] [-lock | -nolock] 
	nBuffers: must be positive prime! 
	nWorkers: number of child processes that exec worker, must be < nBuffers
	sleepMin/sleepMax: positive reals, max > min, define range of sleep times for worker processes
	randSeed: optional, seeds random num generator, if not specified, generator supplied with time(NULL)
	-lock/-nolock: lock turns on semaphores, default is -nolock (w/out semaphors)
	*/
int main(int argc, char** argv){

	error_check_input(argc, argv);

	// Create two pipes, pipe1 and pipe2
	int pipe1[2]; int pipe2[2];
 	if( (pipe(pipe1)<0) || (pipe(pipe2)<0) ){ fprintf(stderr, "Could not create pipe!\n"); exit(0); }

	// fork off a child 
	int pid; 
	if( (pid = fork()) < 0 ) { fprintf(stderr, "Could not fork child!\n"); exit(0); }

	// close respective file descriptors and dup2 the pipe ends 
	if( pid==0 ){  

		// Set up pipes such that we read from the parent via pipe1 and write to the parent via pipe2
		close(pipe1[WRITE]); close(pipe2[READ]);
		dup2(pipe1[READ], READ); dup2(pipe2[WRITE], WRITE);
		close(pipe1[READ]); close(pipe2[WRITE]);

		// Child executes 'sort -nr' 
		if(execlp( "sort", "sort", "-nr", NULL )){ 
			fprintf(stderr, "Child should not have returned! Something went wrong in execvp\n"); exit(0); 
		}

	}

	close(pipe1[READ]); close(pipe2[WRITE]);  // Close pipe1[write] to finish child process

	int seed = (argv[5])? atoi(argv[5]):0;  // TODO: should set to time(NULL) instead of 0...? whatever that means
	// Seed random number generator, srand()
	srand(seed);

	int out = dup(WRITE);
	int i;
	for(i=0; i < atoi(argv[2]); i++){
		int rnd = rand();
		int to_write = rnd;
		printf("%d\n", rnd);
		// Write to child (sort)
		if(write(pipe1[WRITE], &to_write, sizeof(RAND_MAX))<0){fprintf(stderr, "Error in write");}
	}
	close(pipe1[WRITE]);

	// parent generates nWorkers (argv[2]) numbers between sleepMin/sleepMax

	// print all numbers 

	// passes them to the child 

	// Close pipe ends when done writing to child 

	// read back output from child (should be numbers in sorted order) and print 

}

/* For testing purposes */
void print_char(char** x){
  printf("%s", "NOW PRINTING CHARACTER ARRAY:") ;
  for(int i = 0 ; x[i]; i++){
    printf("%s\n", x[i]);
  }
}

void error_check_input(int argc, char** argv){
	printf("%d", argc);
	if(argc > 7 || argc < 4){
		printf("%s", "Usage: nBuffers nWorkers sleepMin sleepMax [randSeed] [-lock|-nolock]\n");
		exit(0);
	}
	if(atoi(argv[3]) >= atoi(argv[4])){
		printf("%s", "sleepMin must be strictly less than sleepMax\n");
		exit(0);
	}
}
