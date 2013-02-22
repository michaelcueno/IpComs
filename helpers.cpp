/**
 * Helper methods for the statsh program, for function descriptions see helpers.h 
 * Copyright @ Michael Cueno 2013 (mcueno2)
 */
#include "helpers.h"
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

bool built_in_command(char* input){
  if(strcmp(input, "exit")==0){
    exit(0);
  }
  if(strcmp(input, "stats")==0){
    return true;
  }
  return false;
}

char* read_line(char* prompt){
  printf("%s",prompt);
  static char* line = NULL;           // Needs to be freed in main!!
  size_t s = 0;
  getline(&line, &s, stdin);
  line[ strlen(line) - 1 ] = '\0';    // get rid of pesky newline character
  return line;
}

int launch_command(int in, int out, char** argv){
  /* Create the arg list for exec */
  dup2(in, IN);                       // Replace stdin with in
  dup2(out, OUT);                     // Replace stdout with out 
  close(in); close(out);

  /* Fork of a process and exec */
  int pid;
  if((pid = fork())<0){ printf("%s", "Couldn't fork"); }
  if(pid==0){                         // in child
    if(execvp(argv[0], argv)<0){
      printf("%s", "Error in exec()\n");
      exit(0);
    }
  }
  return pid;
}

void wait_for(int pid, char* name ){
  int status;
  struct rusage usage;
  wait4(pid, &status, 0, &usage);
}

/* For testing purposes */
void print_char(char** x){
  printf("%s", "NOW PRINTING CHARACTER ARRAY:") ;
  for(int i = 0 ; x[i]; i++){
    printf("%s", x[i]);
  }
}
