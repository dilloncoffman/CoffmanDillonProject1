#include "Queues.h"

/*
 Creates, allocates memory for and returns a Queue
*/
Queue initializeQueue(int capacity) {
	Queue newQ;
	newQ.eventList = (Event*) malloc(sizeof(Event) * capacity);
	if(newQ.eventList == NULL) {
		printf("Empty queue event list");
	}
	newQ.capacity = capacity;
	newQ.currentSize = 0;
	newQ.front = 0; // points to first index in queue
	newQ.rear = capacity - 1; // points to most recent item in queue
	return newQ;
}

/*
 Prints the capacity, currentSize, and event times of the events present in the queue
 */
void printQueue(Queue q) {
	// if the queue is empty, no queue to print, return to main function
    if (isEmpty(&q)) {
        printf("\nERROR: No queue to print!\n");
    } else {
		printf("\n\nPRINTING THE QUEUE");
		int i = 0;
		printf("\ncapacity =  %d", q.capacity);
		printf("\ncurrent size = %d", q.currentSize);
		//printf("\nFirst events time: %d", q.eventList[i].time);
		for (i = 0; i < q.currentSize; i++) {
			// print event structs (the jobSequenceNumber)
			printEvent(q.eventList[i]);
		}
		printf("\n");
	}
}

void printEvent(Event e) {
	printf("\n\nPRINTING EVENT\n");
	printf("\nEvent # %d", e.jobSequenceNumber);
	printf("\nEvent type: %d", e.eventType);
	printf("\nEvent time: %.0f", e.time);
	printf("\n\n");
}

/*
 Frees the memory allocated for the queue, and sets the members to 0
 */
void destroy(Queue *q) {
	if (isEmpty(q)) {
		printf("\nNo queue to free!\n");
	}
	//set currentSize and capacity to 0
    q->currentSize = 0;
    q->capacity = 0;
    free(q->eventList); //free queue
}

Event create_event(int eventType, int jobSequenceNumber, double ttime) {
	Event *newEvent;
	newEvent = (Event*) malloc(sizeof(Event));
	newEvent->eventType = eventType; // 1 for job arrives at Event queue
	newEvent->jobSequenceNumber = jobSequenceNumber; // it's the first job
	newEvent->time = ttime; // the time randomly computed for entering the event queue
	return *newEvent;
}

/*
 Checks if the Queue q is empty, returns 1 if true and 0 if it's not
 */
int isEmpty(Queue *q) {
	if(q->eventList == NULL) {
		printf("No queue to check if empty.\n");
		exit(1);
	}
	return q->currentSize == 0;
}

/*
 Checks if the Queue q is full, returns 1 if true and 0 if it's not
 */
int isFull(Queue *q) {
	return q->currentSize == q->capacity;
}

// /*
//  Pushes new event onto queue
//  */
// void push(Queue *q, Event e) {
// 	// if the queue is full, return
// 	if (isFull(q)) {
// 		printf("\nQueue is full!\n");
// 		return;
// 	} else { // queue is not full, push event to queue
// 		q->rear += 1;
// 		q->eventList[q->rear] = e;
// 		q->currentSize += 1; // increment size of queue by 1, since event successfully pushed to queue
// 		printf("\nPushed event to queue successfully.");
// 	}
// }

/*
 Pushes new event onto queue
 */
void push(Queue *q, Event e) {
	// if q's capacity is 0, set it to INIT_SIZE
	if (q->capacity == 0) {
		q->capacity = INIT_SIZE;
	}
		int i = 0;
	// if the queue is full, dynamically allocate more memory for it by 
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
        for (i = 0; i < (q->capacity)-i; i++) {
            temp[i] = q->eventList[i]; //put dbl to be inserted into reallocated array, should copy over already allocated values
        }
        Event *temp2 = q->eventList; //temp copy address into other pointer
        q->eventList = temp;
        free(temp2); //free old vector;
	}  // queue is not full, push event to queue
	printf("\nDID IT\n");
	q->eventList[q->currentSize] = e;
	q->currentSize += 1; // increment size of queue by 1, since event successfully pushed to queue
	q->front += 1; // increment the counter which keeps track of what is at the front of the queue
	printf("Pushed event to queue successfully.\n");
}

/*
 Sorts a queue (the priority queue)'s events by time and returns that newly sorted queue
 */
Queue sort(Queue *q) {
    if (isEmpty(q)) {
        printf("\n\nNo queue of events to perform a sort on!\n");
    } else {
		int i, swapped;
		while(1) {
			swapped = 0;
			for (i = 0; i <= q->capacity - 3; i++) {
				// if the first event's time is less than the next event's time
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
		return *q;
	}
	return *q;
}

/*
 Pops element from front of the queue and returns it
 */
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

// int push(Queue *q, Event *e) {
// 	if(!isEmpty(q)) {
// 		// NOTICE THAT ONLY LINES 30, 33, and 34 are the errors!
// 		q->rear->next = e->event;
// 		q->rear = e->event;
// 		return 1;
// 	} else {
// 		q->front = e->event;
// 		q->rear = e->event;
// 		return 1;
// 	}

// 	return
// }
// int push(Queue *q, Event *e) {
// 	// if queue's capacity is 0, initialize its size
// 	if (q->capacity == 0) { 
//         q->capacity = INIT_SIZE;
//     }

//     if (q->currentSize == q->capacity) { //if the current size is equal to the capacity, dynamically reallocate space by doubling capacity
//         q->capacity *= 2;
//         int *temp = (int*) malloc(sizeof(e) * q->capacity); //reallocate double the space
//         if (temp == NULL) {
//             printf("Error reallocating!\n");
//             exit(0); // error reallocating space
//         }
//         int i = 0;
//         for (i = 0; i < q->capacity; i++) {
//             temp[i] =  (&q->queue[i]); //put event to be inserted into reallocated queue, and should copy over already allocated events
//         }
//         double *temp2 = q->queue; //temp copy address into other pointer
//         q->queue = temp;
//         free(temp2); //free old vector;
//     }
//     (int*)q->queue[q->currentSize] = &e; //if there's enough space, just insert the event
//     (q->currentSize)++; // increment currentSize since an event has been inserted
//     return 1;
// }