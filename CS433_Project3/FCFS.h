/*
| Jim Counts and Joshua Pantarotto
| 2012-04-02
| CS 433 Project 3
| A scheduler that uses the First Come First Served scheduling algorithm.
*/
#ifndef FCFS_H
#define FCFS_H
class FCFSScheduler
{
    // The ready queue.
    queue< SimulatedProcess *> readyQueue;

public:
    // The CPU state can be running or idle.
    ProcessorState cpuState;

    // Total CPU time used during the simulation.
    int totalCPUTime;

    // The number of jobs completed during the simulation.
    int completedJobs;

    // Initializes a new instance of the FCFS scheduler.
    FCFSScheduler()
    {
        cpuState = IDLE;
        totalCPUTime = 0;
        completedJobs = 0;
    }

    // Add a process to the scheduler's ready queue.
    void AddToReadyQueue( SimulatedProcess *proc )
    {
        readyQueue.push( proc );
    }

    // Run the scheduler at the specified time.
    // If a new process is removed from the ready queue, this method will return a CPU_BURST_COMPLETION
    // event for that process's finish time.
    // If the CPU was busy when this method was called, this method returns NULL.
    SimulatedEvent *Run( int currentTime )
    {
        if ( cpuState == IDLE && !readyQueue.empty() )
        {
            // select and dispatch a proc from ready queue;
            SimulatedProcess *proc = readyQueue.front();
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
