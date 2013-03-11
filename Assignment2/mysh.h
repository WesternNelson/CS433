#ifndef mysh_h
#define mysh_h
#include <stack>
using namespace std;

class mysh
{
public:
    int change_directory( const char *target );
    void push_directory( const char *target );
    void pop_directory();
    char *prompt();
    void execute_external( char *const argv[] );
    void print_directory_stack();

private:
    stack<const char *> directory_stack;

};
#endif