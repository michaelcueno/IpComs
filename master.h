/**
  * Master program for homework assignment 2
  * Copyright: Michael Cueno @ 2013 (mcueno2)
  *   */
#ifndef _MASTER_H
#define _MASTER_M
  
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error_check_and_parse(int argc, char** argv, int* randSeed, bool* lock );

void fill_rand_sorted_ints(int *nums, int count);
    
#define READ 0
#define WRITE 1
#define CHILD 0



#endif 
    
