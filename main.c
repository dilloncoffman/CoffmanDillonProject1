#include "Queues.h"
/**
 * Problems:
 * 1.) Config file read in at runtime and values set, how should that look?
 * 2.) Printing queue of events doesn't populate/work correctly
 * 3.) Logic of final program
 * 4.) Are all queues capacities dynamically allocated?
 **/

// MAIN
int main() {
	// read config file, how do we store the values in this file?
	//readAndWriteConfigFile();
	struct config configStruct;
	configStruct = populateConfigStruct("config.txt");


	/* Struct members */
	printf("SEED: %.0f\n", configStruct.SEED);
	printf("INIT_TIME: %.0f\n", configStruct.INIT_TIME);
	printf("FIN_TIME: %.0f\n", configStruct.FIN_TIME);
	printf("ARRIVE_MIN: %.0f\n", configStruct.ARRIVE_MIN);
	printf("ARRIVE_MAX: %.0f\n", configStruct.ARRIVE_MAX);
	printf("QUIT_PROB: %.1f\n", configStruct.QUIT_PROB);
	printf("CPU_MIN: %.0f\n", configStruct.CPU_MIN);
	printf("CPU_MAX: %.0f\n", configStruct.CPU_MAX);
	printf("DISK1_MIN: %.0f\n", configStruct.DISK1_MIN);
	printf("DISK1_MAX: %.0f\n", configStruct.DISK1_MAX);
	printf("DISK2_MIN: %.0f\n", configStruct.DISK2_MIN);
	printf("DISK2_MAX: %.0f\n", configStruct.DISK2_MAX);


	// test printing a randomly generated number
	// EVERY TIME YOU GENERATE A RANDOM NUMBER, SET THE SEED AS THE NEW RANDOM NUMBER
	configStruct.SEED = generateRandomNumber( configStruct,configStruct.ARRIVE_MIN, configStruct.ARRIVE_MAX);
	printf("\nconfig seed: %f\n", configStruct.SEED);
	configStruct.SEED = generateRandomNumber( configStruct,configStruct.ARRIVE_MIN, configStruct.ARRIVE_MAX);
	printf("config seed: %f\n", configStruct.SEED);
	configStruct.SEED = generateRandomNumber( configStruct,configStruct.ARRIVE_MIN, configStruct.ARRIVE_MAX);
	printf("config seed: %f\n", configStruct.SEED);
	configStruct.SEED = generateRandomNumber( configStruct,configStruct.ARRIVE_MIN, configStruct.ARRIVE_MAX);
	printf("config seed: %f\n", configStruct.SEED);
	configStruct.SEED = generateRandomNumber( configStruct,configStruct.ARRIVE_MIN, configStruct.ARRIVE_MAX);
	printf("config seed: %f\n", configStruct.SEED);
	configStruct.SEED = generateRandomNumber( configStruct,configStruct.ARRIVE_MIN, configStruct.ARRIVE_MAX);
	printf("config seed: %f\n", configStruct.SEED);


	// initialize the EventQueue
	Queue EventQueue = initializeQueue(0);
	printf("\nSize of event queue AT START = %d", EventQueue.currentSize);
	printf("\nCapacity of event queue AT START = %d", EventQueue.capacity);
	printf("\nIs the queue empty AT START? %d", isEmpty(&EventQueue));
	printf("\nIs the queue full AT START? %d\n", isFull(&EventQueue));

	// event type of 9 is SIM_END, 1 is arrival //Event create_event(int eventType, int jobSequenceNumber, double ttime)
	push(&EventQueue, create_event(9, 0, configStruct.FIN_TIME));
	push(&EventQueue, create_event(1, 1, configStruct.INIT_TIME));
	push(&EventQueue, create_event(5, 2, 234));
	
	
	sort(&EventQueue); // ---- CHECK OUT SORT WHY ISN'T IT WORKING?
	printQueue(EventQueue);


	double currentTime = configStruct.INIT_TIME;
	Event currentJob = pop(&EventQueue);
	currentTime += currentJob.time;
	printf("Time check: %.0f", currentTime);

	// While the EventQueue is not empty AND the currentTime is less than FIN_TIME, process events
	// while(!(isEmpty(&EventQueue)) && (currentTime < configStruct.FIN_TIME)) {
	// 	Event currentJob = pop(&EventQueue);
	// 	currentTime = currentJob.time;
	// 	switch(currentJob.eventType) {
	// 		case 1: // arrival
				
	// 			break;
	// 		case 2: // finished
	// 			break;

	// 		case 9: 
	// 			exit(1); // SIM_END
	// 	}

	// }


	// // Push some events to test into EventQ: Event create_event(int eventType, int jobSequenceNumber, int ttime)
	// push(&EventQueue, create_event(1, 1, 1));
	// push(&EventQueue, create_event(5, 5, 5));
	// push(&EventQueue,  create_event(2, 2, 2));
	// push(&EventQueue, create_event(3, 3, 3));
	// push(&EventQueue, create_event(7, 7, 7));
	// push(&EventQueue, create_event(4, 4, 4));
	// push(&EventQueue, create_event(6, 6, 6));
	

	// printf("\nSize of event queue AFTER PUSH = %d", EventQueue.currentSize);
	// printf("\nCapacity of event queue AFTER PUSH = %d", EventQueue.capacity);
	// printf("\n\nUNSORTED QUEUE:");
	// printQueue(EventQueue);
	// sort(&EventQueue);
	// Event previouslyPopped = pop(&EventQueue);
	// printf("JOB # POPPED: %d", previouslyPopped.jobSequenceNumber);
	// printf("\n\nSORTED QUEUE:");
	// printQueue(EventQueue);


	// printf("\nSize of event queue AFTER POP = %d", EventQueue.currentSize);
	// printf("\nIs the queue empty AFTER POP? %d", isEmpty(&EventQueue));
	// // sort(&EventQueue);
	// // printQueue(EventQueue);
	// printf("\n\nDESTROY THE QUEUE\n");
	// destroy(&EventQueue);
	// printf("Is the queue empty AFTER DESTROY? %d", isEmpty(&EventQueue));
	// printQueue(EventQueue);





	// //****************************** ACTUAL PROGRAM START **********************************
	// // set params from config file, enqueue SIM_END event and job1
	// Queue EventQueue = initializeQueue(1000); // make queue dynamic by eventList, dynamically mallocing more memory it needs, will need to handle 1000 events
	// Event SIM_END = create_event(9, 505, 10000);
	// Event job1 = create_event(1, 1, INIT_TIME); // 0 should be INIT_TIME
	// push(&EventQueue, SIM_END);
	// push(&EventQueue, job1);
	// sort(&EventQueue); // sorts it so event with least time is in front, since this is a priority queue
	// // pop(&EventQueue), set current time, select type of event
	// Event currentJob = pop(&EventQueue);
	// int currentTime = INIT_TIME;
	// // based on type of event determine what job does
	// // if CPU arrival, create next job, insert next job into EventQueue, and send currentJob to CPUQueue, then in CPU check if CPU status is idle, if not - do nothing, if it is idle, pop the task from CPUQueue, determine finish time, create finishEvent, and set CPU to busy
	// // if CPU finished, set CPU status to idle, determine if event exits or continues to disk, if it continues to disk - generate a disk event insert event into EventQueue
	// // if Disk arrival, select the shorter DiskQueue (one with shortest currentSize), send current job to DiskQueue, process event with Disk similar to CPU above when arrival of job is sent
	// // if Disk finished, find corresponding job in Disk1 or Disk2 queues, generate new event to CPU, set Disk to idle

	

	return 0; // program was succesful, it ends
}

// // reads the config file and writes that file's contents to the log file
// void readAndWriteConfigFile() {
	
// 	FILE *ptr_file;
// 	FILE *ptr_logFile;
// 	// allocate memory for the info stored in the config file
// 	char *configFileContent = (char*) malloc(sizeof(char));

// 	// open the files to use
// 	ptr_file = fopen("config.txt", "r");
// 	ptr_logFile = fopen("log.txt", "w");

// 	// if file was not opened successfully, print an error then exit
// 	if (!ptr_file) {
// 		printf("ERROR: No file found.");
// 		exit(1);
// 	}

// 	// read the file, printing each line – and write that input file's contents to the log file to be used by our program so we have all data in one succinct spot
// 	while (fgets(configFileContent, sizeof configFileContent, ptr_file) != NULL) {
// 		// print file contents read
// 		printf("%s", configFileContent);
// 		// writes config file's contents to ptr_logFile
// 		fprintf(ptr_logFile, "%s", configFileContent);
// 	}
	
// 	// free the memory used to read the config file
// 	free(configFileContent);
// 	// close the files
// 	fclose(ptr_file);
// 	fclose(ptr_logFile);
// }

// generates a random number between min and max values passed in
double generateRandomNumber(struct config cfg, int min, int max) {
	srand(cfg.SEED);
	double randomNumber = (rand() % (max-min + 1)) + min;
	return randomNumber;
}










/*
 * Populates config struct from config file, and writes config file to log file
 */
struct config populateConfigStruct(char *configFile) {
	struct config configStruct;
	FILE *cFile_ptr = fopen(configFile, "r");
 	FILE *logFile_ptr = fopen("log.txt", "w");
	char configFileContent[150];
	char* token;
	float result; // to store 
	int i = 0;

	if(!cFile_ptr) {
		fputs("Error: File not found.\n", stderr);
        exit(1);
	}
	
	// read file, tokenizing parts of it and assigning members of struct to those values, then return that struct
	while(!feof(cFile_ptr)){
		if (fgets(configFileContent,150,cFile_ptr)) {
			// write file contents to log file
			fprintf(logFile_ptr, "%s", configFileContent);
			// get "number" part of line for that value
			token=strtok(configFileContent," ");
			//printf("Token: %s\n", token);
			// convert token to number (float in this case to account for QUIT_PROB)
			sscanf(token, "%f", &result);
			//printf("Result = %.1f\n", result);
		}
		// assign number result to proper struct member, i keeps track of what member value a given result for a line should be stored in
	   	if (i == 0) {
			configStruct.SEED = result;
		}
		else if (i == 1) {
			configStruct.INIT_TIME = result;
		}
		else if (i == 2) {
			configStruct.FIN_TIME = result;
		}
		else if (i == 3) {
			configStruct.ARRIVE_MIN = result;
		}
		else if (i == 4) {
			configStruct.ARRIVE_MAX = result;
		}
		else if (i == 5) {
			configStruct.QUIT_PROB = result;
		}
		else if (i == 6) {
			configStruct.CPU_MIN = result;
		}
		else if (i == 7) {
			configStruct.CPU_MAX = result;
		}
		else if (i == 8) {
			configStruct.DISK1_MIN = result;
		}
		else if (i == 9) {
			configStruct.DISK1_MAX = result;
		}
		else if (i == 10) {
			configStruct.DISK2_MIN = result;
		}
		else if (i == 11) {
			configStruct.DISK2_MAX = result;
		}
		// increment i to determine which configStruct member the next result should be stored in
		i++;
 		//printf("%s", configFileContent);
	}
	//printf("\n");
	//printf("\n");
	//printf("Size of configFileContent: %lu", sizeof(configFileContent));
	//printf("\n");
	//printf("Seed in function: %f\n\n", configStruct.SEED);
	// close files
	fclose(cFile_ptr);
	fclose(logFile_ptr);
	return configStruct;
}
