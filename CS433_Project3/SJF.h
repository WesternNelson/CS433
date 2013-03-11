/*
| Jim Counts and Joshua Pantarotto
| 2012-04-02
| CS 433 Project 3
| A scheduler for the shortest job first algorithm.
*/
#include "SimulatedProcess.h"
#ifndef SJF_H
#define SJF_H

// A function class that compares processes based on thier remaining CPU duration.
class CompareJob
{
public:
    bool operator()( const SimulatedProcess *lhs, const SimulatedProcess *rhs ) const
    {
        return lhs->remainingCPUDuration > rhs->remainingCPUDuration;
    }
};

// A scheduler that uses the Shortest Job First algorithm.
class SJFScheduler
{
    // The ready queue.
    priority_queue< SimulatedProcess *, vector<SimulatedProcess *>, CompareJob > readyQueue;

public:
    // The current CPU state.
    ProcessorState cpuState;

    // The total CPU time used by processes in the simulation.
    int totalCPUTime;

    // The number of jobs completed during the simulation.
    int completedJobs;

    // Initializes a new instance of the SJFScheduler.
    SJFScheduler()
    {
        cpuState = IDLE;
        totalCPUTime = 0;
        completedJobs = 0;
    }

    // Adds an item to the ready queue.
    void AddToReadyQueue( SimulatedProcess *proc )
    {
        readyQueue.push( proc );
    }

    // Runs the scheduler at the specified time.
    SimulatedEvent *Run( int currentTime )
    {
        if ( cpuState == IDLE && !readyQueue.empty() )
        {
            // select and dispatch a proc from ready queue;
            SimulatedProcess *proc = readyQueue.top();
            readyQueue.pop();
            proc->checkStartTime( currentTime );

            // create a CPU completion event;
            SimulatedEvent *completedEvent = new SimulatedEvent( proc, CPU_BURST_COMPLETION );
            completedEvent->time = currentTime + proc->nextCPUBurstLength;

            // Calculate actual CPU time subtracting extra burst length over duration
            int cpuTime = proc->remainingCPUDuration - proc->nextCPUBurstLength;

            if ( cpuTime < 0 )
            {
                totalCPUTime = totalCPUTime + proc->remainingCPUDuration;
                completedJobs++;
                cout << proc->ToString() << endl;
            }
            else
            {
                totalCPUTime = totalCPUTime + proc->nextCPUBurstLength;
            }

            proc->remainingCPUDuration = cpuTime;

            // add the new event into EQ;
            cpuState = BUSY;
            return completedEvent;
        }

        return NULL;
    }
};
#endif
