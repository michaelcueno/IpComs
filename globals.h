/* Readability Defines */
#define READ 0
#define WRITE 1
#define CHILD 0

#define CONT_MAX 200   // Maximum content length for char arrays 

/* Holds the message buffer for reading/writing to the message queue */
struct msgcontent {
	long int mtype; 
	char content[CONT_MAX]; 
}; 
