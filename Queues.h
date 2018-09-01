/*
    Header file for structs and function prototypes
*/
#ifndef Queues
    // necessary includes
    #include <stdio.h>
    #include <stdlib.h>
    // time is used for seeding the random number generator
    #include <time.h>

    // structs
    typedef struct { // better to use typedef to define Event struct for readability?
        // members of the event type
        // struct Event *next;
        int time; // time is how priority is determined in EventQueue
        int jobSequenceNumber; // how is this used in relation to time etc?
        char *eventType; //is there a list of all event types possible?
    } Event; //variables of the Event type

    typedef struct {
        Event *front; // used to keep track of each queues elements 
        Event *rear;
        struct Queue *queue;
        int currentSize;
        // eventList - a list of strings of possible of events? How do we know all possible events?
        int capacity; // how do we determine this? If sizeof(currentSize) > sizeof(capacity)?
    } Queue; // Queue types to use & pointer to base Queue struct

    // DES function protoypes
    void readAndWriteConfigFile();
    int generateRandomNumber(int min, int max);
    Queue initializeQueue();
    int isEmpty(Queue *q);

#endif
