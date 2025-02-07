#include <iostream>
#include <unistd.h>

int main()
{
    std::cout<<"This is from original Process"<<std::endl;
    /**
     * execl
     * l - list should be used when number of args is known at compile time
     */
    //execl("/bin/ls","ls", "-la", NULL);

    /**
     * execv
     * v - array, should be used when number of args is not known at compile time
     */
    // char *arg_array[] = {"ls", "-a", NULL};
    // execv("/bin/ls", arg_array);

    /**
     * execlp
     * p - only the name of the command is to be given, rest the system will search
     * if 'p' is not used then full binary path e.g. /bin/ls is to be written
     */
    // execlp("ps", "ps", "-a", NULL);
    
    std::cout<<"This print should not work "<<std::endl;
}