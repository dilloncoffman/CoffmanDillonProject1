#include "Queues.h"

// creates a new queue
Queue initializeQueue() {
	Queue newQueue;
	newQueue.queue = malloc(sizeof(Queue));
	newQueue.front = NULL;
	newQueue.rear = NULL;
	newQueue.capacity = 0;
	newQueue.currentSize = 0;
	return newQueue;
}

// checks if the Queue is empty
int isEmpty(Queue *q) {
	return q->front == NULL;
}