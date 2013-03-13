/* Readability Defines */
#define READ 0
#define WRITE 1
#define CHILD 0
#include <sys/sem.h>

// Included from the handout 
#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED) 
/* union semun is defined by including <sys/sem.h> */ 
#else 
/* according to X/OPEN we have to define it ourselves */ 
union semun { 
      int val;                  /* value for SETVAL */ 
      struct semid_ds *buf;     /* buffer for IPC_STAT, IPC_SET */ 
      unsigned short *array;    /* array for GETALL, SETALL */ 
                                /* Linux specific part: */ 
      struct seminfo *__buf;    /* buffer for IPC_INFO */ 
}; 
#endif 

#define CONT_MAX 200   // Maximum content length for char arrays 

/* Holds the message buffer for reading/writing to the message queue */
struct msgcontent {
	long int mtype; 
	char content[CONT_MAX]; 
}; 
