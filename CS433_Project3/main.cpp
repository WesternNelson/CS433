/*
| Jim Counts and Joshua Pantarotto
| 2012-04-02
| CS 433 Project 3
| Simulator to analyze different CPU scheduling algorithms.
*/
#include <iostream>
#include <queue>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include "SJFEventHandler.h"
#include "FCFSEventHandler.h"
#include "FCFS.h"
#include "SJF.h"

using namespace std;

// Limits the simulation time.
const int quitTime = 300 * 1000;

// Number of processes to create.
int count;

// The current simulation time.
int currentTime;

// The event queue sorts by event time.
priority_queue<SimulatedEvent *> events;

// A cache of processes, both complete and in progress.
vector<SimulatedProcess *> process_cache;

// Prints the assignment and the name of the authors.
void print_header()
{
    cout << endl << "CS433 Project 3" << endl;
    cout << "Jim Counts and Joshua Pantarotto" << endl << endl;
}

// Get a random number in between min and max.
int RandomBetween( int min, int max )
{
    return ( rand() % ( max - min ) ) + ( min + 1 );
}

// Random start time for a process.
int GenerateRandomStartTime()
{
    return rand() % 300000;
}

// Random total CPU duration for a process.
int GenerateRandomDuration()
{
    return RandomBetween( 1000, 60000 );
}

// Average burst length for a process (used to predict/assign next burst)
int GenerateAverageBurstLength()
{
    return RandomBetween( 5, 100 );
}

// Prints basic info, checks arguments and initializes the process count.
void Initialize( int argc, const char *argv[] )
{
    print_header();
    srand( time( NULL ) );

    if ( argc < 2 )
    {
        cout << "Please enter the number of processes to create on the command line." << endl;
        exit( 1 );
    }

    ::count = atoi( argv[1] );
}

// Resets the time and clears the process cache.
void Reset()
{
    currentTime = 0;
    process_cache.clear();
}

// Populate the event queue and process cache.
void GenerateEvents()
{
    for ( int i = 0; i < ::count; i++ )
    {
        int startTime = GenerateRandomStartTime();

        SimulatedProcess *proc = new SimulatedProcess( GenerateRandomDuration() );
        proc->averageCPUBurstLength = GenerateAverageBurstLength();
        proc->arrivalTime = startTime;
        proc->processId = i;
        process_cache.push_back( proc );

        SimulatedEvent *evt = new SimulatedEvent( proc, PROCESS_ARRIVAL );
        evt->time = startTime;
        events.push( evt );
    }
}

// Prints results after the end of the simulation.
void PrintResults( int turnaroundTime, int totalCPUTime, int completedJobs )
{
    vector<SimulatedProcess *>::iterator it;

    for ( it = process_cache.begin(); it != process_cache.end(); ++it )
    {
        SimulatedProcess *proc = *it;
        cout << proc->ToString() << endl;
    }

    process_cache.clear();

    cout << "CPU Utilization is " << totalCPUTime / ( float )currentTime * 100 << "%" << endl;
    cout << "Throughput is " << completedJobs / ( ( float )currentTime / 1000 ) << " jobs / s" << endl;

    if ( completedJobs > 0 )
    {
        float averageTurnaround = ( ( float )turnaroundTime / 1000 ) / ( float )completedJobs;
        cout << "Average turnaround time:  " << averageTurnaround << " s" << endl;
    }
    else
    {
        cout << "Average turnaround time: > 300 s" << endl;
    }
}

// Simulates process scheduling with the First Come First Served algorithm.
void ProcessEventsWithFCFS()
{
    FCFSScheduler scheduler;
    FCFSSimulatedEventHandler h = FCFSSimulatedEventHandler( &scheduler, &events );

    while ( !events.empty() && currentTime < quitTime )
    {
        // Get the next item *before* possibly modifying the queue in the handler.
        SimulatedEvent *next = events.top();
        events.pop();
        currentTime = next->time;
        h.HandleSimulatedEvent( next );
    }

    PrintResults( h.turnaroundTime, scheduler.totalCPUTime, scheduler.completedJobs );
}

// Simulated process scheduling with the Shortest Job First alogorithm.
void ProcessEventsWithSJF()
{
    SJFScheduler scheduler;
    SJFSimulatedEventHandler h = SJFSimulatedEventHandler( &scheduler, &events );

    while ( !events.empty() && currentTime < quitTime )
    {
        // Get the next item *before* possibly modifying the queue in the handler.
        SimulatedEvent *next = events.top();
        events.pop();
        currentTime = next->time;
        h.HandleSimulatedEvent( next );
    }

    PrintResults( h.turnaroundTime, scheduler.totalCPUTime, scheduler.completedJobs );
}

// The main entry point for the application runs the separate simulations.
int main( int argc, const char *argv[] )
{
    Initialize( argc, argv );

    cout << endl << "FCFS" << endl << endl;
    Reset();
    GenerateEvents();
    ProcessEventsWithFCFS();

    cout << endl << "SJF" << endl << endl;
    Reset();
    GenerateEvents();
    ProcessEventsWithSJF();

    return 0;
}
