# Overview 

This program was written for a homework assignment in my operating systems class to illustrate
the use of inter process communications in the linux operating system

# Use

- The program can be compiled by running 'make' from the src directory.

- The master program should be launched with the command line args specified in the pdf handout. 

# Design 

The bulk of the program is in the master.cpp file.
Each main task is seprated into it's own function call. All function descriptions can be found in master.h 
(or worker.h for those used in worker.cpp). Details, and inline comments can be found in the functions themsleves 
as well in the function definitions (in the .cpp files). I have tried to make the main methods of the master 
and worker programs as human readable as possible and this is why you will find few comments there. 

# Bugs

The nBuffers arg (arg1) to the master program needs to be a prime number. This is not checked and the program will not
work correctly if it is not prime. 

### copyright @ 2013 Michael Cueno | mcueno2@uic.edu 
