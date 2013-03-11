
#include <iostream>
#include <string.h>
// For UNIX commands
#include <sys/types.h>
#include <unistd.h>
// Declaration for exit()
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include "mysh.h"


using namespace std;

// The maximum number of arguments the user is allowed to pass.
//const int MAXARGS = 256;
const string PROMPT = "mysh";

// Forks a child process from the shell and attempts to execute the named
// process.  Takes a char * array of arguments to the attempted executable
void mysh::execute_external( char *const argv[] )
{
    pid_t cpid;
    int status;

    cpid = fork();

    if ( cpid == -1 )
    {
        fprintf( stderr, "Fork failed!\n" );
        exit( -1 );
    }

    if ( cpid == 0 )
    {
        int i = execvp( argv[0], argv );

        if ( i < 0 )
        {
            printf( "%s: command not found\n", argv[0] );
            exit( 1 );
        }
    }
    else
    {
        /* Code executed by parent */
        waitpid( cpid, &status, WUNTRACED | WCONTINUED );
    }
}

// Gets a string entered by the user at the command prompt.
char *mysh::prompt()
{
    char *cwd = new char[512];

    if ( getcwd( cwd, 512 ) != NULL )
    {
        printf( "[%s %s] ", PROMPT.c_str(), cwd );
    }
    else
    {
        printf( "[%s] ", PROMPT.c_str() );
    }


    char *userStr = new char[256];
    cin.getline( userStr, 256 );
    return userStr;
}

// Changes the current working directory.
// Params: The target directory
int mysh::change_directory( const char *target )
{
    int result = chdir( target );

    if ( result != 0 )
    {
        printf( "Error: %s is an invalid directory\n", target );
    }

    return result;
}

// Pushes the current working directory to a stack structure
// and attempts to change directories to the target directory
// Params: Char * for the name of the target directory
void mysh::push_directory( const char *target )
{
    char *cwd = new char[512];

    if ( getcwd( cwd, 512 ) != NULL )
    {
        directory_stack.push( cwd );
        int result = change_directory( target );

        if ( result != 0 )
        {
            directory_stack.pop();
        }
    }
}

// Pops the first directory off of the stack or notifies the user of
// an empty directory stack
void mysh::pop_directory()
{
    if ( directory_stack.empty() )
    {
        printf( "Error: Directory stack is empty\n" );
    }
    else
    {
        int result = change_directory( directory_stack.top() );

        if ( result == 0 )
        {
#if DEBUG
            //printf("Directory change successful\n");
#endif
            directory_stack.pop();
        }
    }
}

// Prints the contents of the directory stack
void mysh::print_directory_stack()
{
    // To hold the stack contents while displaying the stack
    stack<const char *> dirs;

    if ( directory_stack.empty() )
    {
        printf( "Error: Directory stack is empty\n" );
    }
    else
    {
        while ( !directory_stack.empty() )
        {
            dirs.push( directory_stack.top() );
            directory_stack.pop();
            printf( "%s\n", dirs.top() );
        }

        while ( !dirs.empty() )
        {
            directory_stack.push( dirs.top() );
            dirs.pop();
        }
    }

}

