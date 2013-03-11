/*
| Jim Counts and Joshua Pantarotto
| 2012-04-02
| CS 433 Project 3
| An event handler for the FCFS scheduler.
*/
#include <iostream>
#include <queue>
#include <stdlib.h>
#include <time.h>
#include "SimulatedEvent.h"
#include "FCFS.h"
#include "random.h"

#ifndef FCFSEVENTHANDLER_H
#define FCFSEVENTHANDLER_H
using namespace std;

class FCFSSimulatedEventHandler
{
    // A reference to the event queue.
    priority_queue<SimulatedEvent *> *EventQueue;

    // A reference to the scheduler.
    FCFSScheduler *Scheduler;

    // Get a random number between min and max.
    int RandomBetween( int min, int max )
    {
        return ( rand() % ( max - min ) ) + ( min + 1 );
    }

    // Get a random length for an IO burst.
    int GenerateRandomIOBurstLength()
    {
        return RandomBetween( 30, 100 );
    }

public:
    // The sum of the turnaround times for all completed processes.
    int turnaroundTime;

    // Initializes a new instance of the FCFSSimulatedEventHandler.
    FCFSSimulatedEventHandler( FCFSScheduler *scheduler, priority_queue<SimulatedEvent *> *eventQueue )
    {
        EventQueue = eventQueue;
        Scheduler = scheduler;
        Scheduler->cpuState = IDLE;
        srand( time( NULL ) );
        turnaroundTime = 0;
    }

    // Handles an individual simulation event.
    void HandleSimulatedEvent( const SimulatedEvent *e )
    {
        switch ( e->type )
        {
            // Process Arrival
        case PROCESS_ARRIVAL:
            Handle_PROC_ARRIVAL( e );
            break;

            // CPU Burst Completion
        case CPU_BURST_COMPLETION:
            Handle_CPU_COMPLETION( e );
            break;

            // I/O Completion
        case IO_COMPLETION:
            Handle_IO_COMPLETION( e );
            break;

            // Timer Expiration
        case TIMER_EXPIRATION:
            Handle_TIMER_EXPIRATION( e );
            break;

        default:
            cout << "Unknown event type." << endl;
            return;
        }

        SimulatedEvent *newEvent = Scheduler->Run( e->time );

        if ( newEvent != NULL )
        {
            EventQueue->push( newEvent );
        }
    }

    // Sets the next CPU burst on a process.
    void SetNextBurstFor( SimulatedProcess *proc )
    {
        // generate length of next CPU burst
        int nextBurst = CPUBurstRandom( proc->averageCPUBurstLength );
        proc->nextCPUBurstLength = nextBurst;
    }

    // Put new proceses on the ready queue.
    void Handle_PROC_ARRIVAL( const SimulatedEvent *e )
    {
        SimulatedProcess *proc = e->process;
        SetNextBurstFor( proc );
        Scheduler->AddToReadyQueue( proc );
    }

    // Generate IO Bursts if the process hasn't completed, otherwise collect statistics
    // on the completed process.
    void Handle_CPU_COMPLETION( const SimulatedEvent *e )
    {
        SimulatedProcess *proc = e->process;

        if ( 0 < proc->remainingCPUDuration )
        {
            int ioBurstLength = GenerateRandomIOBurstLength();
            proc->SetIOBurstTime( ioBurstLength );

            // create an IO completion event
            SimulatedEvent *ioCompletionEvent = new SimulatedEvent( proc, IO_COMPLETION );
            int completionTime = e->time + ioBurstLength;
            ioCompletionEvent->time = completionTime;

            // add the new event into the event queue
            EventQueue->push( ioCompletionEvent );
        }
        else
        {
            proc->finishTime = e->time + proc->remainingCPUDuration;
            turnaroundTime = turnaroundTime + ( proc->finishTime - proc->arrivalTime );
        }


        // If the process had no remaining cpu time, just discard it by doing nothing

        Scheduler->cpuState = IDLE;
    }

    // Put processes that have completed IO back onto the ready queue with a new CPU burst length.
    void Handle_IO_COMPLETION( const SimulatedEvent *e )
    {
        SimulatedProcess *proc = e->process;
        SetNextBurstFor( proc );
        Scheduler->AddToReadyQueue( proc );
    }

    // Handle expired timers for algorithms such as RR.
    void Handle_TIMER_EXPIRATION( const SimulatedEvent *e )
    {
        SimulatedProcess *proc = e->process;
        Scheduler->AddToReadyQueue( proc );
        SetNextBurstFor( proc );

        int remainder = proc->remainingCPUDuration - proc->nextCPUBurstLength;
        proc->remainingCPUDuration = remainder;
        Scheduler->cpuState = IDLE;
    }
};
#endif
