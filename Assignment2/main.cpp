//************************************************************************************
//  main.cpp
//  mysh
//
//  Created by Joshua Pantarotto on 2/21/12
//  Modified: Jim Counts and Josh Pantarotto 2/27/2012
//  CS 433 Operating Systems
//  Programming Assignment 2
//
//  This program acts as a basic UNIX shell and responds to basic unix commands.
//  Filenames must not contain spaces.  Implements its own version of cd, pushd, popd,
//  and dirs as well as all other shell executables via threads.
//  Compile using "make" command in UNIX shell.
//*************************************************************************************

#include "mysh.h"
#include <iostream>
#include <stack>
#include <string.h>
#include <stdlib.h>

const int MAXARGS = 256;

// The main entry point for the mysh application
int main( int argc, const char *argv[], char *const envp[] )
{
    mysh newShell;
    // A collection of tokens
    char *tokens[MAXARGS];

    // Get initial user command
    char *userStr = NULL;

    do
    {
        userStr = newShell.prompt();

#if DEBUG
        cout << "User string is: " << userStr << endl;
#endif


        int arg_count = 0;
        tokens[arg_count++] = strtok( userStr, " " );

        if ( tokens[arg_count - 1] == NULL )
        {
            continue;
        }



        while ( arg_count < MAXARGS && tokens[arg_count - 1] != NULL )
        {
            tokens[arg_count++] = strtok( NULL, " " );
        }

#if DEBUG

        for ( int i = 0; i < arg_count; i++ )
        {
            printf( "Arg[%i]: %s\n", i, tokens[i] );
        }

#endif

        if ( strcmp( tokens[0], "cd" ) == 0 )
        {
            newShell.change_directory( tokens[1] );
        }
        else if ( strcmp( tokens[0], "pushd" ) == 0 )
        {
            newShell.push_directory( tokens[1] );
#if DEBUG
            //cout << "Directory stack size: " << directory_stack.size() << endl;
#endif
        }
        else if ( strcmp( tokens[0], "popd" ) == 0 )
        {
            newShell.pop_directory();
        }
        else if ( strcmp( tokens[0], "dirs" ) == 0 )
        {
            newShell.print_directory_stack();
        }
        else if ( strcmp( tokens[0], "exit" ) == 0 )
        {
            exit( 0 );
        }
        else
        {
            newShell.execute_external( tokens );
        }


    }
    while ( strcmp( userStr, "exit" ) != 0 );

    exit( 0 );
}
