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
	int randSeed; 			// optional, seeds random num generator, if not specified, generator supplied with time(NULL)
	bool lock;     			// lock turns on semaphores, default is -nolock (w/out semaphors)
	error_check_and_parse(argc, argv, &randSeed, &lock);
	int nBuffers = atoi(argv[1]); 			// must be positive prime! 
	int nWorkers = atoi(argv[2]); 			// number of child processes that exec worker, must be < nBuffers
	int sleepMin = atoi(argv[3]); 			//
	int sleepMax = atoi(argv[4]); 			// positive reals, max > min, define range of sleep times for worker processes
	int rand_nums[nWorkers];

	fill_rand_sorted_ints(rand_nums, nWorkers, randSeed);

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
		close(pipe1[READ]); close(pipe2[WRITE]);    		// Close pipe1[write] to finish child process
		srand(randSeed); 									// Set random seed 
		for(int i=0; i < count; i++){
			fprintf(to_sort, "%d\n", rand());
		}
		fclose(to_sort);

		
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

}

int* fork_workers(){

}

































