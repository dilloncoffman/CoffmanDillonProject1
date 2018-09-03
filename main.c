#include "Queues.h"

// MAIN
int main() {
	// read config file, how do we store the values in this file?
	readAndWriteConfigFile();
	// test printing a randomly generated number
	printf("\n%d\n", generateRandomNumber(ARRIVE_MIN, ARRIVE_MAX));
	// initialize the EventQueue
	Queue EventQueue = initializeQueue(9);
	Event newEventNode = create_event(1, 1, 10);

	printf("\nSize of event queue = %d", EventQueue.currentSize);
	printf("\nCapacity of event queue = %d", EventQueue.capacity);
	printf("\nIs the queue empty? %d", isEmpty(&EventQueue));
	printf("\nIs the queue full? %d\n", isFull(&EventQueue));
	printf("size of event queue = %d", EventQueue.currentSize);
	push(&EventQueue, newEventNode);
	push(&EventQueue, create_event(2, 2, 20)); // create a random event to push to see if pop works
	push(&EventQueue, create_event(2, 3, 45));
	push(&EventQueue, create_event(2, 4, 30));
	push(&EventQueue, create_event(2, 6, 4));
	push(&EventQueue, create_event(2, 7, 544));
	push(&EventQueue, create_event(2, 8, 32));
	printf("\nSize of event queue = %d", EventQueue.currentSize);
	sort(&EventQueue);
	printQueue(EventQueue);
	pop(&EventQueue); // pop the pushed on event
	printf("\nSize of event queue = %d", EventQueue.currentSize);
	//printQueue(EventQueue); // print the queue again to see the currentSize is back to 0
	//destroy(&EventQueue); // not sure if destroy is freeing the memory it should
	printf("\nIs the queue empty? %d", isEmpty(&EventQueue));
	sort(&EventQueue);
	printf("\n\n DESTROY THE QUEUE");
	destroy(&EventQueue);
	printf("\nIs the queue empty? %d", isEmpty(&EventQueue));
	

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
