/*
| Jim Counts and Joshua Pantarotto
| 2012-04-02
| CS 433 Project 3
| Defines the states that a process can assume.
*/
#ifndef PROCESSSTATUS_H
#define PROCESSSTATUS_H
enum ProcessStatus
{
    READY = 0,
    RUNNING = 1,
    WAITING = 2,
    TERMINATED = 3
};
#endif
