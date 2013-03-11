/*
Jim Counts and Joshua Pantarotto
CS433 Operating Systems
Project 5 - Due Wednesday May 9
Implement a virtual memory management system.
*/

#include <algorithm>
#include <fstream>
#include <queue>
#include <iostream>
#include <math.h>
#include <sstream>
#include <stdlib.h>
#include <vector>

using namespace std;

// Defines constants for known eviction policies.
enum PolicyType
{
    // First-In/First-Out
    FIFO,

    // Random
    Random,

    // Least Recently Used
    LRU,
};

// Gets the command line argument at the specified poistion, or returns
// a default value if no argument is found at that position.
string GetArgumentAt(
    int argc,
    char *argv[],
    int position,
    string defaultValue )
{
    if ( position < argc )
    {
        return string( argv[position] );
    }

    return defaultValue;
}

// Converts integers stored in strings to thier int equivalent.
int ParseInteger( string value )
{
    stringstream ss( value );
    int i;
    ss >> i;
    return i;
}

// Gets the memory size provided by the user at the command line.
int GetMemorySize( int argc, char *argv[] )
{
    string value = GetArgumentAt( argc, argv, 2, "0" );
    return 1 << min( max( ParseInteger( value ), 13 ), 30 );
}

// Gets the page size provided by the user at the command line.
int GetPageSize( int argc, char *argv[] )
{
    string value = GetArgumentAt( argc, argv, 1, "0" );
    return 1 << min( max( ParseInteger( value ), 8 ), 13 );
}

// Gets the Eviction Policy provided by the user at the command line.
PolicyType GetPolicy( int argc, char *argv[] )
{
    string value = GetArgumentAt( argc, argv, 3, "FIFO" );

    if ( value.compare( "FIFO" ) == 0 )
    {
        return FIFO;
    }

    if ( value.compare( "RND" ) == 0 )
    {
        return Random;
    }

    if ( value.compare( "LRU" ) == 0 )
    {
        return LRU;
    }

    return FIFO;
}

void RandomPageTable( int pageSize, int memorySize, int frameCount )
{
    cout << "Random w/ PageSize: " << pageSize <<
         " MemorySize: " << memorySize <<
         " Frames: " << frameCount << endl;

    vector<int> v;
    int fault = 0;
    int allocate = 0;
    int evict = 0;
    int references = 0;
    srand( time( NULL ) );

    ifstream in;
    in.open( "references.txt" );
    char s[256];
    in.getline( s, 256 );

    while ( in.good() )
    {
        in.getline( s, 256 );
        string text( s );

        if ( text.compare( "" ) != 0 )
        {
            int addr = ParseInteger( text );
            int partition = 32 - log2( addr );
            int pageNo = addr >> partition;

            // Lookup
            references++;
            vector<int>::iterator it = find( v.begin(), v.end(), pageNo );

            if ( it == v.end() )
            {
                fault++;

                if ( 0 < frameCount )
                {
                    // Allocate
                    allocate++;
                    v.push_back( pageNo );
                    frameCount--;
                }
                else
                {
                    // Evict
                    evict++;
                    int idx = rand() % v.size();
                    vector<int>::iterator ep = v.begin() + idx;
                    v.erase( ep );
                    v.push_back( pageNo );
                }

            }
        }
    }

    cout << "References: " << references <<
         " Faults: " << fault <<
         " Allocate: " << allocate <<
         " Evict " << evict << endl;
}

void FifoPageTable( int pageSize, int memorySize, int frameCount )
{
    cout << "FIFO w/ PageSize: " << pageSize <<
         " MemorySize: " << memorySize <<
         " Frames: " << frameCount << endl;

    vector<int> v;
    int fault = 0;
    int allocate = 0;
    int evict = 0;
    int references = 0;

    ifstream in;
    in.open( "references.txt" );
    char s[256];
    in.getline( s, 256 );

    while ( in.good() )
    {
        in.getline( s, 256 );
        string text( s );

        if ( text.compare( "" ) != 0 )
        {
            int addr = ParseInteger( string( s ) );
            int partition = 32 - log2( addr );
            int pageNo = addr >> partition;

            // Lookup
            references++;
            vector<int>::iterator it = find( v.begin(), v.end(), pageNo );

            if ( it == v.end() )
            {
                fault++;

                if ( 0 < frameCount )
                {
                    // Allocate
                    allocate++;
                    v.push_back( pageNo );
                    frameCount--;
                }
                else
                {
                    // Evict
                    evict++;
                    v.erase( v.begin() );
                    v.push_back( pageNo );
                }
            }
        }
    }

    cout << "References: " << references <<
         " Faults: " << fault <<
         " Allocate: " << allocate <<
         " Evict " << evict << endl;
}

void LruPageTable( int pageSize, int memorySize, int frameCount )
{
    cout << "LRU w/ PageSize: " << pageSize <<
         " MemorySize: " << memorySize <<
         " Frames: " << frameCount << endl;

    vector<int> v;
    int fault = 0;
    int allocate = 0;
    int evict = 0;
    int references = 0;

    ifstream in;
    in.open( "references.txt" );
    char s[256];
    in.getline( s, 256 );

    while ( in.good() )
    {
        in.getline( s, 256 );
        string text( s );

        if ( text.compare( "" ) != 0 )
        {
            int addr = ParseInteger( string( s ) );
            int partition = 32 - log2( addr );
            int pageNo = addr >> partition;

            // Lookup
            references++;
            vector<int>::iterator it = find( v.begin(), v.end(), pageNo );

            if ( it == v.end() )
            {
                fault++;

                if ( 0 < frameCount )
                {
                    // Allocate
                    allocate++;
                    v.push_back( pageNo );
                    frameCount--;
                }
                else
                {
                    // Evict
                    evict++;
                    v.erase( v.begin() );
                    v.push_back( pageNo );
                }
            }
            else
            {
                v.erase( it );
                v.push_back( pageNo );
            }
        }
    }

    cout << "References: " << references <<
         " Faults: " << fault <<
         " Allocate: " << allocate <<
         " Evict " << evict << endl;
}

int main( int argc, char *argv[] )
{
    cout << endl << "CS433 Project 5" << endl;
    cout << "Jim Counts and Joshua Pantarotto" << endl << endl;

    int pageSize = GetPageSize( argc, argv );

    int memorySize = GetMemorySize( argc, argv );
    int frameCount = memorySize / pageSize;

    PolicyType policy = GetPolicy( argc, argv );

    switch ( policy )
    {
    case Random:
        RandomPageTable( pageSize, memorySize, frameCount );
        break;
    case FIFO:
        FifoPageTable( pageSize, memorySize, frameCount );
        break;
    case LRU:
        LruPageTable( pageSize, memorySize, frameCount );
        break;
    default:
        return -1;
    }


    return 0;
}
