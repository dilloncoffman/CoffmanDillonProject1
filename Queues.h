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
        float SEED,
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
        int rand;
    };
    // define an Event type as a struct with certain data to be used
    typedef struct { // better to use typedef to define Event struct for readability?
        // members of the event type
        // struct Event *next;
        int time; // time is how priority is determined in EventQueue
        int jobSequenceNumber; // how is this used in relation to time etc?
        int eventType; // an int because we'll want a switch case that prints the events type based off the number of event taking place
        //struct Event *event; // the event itself
    } Event; //variables of the Event type
    // define a Queue struct with certain data to be used
    typedef struct {
        // Event *front; // used to keep track of each queues elements 
        // Event *rear;
        int rear, front, currentSize, capacity; // rear and front will keep track of rear and front indices
        Event *eventList;//list of events in queue (AKA an array of Event structs)
    } Queue; // Queue types to use & pointer to base Queue struct

    // DES function protoypes
    struct config populateConfigStruct(char *file);
    int generateRandomNumber(struct config cfg, int min, int max);
    Queue initializeQueue();
    void printQueue(Queue q);
    void printEvent(Event e);
    void destroy(Queue *q);
    void destroyEvent(Event *e);
    void sort(Queue *q);
    Event create_event(int eventType, int jobSequenceNumber, int time);
    int isEmpty(Queue *q);
    int isFull(Queue *q);
    void push(Queue *q, Event e);
    Event pop(Queue *q);

#endif
