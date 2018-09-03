/*
    Header file for structs and function prototypes
*/
#ifndef Queues
    // necessary includes
    #include <stdio.h>
    #include <stdlib.h>
    #include <time.h> // used for seeding random number generator

    #define INIT_SIZE 4 // used for inserting into queue if queue's capacity = 0 (see push()function in Queues.c)
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

    // structs
    typedef struct { // better to use typedef to define Event struct for readability?
        // members of the event type
        // struct Event *next;
        int time; // time is how priority is determined in EventQueue
        int jobSequenceNumber; // how is this used in relation to time etc?
        int eventType; // an int because we'll want a switch case that prints the events type based off the number of event taking place
        //struct Event *event; // the event itself
    } Event; //variables of the Event type

    typedef struct {
        // Event *front; // used to keep track of each queues elements 
        // Event *rear;
        int rear, front, currentSize, capacity; // rear and front will keep track of rear and front indices
        Event *eventList; //list of events in queue (AKA an array of Event structs)
    } Queue; // Queue types to use & pointer to base Queue struct

    // DES function protoypes
    void readAndWriteConfigFile();
    int generateRandomNumber(int min, int max);
    Queue initializeQueue();
    void printQueue(Queue q);
    void destroy(Queue *q);
    void destroyEvent(Event *e);
    void sort(Queue *q);
    Event create_event(int eventType, int jobSequenceNumber, int time);
    int isEmpty(Queue *q);
    int isFull(Queue *q);
    void push(Queue *q, Event e);
    Event pop(Queue *q);
    //int push(Queue *q, Event *e);
    //int pushToPriorityQueue(Queue *queue, Event *e);

#endif
