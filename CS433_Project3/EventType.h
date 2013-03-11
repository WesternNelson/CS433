/*
| Jim Counts and Joshua Pantarotto
| 2012-04-02
| CS 433 Project 3
| Defines constants that represents simulated event types.
*/
#ifndef EVENTTYPE_H
#define EVENTTYPE_H
enum EventType
{
    PROCESS_ARRIVAL = 0,
    CPU_BURST_COMPLETION = 1,
    IO_COMPLETION = 2,
    TIMER_EXPIRATION = 3
};
#endif
