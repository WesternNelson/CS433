/*
Jim Counts and Joshua Pantarotto
CS433 Operating Systems
Project 4 - Due Wednesday April 18
Implement a producer/consumer queue.
*/
#include <iostream>
#include "Tests.h"
#include "Program.h"

using namespace std;
int main( int argc, char *argv[] )
{
    cout << endl << "CS433 Project 4" << endl;
    cout << "Jim Counts and Joshua Pantarotto" << endl << endl;

    Program p;
    bool runTests = p.ShouldRunTests( argc, argv );

    if ( runTests )
    {
        Tests t;
        t.RunTests();
    }
    else
    {
        p.Run( argc, argv );
    }

    return 0;
}
