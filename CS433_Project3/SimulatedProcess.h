/*
| Jim Counts and Joshua Pantarotto
| 2012-04-02
| CS 433 Project 3
| An process in the simulation.
*/
#include <iostream>
#include <string>
#include "ProcessStatus.h"

#ifndef SIMULATEDPROCESS_H
#define SIMULATEDPROCESS_H
using namespace std;
class SimulatedProcess
{
    // The next io burst time.
    int ioBurstTime;

public:
    // The total io burst time for this process.
    long totalIOBurstTime;

    // The process identifier.
    int processId;

    // The process start time.
    int startTime;

    // The process arrival time.
    int arrivalTime;

    // The process finish time.
    int finishTime;

    // The total cpu time needed by this process.
    int totalCPUDuration;

    // The remaining cpu time needed by this process.
    int remainingCPUDuration;

    // The average time this process uses the CPU before needing IO.
    int averageCPUBurstLength;

    // The next time this process will need to stop for IO.
    int nextCPUBurstLength;

    // The priority assigned to this process.
    int priority;

    // The current process status.
    ProcessStatus status;

    // Initializes a new instance of the SimulatedProcess class.
    // duration: initializes the total cpu duration.
    SimulatedProcess( int duration )
    {
        totalCPUDuration = duration;
        remainingCPUDuration = duration;
        finishTime = -1;
        totalIOBurstTime = 0;
        nextCPUBurstLength = 0;
        startTime = -1;
    }

    // Sets the next IO burst time.
    void SetIOBurstTime( int time )
    {
        totalIOBurstTime = totalIOBurstTime + time;
        ioBurstTime = time;
    }

    // Sets the starttime if necessary.
    void checkStartTime( const int currentTime )
    {
        if ( startTime == -1 )
        {
            startTime = currentTime;
        }

        return;
    }

    // Formats this instance as a string.
    string ToString() const
    {
        stringstream out;
        out << "Process " << processId << ":" << endl;
        out << "arrival time: " << arrivalTime << " ms" << endl;
        out << "finish time: " << finishTime << " ms" << endl;
        out << "service time: " << totalCPUDuration << " ms" << endl;
        out << "I/O time: " << totalIOBurstTime << " ms" << endl;
        out << "turnaroud time: " << finishTime - arrivalTime << " ms" << endl;
        out << "waiting time: " << ( finishTime - arrivalTime ) - totalCPUDuration << " ms" << endl;
        return out.str();
    }
};
#endif
