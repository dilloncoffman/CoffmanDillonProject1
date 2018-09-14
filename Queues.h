/*
    Header file for structs and function prototypes
*/
#ifndef Queues
    // necessary includes
    #include <stdio.h>
    #include <stdlib.h>
    #include <time.h> // used for seeding random number generator
    #include <string.h> // may not need, tried to use for getting variables from config file

    #define INIT_SIZE 4 // for dynamically allocating queue based on it's size

    // ************ STRUCTS  *************
    // define all config file values into a struct config
    struct config {
        double SEED,
        INIT_TIME,
        FIN_TIME,
        ARRIVE_MIN,
        ARRIVE_MAX,
        QUIT_PROB,
        CPU_MIN,
        CPU_MAX,
        DISK1_MIN,
        DISK1_MAX,
        DISK2_MIN,
        DISK2_MAX;
    };
    // define an Event type as a struct with certain data to be used
    typedef struct {
        float time;
        int jobSequenceNumber;
        int eventType;
    } Event; 
    // define a Queue struct with certain data to be used
    typedef struct {
        int front, currentSize, capacity; // front will hold index of whatever event is at the front of the queue, currentSize is the current number of events in a queue, and capacity is the max number of elements a queue can hold before being dynamically reallocated
        Event *eventList; //list of events in queue (AKA an array of Event structs)
    } Queue;

    // DES function protoypes
    struct config populateConfigStruct(char *file); // populates config struct by reading in config file and converting its values then storing them in config's members, also writes the config file to the log file
    void process_CPU(struct config *cfg, Queue *eventQueue, Queue *cpuQueue, int cpu_status, Event e, float currentTime, FILE *logFile_ptr); // processes CPU event
    void process_Disk(struct config *cfg, Queue *disk1Queue, Queue *disk2Queue, Queue *eventQueue, Queue *cpuQueue, Event diskEvent, int disk1_status, int disk2_status, float currentTime, FILE *logFile_ptr); // processes Disk event
    float generateRandomNumber(struct config *cfg, int min, int max); // generates a random number number between min and max
    Queue initializeQueue(int capacity);
    void printQueue(Queue *q); // to test
    void printEvent(Event *e); // to test
    void destroy(Queue *q); // to free queue space
    Queue sort(Queue *q); // to sort queue's events by time
    Event create_event(int eventType, int jobSequenceNumber, float ttime); // to create an event
    void destroyEvent(Event *e); // to destroy just an event, don't really need this
    int isEmpty(Queue *q); // checks if queue is empty
    int isFull(Queue *q); // checks if a queue is full
    void push(Queue *q, Event *e); // pushes an event to the queue
    Event pop(Queue *q); // pops an event from the front of the queue

#endif
