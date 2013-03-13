/** 
  * Master delcarations file 
  * Copyright: Michael Cueno @ 2013 (mcueno)
  */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
         #include <sys/types.h>
       #include <sys/ipc.h>
       #include <sys/msg.h>
  #include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


int main(int argc, char** argv){
	

	int id = msgget( IPC_PRIVATE, IPC_CREAT);
		if(id<0){
			fprintf(stderr, "Could not create message queue! %s\n", strerror( errno )); exit(0);
		}

    printf("%d\n", id);

}

