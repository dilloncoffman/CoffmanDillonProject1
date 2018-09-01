#include "Queues.h"
// constants to be used
#define SEED time(NULL)
#define INIT_TIME 10000
#define FIN_TIME 10
#define ARRIVE_MIN 10
#define ARRIVE_MAX 500
#define QUIT_PROB 0.2
#define CPU_MIN 10
#define CPU_MAX 100
#define DISK1_MIN 10
#define DISK1_MAX 100
#define DISK2_MIN 10
#define DISK2_MAX 100

// typedef struct Event { // better to use typedef to define Event struct for readability?
// 	// members of the event type
// 	struct eventNode *next; 
// 	int time; // time is how priority is determined in EventQueue
// 	int jobSequenceNumber; // how is this used in relation to time etc?
// 	char *eventType; //is there a list of all event types possible?
// } eventNode, *eventNodePtr; //variables of the Event type
// // struct Event {
// // 	int time;
// // 	int jobSequenceNumber;
// // 	char eventType;
// // };

// typedef struct Queue {
// 	eventNodePtr front; // used to keep track of each queues elements 
// 	eventNodePtr rear;
// 	int currentSize;
// 	// eventList - a list of strings of possible of events? How do we know all possible events?
// 	int capacity; // how do we determine this? If sizeof(currentSize) > sizeof(capacity)?
// } QType, *Queue; // Queue types to use & pointer to base Queue struct
// PRIORITY QUEUE, priority determined by time
// struct EventQueue {
// 	int currentSize;
// 	// eventList - a list of strings of possible events? Or something else?
// 	int capacity; // what determines capacity?
// };
// NORMAL QUEUES
// Would it be better to build out one queue struct, and then just be like struct Queue cpuQueue, disk1Queue, disk2Queue etc.?
// struct cpuQueue {
// };
// struct disk1Queue {
// };
// struct disk2Queue {
// };

// // initializes a new empty Queue to use
// Queue initQueue()
// {
//     Queue newQueue = malloc(sizeof(QType));
//     newQueue->front = NULL;
//     newQueue->rear = NULL;
// 	newQueue->capacity = 0;
// 	newQueue->currentSize = 0;
// 	//newQueue->eventList = {"job arrived", " job terminated", "etc."};
//     return newQueue;
// }

// MAIN
int main() {
	// read config file, how do we store the values in this file?
	readAndWriteConfigFile();
	printf("\n%d\n", generateRandomNumber(ARRIVE_MIN, ARRIVE_MAX));
	Queue EventQueue = initializeQueue();
	printf("\n%d\n", isEmpty(&EventQueue));
	// Queue EventQueue = initQueue();
	// destroy(EventQueue);

	//printf("Is the current queue empty?: %d", isEmpty(EventQueue));

	return 0; // program was succesful, it ends
}

// reads the config file and writes that file's contents to the log file
void readAndWriteConfigFile() {
	
	FILE *ptr_file;
	FILE *ptr_logFile;
	// allocate memory for the info stored in the config file
	char *configFileContent = (char*) malloc(sizeof(char));

	// open the files to use
	ptr_file = fopen("config.txt", "r");
	ptr_logFile = fopen("log.txt", "w");

	// if file was not opened successfully, print an error then exit
	if (!ptr_file) {
		printf("ERROR: No file found.");
		exit(1);
	}

	// read the file, printing each line – and write that input file's contents to the log file to be used by our program so we have all data in one succinct spot
	while (fgets(configFileContent, sizeof configFileContent, ptr_file) != NULL) {
		// print file contents read
		printf("%s", configFileContent);
		// writes config file's contents to ptr_logFile
		fprintf(ptr_logFile, "%s", configFileContent);
	}
	
	// free the memory used to read the config file
	free(configFileContent);
	// close the files
	fclose(ptr_file);
	fclose(ptr_logFile);
}

// generates a random number between min and max values passed in
int generateRandomNumber(int min, int max) {
	srand(SEED);
	return (rand() % (max-min + 1)) + min;
}
