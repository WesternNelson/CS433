/*
| Jim Counts and Joshua Pantarotto
| 2012-04-02
| CS 433 Project 3
| An event in the simulation.
*/
#include <string>
#include "EventType.h"
#include "SimulatedProcess.h"
#include "ProcessorState.h"

#ifndef SIMULATEDEVENT_H
#define SIMULATEDEVENT_H
using namespace std;

class SimulatedEvent
{

public:
    // Time the event should take place
    int time;

    // Type of event.
    EventType type;

    // Process associated with the event.
    SimulatedProcess *process;

    // Initializes a new instance of the SimulatedEvent class.
    SimulatedEvent( SimulatedProcess *proc, EventType etype )
    {
        time = 0;
        type = etype;
        process = proc;
    }

    // Override < operator for LessThan-Comparable compatibility
    bool operator< ( const SimulatedEvent &rhs ) const
    {
        return time > rhs.time;
    }
};
#endif
