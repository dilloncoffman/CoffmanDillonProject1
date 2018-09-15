#include "Queues.h"

#define ARRIVAL 1
#define CPU_FINISH 2
#define DISK1_ARRIVAL 3
#define DISK2_ARRIVAL 4
#define DISK1_FINISH 5
#define DISK2_FINISH 6
#define SIM_END 7


int main() {
	int isCpuEmpty = 1;
	int isDisk1Empty = 1;
	int isDisk2Empty = 1;
	int jobNumber = 1;
	int executionInCPU = 0;
	int executionInDisk1 = 0;
	int executionInDisk2 = 0;

	struct config configStruct;
	configStruct = populateConfigStruct("config.txt");

	int currentTime = configStruct.INIT_TIME;
	int nextTime = generateRandomNumber(&configStruct, configStruct.ARRIVE_MIN, configStruct.ARRIVE_MAX);

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

	FILE *logFile_ptr = fopen("log.txt", "a"); // opens file for appending, file pointer moved to end to avoid overwriting config values already written using populateConfigStruct

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
	push(&EventQueue, &simEnd); // push SIM_END to EventQueue right away

	//fprintf(logFile_ptr, "\nAt time %.0f, Job %d arrived.", firstJob.time, firstJob.jobSequenceNumber); // print waht time job 1 arrived
	// printf("\nPrinting Event Queue: \n");
	// printQueue(&EventQueue);
	// printf("\nPrinting CPU Queue: \n");
	// printQueue(&CPUQueue);
	
	//Event currentJob = pop(&EventQueue);
	configStruct.SEED = generateRandomNumber(&configStruct, configStruct.ARRIVE_MIN, configStruct.ARRIVE_MAX);
	int i = 0;

	//  While the EventQueue is not empty AND the currentTime is less than FIN_TIME, process events
	while(!(isEmpty(&EventQueue)) && (currentTime <= configStruct.FIN_TIME)) { // currentTime <= because need to run SIM_END event, which logs "Simulation finished." to log file
		configStruct.SEED = generateRandomNumber(&configStruct, configStruct.ARRIVE_MIN, configStruct.ARRIVE_MAX);  // generates a new seed to be used for each iteration
		// every iteration of the while check to see if the currentTime is equal to the time the next event should arrive in the int above, if it is equal then create the nextEvent and add it to CpuQueue, then find the nextTime (global var)
		if (currentTime == nextTime) { // if currentTime is equal to the time the time nextEvent should arrive
				// create new event, have global int initialized to random int between arrive min and arrive max
				jobNumber += 1; // increment job number for nextEvent
				Event nextEvent = create_event(ARRIVAL, jobNumber,  currentTime); // create nextEvent
				fprintf(logFile_ptr, "\nAt time %.0f, Job %d arrives.", nextEvent.time, nextEvent.jobSequenceNumber); // log event creation ie. arrival
				push(&CPUQueue, &nextEvent); // push the nextEvent to the CPUQueue
				configStruct.SEED = generateRandomNumber(&configStruct, configStruct.ARRIVE_MIN, configStruct.ARRIVE_MAX);
				nextTime =  ((int) generateRandomNumber(&configStruct, configStruct.ARRIVE_MIN, configStruct.ARRIVE_MAX)); //update nextTime an event should occur
				nextTime += currentTime; //update nextTime an event should occur
		}
		
		// go through eventQ, see if a job is scheduled to finish at the current time, if so remove them from event queue and process them, determine if it exits or creates disk event, don't forget to set isCpuEmpty to empty if it's CPU_FINISH
		if(!(executionInCPU == 0)) { // so long as it's not the first execution in the CPU, avoid popping off EventQueue, otherwise you will get a SIM_END event
			for (i = 0; i <= EventQueue.currentSize; i++) { // go through EventQueue, i less than or equal to EventQueue.currentSize, otherwise SIM_END event will never log to file
				if ((EventQueue.eventList[i].time) == (currentTime)) { // if any event's finish time is equal to the current time
					Event finishEvent = pop(&EventQueue); // pop that finish event from EventQueue
					switch (finishEvent.eventType) { // depending on event's finish state, handle the event
						case CPU_FINISH:
							printf("\ncpu finish\n");
							configStruct.SEED = generateRandomNumber(&configStruct, configStruct.ARRIVE_MIN, configStruct.ARRIVE_MAX);
							// check QUIT PROB
							if ((generateRandomNumber(&configStruct, 0, 1)) <= (configStruct.QUIT_PROB)) { // probability event exits system
								fprintf(logFile_ptr, "\nAt time %d, Job %d exits the system.", currentTime, finishEvent.jobSequenceNumber);
								printf("\nEvent exits system\n");
							} else { // check the disk queue sizes, create disk event, push it to appropriate queue
								fprintf(logFile_ptr, "\nAt time %d, Job %d finishes at CPU.", currentTime, finishEvent.jobSequenceNumber); // event finishes at CPU
								// diskEvent will be currentEvents fields, except change the eventType to Disk arrival
								configStruct.SEED = generateRandomNumber(&configStruct, configStruct.ARRIVE_MIN, configStruct.ARRIVE_MAX);
								Event diskEvent = create_event(finishEvent.eventType, finishEvent.jobSequenceNumber, finishEvent.time);
								
								if (Disk1Queue.currentSize == Disk2Queue.currentSize) { // if the disk queues are equal, randomly determine which one to use
									if (generateRandomNumber(&configStruct, 0, 1) < (0.25)) { // if random number is <= 0.4, use disk 1, otherwise use disk2
										printf("\nPushed disk event to disk 1 Queue..\n");
										diskEvent.eventType = DISK1_ARRIVAL;
										push(&Disk1Queue, &diskEvent); // push disk event to disk1Queue
										break;
									} else {
										printf("\nPushed disk event to disk 2 Queue..\n");
										diskEvent.eventType = DISK2_ARRIVAL;
										push(&Disk2Queue, &diskEvent);
										break;
									}
								} else { // the disk queues are not equal in size, so use the smaller one
									// check the size of each disk queue, push diskEvent to shorter one
									if (Disk1Queue.currentSize < Disk2Queue.currentSize) {
										printf("\nPushed disk event to disk 1 Queue..\n");
										diskEvent.eventType = DISK1_ARRIVAL;
										push(&Disk1Queue, &diskEvent); // push disk event to disk1Queue
										break;
									} 
									if (Disk2Queue.currentSize < Disk1Queue.currentSize) {
										// push diskEvent to disk2Queue
										printf("\nPushed disk event to disk 2 Queue..\n");
										diskEvent.eventType = DISK2_ARRIVAL;
										push(&Disk2Queue, &diskEvent);
										break;
									}
								}
							}
							// set the isCPUEmpty 1 CPU
							isCpuEmpty = 1;
							break;
						case DISK1_FINISH:
							// update state to ARRIVAL since it will go to the CPUQueue
							EventQueue.eventList[i].eventType = ARRIVAL;
							// print to log file, done in disk 1
							fprintf(logFile_ptr, "\nAt time %d, Job %d finishes at Disk 1.", currentTime, EventQueue.eventList[i].jobSequenceNumber); // event finishes at Disk 1
							// send right back to CPU queue
							push(&CPUQueue, &EventQueue.eventList[i]);
							// set the isDiskEmpty 1
							isDisk1Empty = 1;
							break;
						case DISK2_FINISH:
							// update state to ARRIVAL since it will go to the CPUQueue
							EventQueue.eventList[i].eventType = ARRIVAL;
							// print to log file, done in disk 1
							fprintf(logFile_ptr, "\nAt time %d, Job %d finishes at Disk 2.", currentTime, EventQueue.eventList[i].jobSequenceNumber); // event finishes at Disk 1
							// send right back to CPU queue
							push(&CPUQueue, &EventQueue.eventList[i]);
							// set the isDiskEmpty 1
							isDisk2Empty = 1;
							break;
						case SIM_END:
							fprintf(logFile_ptr, "\nSimulation finished. :)");
							break;
							// print to log file that simulation finished
							// exit or return 0
					}
				}
			}
		}
		
		if (isCpuEmpty == 1) { // if the CPU is idle
			if(executionInCPU == 0) {
				push(&CPUQueue, &firstJob); // push firstJob to CPUQueue
				fprintf(logFile_ptr, "\nAt time %.0f, Job %d ARRIVES.", firstJob.time, firstJob.jobSequenceNumber); // log event execution in CPU
			}
			if (CPUQueue.currentSize >= 1) { // if CPUQueue's size is greater than or equal to 1
				// PROCESS EVENT FROM CPUQUEUE
				Event currentEvent = pop(&CPUQueue); // get current event from CPU Queue
				fprintf(logFile_ptr, "\nAt time %.0f, Job %d enters the CPU.", currentEvent.time, currentEvent.jobSequenceNumber); // log event execution in CPU
				isCpuEmpty = 0; // set CPU state to busy AKA event is now executing in CPU
				currentEvent.time = ((int)(generateRandomNumber(&configStruct, configStruct.CPU_MIN, configStruct.CPU_MAX))) + currentTime; // set currentEvent's time to the time it will finish in CPU
				currentEvent.eventType = CPU_FINISH; // update event's state to currently executing in CPU, to not confuse myself I named this CPU_FINISH
				
				push(&EventQueue, &currentEvent); //push that event to Event Queue
				//printEvent(&currentEvent);
				sort(&EventQueue); // sort to maintain priority by shortest event time
				executionInCPU += 1; // keeps track of how many executions in CPU
			}
		}

		// Disk 1 component
		if (isDisk1Empty == 1) { // if the CPU is idle
			if (Disk1Queue.currentSize >= 1) { // if CPUQueue's size is greater than or equal to 1
				// PROCESS EVENT FROM CPUQUEUE
				Event currentEvent = pop(&Disk1Queue); // get current event from Disk 1 Queue
				fprintf(logFile_ptr, "\nAt time %.0f, Job %d entered Disk 1.", currentEvent.time, currentEvent.jobSequenceNumber); // log event execution in CPU
				isDisk1Empty = 0; // set Disk state to busy AKA event is now executing in Disk 1
				currentEvent.time = ((int)(generateRandomNumber(&configStruct, configStruct.DISK1_MIN, configStruct.DISK1_MAX))) + currentTime; // set currentEvent's time to finishTime
				currentEvent.eventType = DISK1_FINISH; // update event's state to currently executing in CPU

				//Event job_fin = create_event(CPU_FINISH, currentEvent.jobSequenceNumber, generateRandomNumber(&configStruct, configStruct.CPU_MIN, configStruct.CPU_MAX) + currentTime); // create a CPU_FINISH event at CPU with random time plus the currentTime
				//fprintf(logFile_ptr, "\nAt time %.0f, Job %d finishes at Disk 1.", currentEvent.time, currentEvent.jobSequenceNumber); //log event
				push(&EventQueue, &currentEvent); //push that event to Event Queue
				printEvent(&currentEvent);
				sort(&EventQueue); // sort to maintain priority by shortest event time
				executionInDisk1 += 1; // keeps track of how many executions in CPU
			}
		}

		if (isDisk2Empty == 1) { // if the CPU is idle
			if (Disk2Queue.currentSize >= 1) { // if CPUQueue's size is greater than or equal to 1
				// PROCESS EVENT FROM CPUQUEUE
				Event currentEvent = pop(&Disk2Queue); // get current event from Disk 1 Queue
				fprintf(logFile_ptr, "\nAt time %.0f, Job %d entered Disk 2.", currentEvent.time, currentEvent.jobSequenceNumber); // log event execution in CPU
				isDisk2Empty = 0; // set Disk state to busy AKA event is now executing in Disk 1
				currentEvent.time = ((int)(generateRandomNumber(&configStruct, configStruct.DISK2_MIN, configStruct.DISK2_MAX))) + currentTime; // set currentEvent's time to finishTime
				currentEvent.eventType = DISK2_FINISH; // update event's state to currently executing in CPU

				//Event job_fin = create_event(CPU_FINISH, currentEvent.jobSequenceNumber, generateRandomNumber(&configStruct, configStruct.CPU_MIN, configStruct.CPU_MAX) + currentTime); // create a CPU_FINISH event at CPU with random time plus the currentTime
				//fprintf(logFile_ptr, "\nAt time %.0f, Job %d finishes at Disk 2.", currentEvent.time, currentEvent.jobSequenceNumber); //log event
				push(&EventQueue, &currentEvent); //push that event to Event Queue
				printEvent(&currentEvent);
				sort(&EventQueue); // sort to maintain priority by shortest event time
				executionInDisk2 += 1; // keeps track of how many executions in CPU
			}
		}
		currentTime += 1; // increment time by one for each execution of loop
	}
			// free memory for all Queues allocated
			destroy(&EventQueue);
			destroy(&CPUQueue);
			destroy(&Disk1Queue);
			destroy(&Disk2Queue);
			// close log file
			fclose(logFile_ptr);
			return 0;
		}
		




		// Event testFinish = pop(&EventQueue); // pop the event with smallest time off front of EventQueue
		// if (((int) testFinish.time) == ((int) currentTime)) { // if finish event time is equal to the current
		// 	printf("\nFinish event time WAS equal to current time!\n");
		// 	push(&CPUQueue, &testFinish); // push that event to the CPUQueue
		// 	currentEvent = testFinish;
		// 	currentTime += testFinish.time;
		// } else { 
		// 	printf("\nPushed event back onto EventQueue");
		// 	push(&EventQueue, &testFinish); // push finish event back onto EventQueue
		// 	sort(&EventQueue); // sort EventQueue so smallest time is at front
		// }

	//	printf("\nCURRENT EVENT TYPE: %d", currentEvent.eventType);

		// switch (currentEvent.eventType) {
		// 	case ARRIVAL: // arrival
		// 		process_CPU(&configStruct, &EventQueue, &CPUQueue, isCpuEmpty, currentEvent, currentTime, logFile_ptr);
		// 		break;
		// 	case CPU_FINISH: // finishes at CPU
		// 		process_CPU(&configStruct, &EventQueue, &CPUQueue, isCpuEmpty, currentEvent, currentTime, logFile_ptr);
		// 		break;
		// 	case DISK_ARRIVAL: // arrives at Disk
		// 		process_Disk(&configStruct, &Disk1Queue, &Disk2Queue, &EventQueue, &CPUQueue, currentEvent, isDisk1Empty, isDisk2Empty, currentTime, logFile_ptr);
		// 		// process_Disk determines which queue to use, then is it just like process_CPU minus the exit part?
		// 		break;
		// 	case DISK1_FINISH: // finishes IO at Disk1
		// 		process_Disk(&configStruct, &Disk1Queue, &Disk2Queue, &EventQueue, &CPUQueue, currentEvent, isDisk1Empty, isDisk2Empty, currentTime, logFile_ptr);
		// 		break;
		// 	case DISK2_FINISH: // finishes IO at Disk2
		// 		// What should we be doing here?
		// 		process_Disk(&configStruct, &Disk1Queue, &Disk2Queue, &EventQueue, &CPUQueue, currentEvent, isDisk1Empty, isDisk2Empty, currentTime, logFile_ptr);
		// 		break;
		// 	case SIM_END: // simulation finished
		// 		break;
		// }

// generates a random number between min and max values passed in, seeds random number generator with new seed
float generateRandomNumber(struct config *cfg, int min, int max) {
	srand(time(NULL)); // seed random number generator
	float randomNumber = ((float)rand()/(float)(RAND_MAX)) * max;
	return (float)randomNumber;
}

// void process_CPU(struct config *cfg, Queue *eventQueue, Queue *cpuQueue, int cpu_status, Event currentEvent, float currentTime, FILE *logFile_ptr) {
// 	//cfg->SEED = generateRandomNumber(cfg, cfg->ARRIVE_MIN, cfg->ARRIVE_MAX);
// 	cfg->SEED = generateRandomNumber(cfg, cfg->ARRIVE_MIN, cfg->ARRIVE_MAX);
// 	// if an event 
// 	if (currentEvent.eventType == ARRIVAL) {
// 	//	fprintf(logFile_ptr, "\nAt time %.0f, Job %d arrived.", currentEvent.time, currentEvent.jobSequenceNumber);
// 		// creates a new event to be added to CPUQueue, incrementing job sequence by 1, and its time
// 		printf("\nCURRENT EVENT TYPE IN PROCESS_CPU: %d", currentEvent.eventType);
	
// 		push(cpuQueue, &currentEvent); // send currentJob to CPU
// 	} else {
// 		printf("\nHandling Finish Event at CPU\n");
// 		//if event type is finished at CPU (after being pushed into CPUQueue) (2)
// 		cpu_status = !(cpu_status); // flip cpu status to idle
// 		if(generateRandomNumber(cfg, 0, 1) <= (cfg->QUIT_PROB)) { // probability event exits system
// 			currentEvent.eventType = 100;
// 			fprintf(logFile_ptr, "\nAt time %.0f, Job %d exits the system.", currentEvent.time, currentEvent.jobSequenceNumber);
// 			//free(&currentEvent);
// 			printf("\nEvent exits system\n"); // how do we handle this?
// 		} else { // event doesn't exit system, create a disk event based off the current event and push that to the eventQueue
// 			printf("\nEvent didn't exit system, created disk event\n");
// 			Event diskEvent = create_event(DISK_ARRIVAL, currentEvent.jobSequenceNumber, currentTime); // create a disk event
// 			push(cpuQueue, &diskEvent); // push disk event to eventQueue
// 			//sort(eventQueue); // sort to maintain priority by shortest event time
// 		}
// 	}
// 	if ((!(isEmpty(cpuQueue))) && (cpu_status == 0)) {
// 		printf("\nEntered CPUQueue if statement\n");
// 		Event process = pop(cpuQueue); // get event from CPU Queue
		
// 		//cpu_status = 1;
// 	}
// }

// void process_Disk(struct config *cfg, Queue *disk1Queue, Queue *disk2Queue, Queue *eventQueue, Queue *cpuQueue, Event diskEvent, int disk1_status, int disk2_status, float currentTime, FILE *logFile_ptr) {
	
// }


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
