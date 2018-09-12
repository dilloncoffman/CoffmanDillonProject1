#include "Queues.h"

/*
 Creates, allocates memory for and returns a Queue
*/
Queue initializeQueue(int capacity) {
	Queue newQ;
	newQ.eventList = (Event*) malloc(sizeof(Event) * capacity);
	if(newQ.eventList == NULL) {
		printf("\nEmpty queue event list\n");
	}
	newQ.capacity = capacity;
	newQ.currentSize = 0;
	newQ.front = 0; // points to first index in queue
	return newQ;
}


// Frees the memory allocated for the queue, and sets the members to 0
void destroy(Queue *q) {
	if (isEmpty(q)) {
		printf("\nNo queue to free!\n");
	}
	//set currentSize and capacity to 0
    q->currentSize = 0;
    q->capacity = 0;
    free(q->eventList); //free queue
}

// Creates an event (OR could just use the Event e = {memberdata, memberdata, memberdata} notation too)
Event create_event(int eventType, int jobSequenceNumber, double ttime) {
	Event newEvent;
	newEvent.eventType = eventType; 
	newEvent.jobSequenceNumber = jobSequenceNumber;
	newEvent.time = ttime;
	return newEvent;
}

void destroyEvent(Event *e) {
	e->eventType = 0;
	e->jobSequenceNumber = 0;
	e->time = 0;
	free(e);
}

// Prints the capacity, currentSize, and event times of the events present in the queue
void printQueue(Queue *q) {
	// if the queue is empty, no queue to print, return to main function
    if (isEmpty(q)) {
        printf("\nERROR: No queue to print!\n");
    } else {
		printf("\n\nPRINTING THE QUEUE");
		int i = 0;
		printf("\ncapacity =  %d", q->capacity);
		printf("\ncurrent size = %d", q->currentSize);
		for (i = 0; i < q->currentSize; i++) {
			printEvent(&q->eventList[i]);
		}
		printf("\n");
	}
}

// Prints an event passed in
void printEvent(Event *e) {
	printf("\n\nPRINTING EVENT");
	printf("\nEvent # %d", e->jobSequenceNumber);
	printf("\nEvent type: %d", e->eventType);
	printf("\nEvent time: %.0f", e->time);
	printf("\n\n");
}


// Checks if the Queue q is empty, returns 1 if true and 0 if not
int isEmpty(Queue *q) {
	if(q->eventList == NULL) {
		printf("\nNo queue to check if empty.\n");
		exit(1);
	}
	return q->currentSize == 0;
}

// Checks if the Queue q is full, returns 1 if true and 0 if it's not
int isFull(Queue *q) {
	return q->currentSize == q->capacity;
}


// Pushes new event onto queue
void push(Queue *q, Event *e) {
	// if q's capacity is 0, set it to INIT_SIZE
	if (q->capacity == 0) {
		q->capacity = INIT_SIZE;
	}
		int i = 0;
	//if the queue is full, dynamically allocate more memory for it 
	if (isFull(q)) {
		printf("\nQueue's capacity to be reallocated: %d", q->capacity);
		printf("\n REALLOCATING \n");
		q->capacity *= 2;
		printf("Queue's capacity AFTER reallocation: %d\n", q->capacity);
        Event *temp = (Event *)malloc(sizeof(Event) * q->capacity); //reallocate double the space
        if (temp == NULL) {
            printf("Error reallocating!\n");
            exit(1); //error reallocating vector
        }
        for (i = 0; i < q->currentSize; i++) {
            temp[i] = q->eventList[i]; //put event to be inserted into reallocated eventList
        }
        Event *temp2 = q->eventList; //temp copy address into other pointer
        q->eventList = temp;
        free(temp2); //free old eventList
	} else { // queue is not full, push event to queue
		printf("\nDID IT\n");
		q->eventList[q->currentSize] = *e;
		q->currentSize += 1; // increment size of queue by 1, since event successfully pushed to queue
		q->front += 1; // increment the counter which keeps track of what is at the front of the queue
		printf("Pushed event to queue successfully.\n");
	}  
	
}


// Sorts a queue (the priority queue)'s events by time and returns that newly sorted queue
Queue sort(Queue *q) {
    if (isEmpty(q)) {
        printf("\n\nNo queue of events to perform a sort on!\n");
    } else {
		int i, swapped;
		while(1) {
			swapped = 0;
			for (i = 0; i < q->currentSize; i++) {
				// if the first event's time is less than the next event's times
				if (q->eventList[i].time < q->eventList[i+1].time) {
					// swap the events
					Event temp = q->eventList[i];
					q->eventList[i] = q->eventList[i+1];
					q->eventList[i+1] = temp;
					swapped = 1;
				}
			}
			// if you've sorted all the events, swapped will remain 0 and break from loop
			if (swapped == 0) {
				break;
			}
		}
		// printing sorted event times
		printf("\n\nSorted list times?\n");
		for(i=0; i < q->currentSize; i++) {
			printf("%.0f ", q->eventList[i].time);
		}
	}
	return *q;
}


// Pops element from front of the queue and returns it
Event pop(Queue *q) {
	// if the queue is empty, nothing to pop, exit (not sure if we'd want to exit or return an empty event)
	if (isEmpty(q)) {
		printf("\nQueue is empty, nothing to pop!");
		exit(1);
	} else {
		Event e = q->eventList[(q->front - 1)];
		q->front -= 1; // decrement front index to keep track of front of queue
		q->currentSize -= 1; // decrement the size of the queue since you've popped off an item
		printf("\nPopped from queue successfully.\n");
		return e;
	}
}