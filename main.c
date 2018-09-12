#include "Queues.h"

#define ARRIVAL 1
#define CPU_FINISH 2
#define DISK_ARRIVAL 3
#define DISK1_FINISH 4
#define DISK2_FINISH 5
#define SIM_END 6

int main() {
	struct config configStruct;
	configStruct = populateConfigStruct("config.txt");

	// Struct members from file
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
	configStruct.SEED = generateRandomNumber( &configStruct,configStruct.ARRIVE_MIN, configStruct.ARRIVE_MAX);
	prob(configStruct);
	printf("\nconfig seed: %f\n", configStruct.SEED);
	configStruct.SEED = generateRandomNumber( &configStruct,configStruct.ARRIVE_MIN, configStruct.ARRIVE_MAX);
	prob(configStruct);
	printf("config seed: %f\n", configStruct.SEED);
	configStruct.SEED = generateRandomNumber( &configStruct,configStruct.ARRIVE_MIN, configStruct.ARRIVE_MAX);
	prob(configStruct);
	printf("config seed: %f\n", configStruct.SEED);
	configStruct.SEED = generateRandomNumber( &configStruct,configStruct.ARRIVE_MIN, configStruct.ARRIVE_MAX);
	prob(configStruct);
	printf("config seed: %f\n", configStruct.SEED);
	configStruct.SEED = generateRandomNumber( &configStruct,configStruct.ARRIVE_MIN, configStruct.ARRIVE_MAX);
	prob(configStruct);
	printf("config seed: %f\n", configStruct.SEED);
	configStruct.SEED = generateRandomNumber( &configStruct,configStruct.ARRIVE_MIN, configStruct.ARRIVE_MAX);
	prob(configStruct);
	printf("config seed: %f\n", configStruct.SEED);


	// initialize the Queues
	Queue EventQueue = initializeQueue(0);
	Queue CPUQueue = initializeQueue(0);
	Queue Disk1Queue = initializeQueue(0);
	Queue Disk2Queue = initializeQueue(0);
	printf("\nSize of event queue AT START = %d", EventQueue.currentSize);
	printf("\nCapacity of event queue AT START = %d", EventQueue.capacity);
	printf("\nIs the queue empty AT START? %d", isEmpty(&EventQueue));
	printf("\nIs the queue full AT START? %d\n", isFull(&EventQueue));

	Event simEnd = create_event(SIM_END, 0, configStruct.FIN_TIME);
	Event firstJob = create_event(ARRIVAL, 1, configStruct.INIT_TIME);
	Event randomEvent = create_event(5, 2, 12);
	Event randomEvent2 = create_event(CPU_FINISH, 3, 505);
	Event randomEvent3 = create_event(DISK_ARRIVAL, 4, 24);
	// push SIM_END to EventQueue
	push(&EventQueue, &simEnd);
	// push firstJob to CPU Queue
	push(&EventQueue, &firstJob);
	push(&EventQueue, &randomEvent);
	push(&EventQueue, &randomEvent2);
	push(&EventQueue, &randomEvent3);

	printf("\nPrinting Event Queue: \n");
	printQueue(&EventQueue);
	printf("\nPrinting CPU Queue: \n");
	printQueue(&CPUQueue);


	double currentTime = configStruct.INIT_TIME;
	int cpu_status = 0;
	//Event currentJob = pop(&EventQueue);
	printf("Time check: %.0f", currentTime);

	//  While the EventQueue is not empty AND the currentTime is less than FIN_TIME, process events
	while(!(isEmpty(&EventQueue)) && (currentTime < configStruct.FIN_TIME)) {
		// generates a new seed to be used for each iteration
		configStruct.SEED = generateRandomNumber(&configStruct, configStruct.ARRIVE_MIN, configStruct.ARRIVE_MAX);
		sort(&EventQueue); // sort EventQueue to ensure event with smallest time is at front
		Event currentEvent = pop(&EventQueue); // pop the event with smallest time off front of EventQueue
		if (currentEvent.time == currentTime) {
			// do something..
		}
		currentTime = currentEvent.time; // currentTime = currentEvent's time'
		switch(currentEvent.eventType) {
			case ARRIVAL: // arrival
				process_CPU(&configStruct, &EventQueue, &CPUQueue, cpu_status, currentEvent, currentTime);
				break;
			case CPU_FINISH: // finishes at CPU
				process_CPU(&configStruct, &EventQueue, &CPUQueue, cpu_status, currentEvent, currentTime);
				break;
			case DISK_ARRIVAL: // arrives at Disk
				process_Disk(&Disk1Queue, &Disk2Queue, currentEvent); 
				// process_Disk determines which queue to use, then is it just like process_CPU minus the exit part?
				break;
			case DISK1_FINISH: // finishes IO at Disk1
				// What should we be doing here?
				break;
			case DISK2_FINISH: // finishes IO at Disk2
				// What should we be doing here?
				break;
			case SIM_END: 
				return 0; // simulation finished
				break;
		}
	}

	// free memory for all Queues allocated
	// destroy(&EventQueue);
	// destroy(&CPUQueue);
	// destroy(&Disk1Queue);
	// destroy(&Disk2Queue);
	return 0;
}

// generates a random number between min and max values passed in, seeds random number generator with new seed
int generateRandomNumber(struct config *cfg, int min, int max) {
	srand(cfg->SEED);
	int randomNumber = (rand() % (max-min + 1)) + min;
	return randomNumber;
}

// Not sure how this function should work within process_CPU to exit
int prob(struct config cfg) {
	return ((rand() % 100) < cfg.QUIT_PROB) ? 1 : 0;
}

void process_CPU(struct config *cfg, Queue *eventQueue, Queue *cpuQueue, int cpu_status, Event currentEvent, double currentTime) {
	// if event type is arrival (1)
	if (currentEvent.eventType == ARRIVAL) {
		// creates a new event to be added to EventQueue, incrementing job sequence by 1
		Event nextEvent = create_event(ARRIVAL, currentEvent.jobSequenceNumber + 1, generateRandomNumber(cfg, cfg->ARRIVE_MIN, cfg->ARRIVE_MAX) + currentTime);
		push(eventQueue, &nextEvent);
		sort(eventQueue); // will want the priority queue to be sorted by time
		printf("\nAttempting to print EventQueue: \n");
		printQueue(eventQueue);
		push(cpuQueue, &currentEvent); // send currentJob to CPU
		printf("\nAttempting to print CPUQueue: \n");
		printQueue(cpuQueue);
	} else {
		printf("\nEvent type is finish..\n");
		//if event type is finished at CPU (after being pushed into CPUQueue) (2)
		cpu_status = 0; // flip cpu status to idle
		if(prob(*cfg)) { // probability it exits OR
			// if it doesn't exit, create a disk event based off the current event and push that to the eventQueue
			Event diskEvent = create_event(DISK_ARRIVAL, currentEvent.jobSequenceNumber, currentTime);
			push(eventQueue, &diskEvent); // push disk event to eventQueue
			sort(eventQueue); // sort to maintain priority by shortest event time
		}
	}
	if ((!(isEmpty(cpuQueue))) && (cpu_status == 0)) {
		printf("\nEntered CPUQueue if statement\n");
		Event process = pop(cpuQueue); // get event from CPU Queue
		Event job_fin = create_event(CPU_FINISH, process.jobSequenceNumber, generateRandomNumber(cfg, cfg->CPU_MIN, cfg->CPU_MAX) + currentTime); // create a CPU_FINISH event at CPU
		push(eventQueue, &job_fin); // push the CPU_FINISH event to EventQueue
		sort(eventQueue); // sort to maintain priority by shortest event time
		cpu_status = 1;
	}
}

void process_Disk(Queue *disk1Queue, Queue *disk2Queue, Event diskEvent) {
	// check the size of each disk queue, push diskEvent to shorter one
	if (disk1Queue->currentSize < disk2Queue->currentSize) {
		printf("\nPushed disk event to disk 1 Queue..\n");
		push(disk1Queue, &diskEvent);
		printQueue(disk1Queue);
	} else {
		// push diskEvent to disk2Queue
		printf("\nPushed disk event to disk 2 Queue..\n");
		push(disk2Queue, &diskEvent);
		printQueue(disk2Queue);
	}
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
			// convert token to number (double in this case to account for QUIT_PROB value of 0.2)
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
	}
	// close files
	fclose(cFile_ptr);
	fclose(logFile_ptr);
	// return newly populated configStruct to use in main program
	return configStruct;
}
