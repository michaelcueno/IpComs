/** 
  * Master delcarations file 
  * Copyright: Michael Cueno @ 2013 (mcueno)
  */
#include "master.h"
#include "helpers.h"


/** command line args: nBuffers nWorkers sleepMin sleepMax [randSeed] [-lock | -nolock] 
	nBuffers: must be positive prime! 
	nWorkers: number of child processes that exec worker, must be < nBuffers
	sleepMin/sleepMax: positive reals, max > min, define range of sleep times for worker processes
	randSeed: optional, seeds random num generator, if not specified, generator supplied with time(NULL)
	-lock/-nolock: lock turns on semaphores, default is -nolock (w/out semaphors)
	*/

int main(int argc, char** argv){
	part_one(argc, argv);
}

void part_one(int argc, char** argv){

	error_check_input(argc, argv);

	int out = dup(WRITE); 

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
	dup2(pipe1[WRITE], WRITE); dup2(pipe2[READ], READ);
	close(pipe1[WRITE]); close(pipe2[READ]);

	for(int i=0; i < atoi(argv[2]); i++){
		printf("%d\n", rand());
	}

    dup2(out, WRITE);

}


void error_check_input(int argc, char** argv){
	if(argc > 7 || argc < 4){
		printf("%s", "Usage: nBuffers nWorkers sleepMin sleepMax [randSeed] [-lock|-nolock]\n");
		exit(0);
	}
	if(atoi(argv[3]) >= atoi(argv[4])){
		printf("%s", "sleepMin must be strictly less than sleepMax\n");
		exit(0);
	}
}
