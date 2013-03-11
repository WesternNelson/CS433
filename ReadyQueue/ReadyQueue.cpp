// ReadyQueue.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <sys/time.h>
#include <stdlib.h>
#include "global.h"
#include <stdio.h>

struct ProcessControlBlock
{
    char processId;
    char priority;
    ProcessState processState;

    ProcessControlBlock()
    {
        processState = New;
    }
};

struct ReadyQueue
{
    static const int QUEUE_SIZE = 10;
    ProcessControlBlock *items[QUEUE_SIZE];
    char head;

    ReadyQueue()
    {
        head = 9;
    }

    void ageQueue()
    {
        for ( char tail = head + 1; tail < QUEUE_SIZE; tail++ )
        {
            char priority = items[tail]->priority;

            if ( priority > 1 )
            {
                items[tail]->priority = priority - 1;
            }
        }
    }

    void insertProc( ProcessControlBlock *process )
    {
        if ( process->processState != Ready )
        {
            process->processState = Ready;
            items[head] = process;
            head--;
        }

        return;
    }

    ProcessControlBlock *removeHighestProc()
    {
        char tail = head + 1;
        char queue_length = QUEUE_SIZE - tail;

        // Single node by itself is always the smallest
        if ( queue_length > 1 )
        {
            for ( char root = head + ( queue_length >> 1 ); root >= tail; root-- )
            {
                char left_leaf = ( root << 1 ) + 1 - tail;
                char right_leaf = left_leaf + 1;

                // find the smallest leaf
                char leaf;

                if ( right_leaf == QUEUE_SIZE )
                {
                    leaf = left_leaf;
                }
                else if ( items[left_leaf]->priority < items[right_leaf]->priority )
                {
                    leaf = left_leaf;
                }
                else
                {
                    leaf = right_leaf;
                }

                // swap root and smallest leaf when leaf is smaller
                if ( items[leaf]->priority < items[root]->priority )
                {
                    ProcessControlBlock *value = items[leaf];
                    items[leaf] = items[root];
                    items[root] = value;
                }
            }
        }

        ProcessControlBlock *selectedItem = items[++head];
        selectedItem->processState = Running;
        items[head] = 0;
        return selectedItem;
    }

    char queueSize()
    {
        return QUEUE_SIZE - head - 1;
    }

    void displayQueue()
    {
        printf( "Queue:\n" );

        for ( char i = head + 1; i < QUEUE_SIZE; i++ )
        {
            printf( "%2i: PID %2i Priority %2i State %2i\n", i, items[i]->processId, items[i]->priority, items[i]->processState );
        }

        return;
    }
};

void TestOne()
{
    ProcessControlBlock *pcb_table = new ProcessControlBlock[20];

    for ( char i = 0; i < 20; i++ )
    {
        pcb_table[i].processId = i;
        pcb_table[i].priority = i;
    }

    ReadyQueue q1;
    q1.insertProc( &pcb_table[5] );
    q1.insertProc( &pcb_table[1] );
    q1.insertProc( &pcb_table[8] );
    q1.insertProc( &pcb_table[11] );
    q1.displayQueue();

    for ( char i = 0; i < 2; i++ )
    {
        q1.removeHighestProc();
        q1.displayQueue();
    }

    q1.insertProc( &pcb_table[3] );
    q1.insertProc( &pcb_table[7] );
    q1.insertProc( &pcb_table[2] );
    q1.insertProc( &pcb_table[12] );
    q1.insertProc( &pcb_table[9] );

    char count = q1.queueSize();

    // we start at 1 because the first thing we do is remove one item from the queue.
    for ( char i = 1; i < count; i++ )
    {
        q1.removeHighestProc();
        q1.displayQueue();
    }

    delete [] pcb_table;
}

void TestTwo()
{
    ProcessControlBlock *pcb_table = new ProcessControlBlock[20];

    for ( char i = 0; i < 20; i++ )
    {
        pcb_table[i].processId = i;
    }

    ReadyQueue q1;
    struct timeval seed;
    gettimeofday( &seed, NULL );
    srand( seed.tv_usec );

    for ( char i = 0; i < 10; i++ )
    {
        ProcessControlBlock *pcb;
        char priority = 50;

        do
        {
            priority = rand() % 50;
            pcb = &pcb_table[priority % 20];
        }
        while ( pcb->processState == Ready );

        pcb->priority = priority;
        q1.insertProc( pcb );
    }

    struct timeval startat, endat;

    gettimeofday( &startat, NULL );

    for ( int i = 0; i < 1000000; i++ )
    {
        q1.removeHighestProc();
        q1.ageQueue();
        ProcessControlBlock *pcb;
        char priority = 50;

        do
        {
            priority = rand() % 50;
            pcb = &pcb_table[priority % 20];
        }
        while ( pcb->processState == Ready );

        pcb->priority = priority;
        q1.insertProc( pcb );
    }

    gettimeofday( &endat, NULL );
    int sec = endat.tv_sec - startat.tv_sec;
    int usec = endat.tv_usec - startat.tv_usec;

    if ( usec < 0 )
    {
        --sec;
        usec += 1000000;
    }

    printf( "Time: %i Seconds and %i microseconds\n", sec, usec );
    q1.displayQueue();

    delete [] pcb_table;
}

int main()
{
    printf( "Jim Counts and Joshua Pantarotto\n" );
    TestOne();
    TestTwo();
    return 0;
}

