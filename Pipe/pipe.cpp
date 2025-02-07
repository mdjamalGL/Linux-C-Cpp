#include <stdio.h>    // For printf()
#include <stdlib.h>   // For exit()
#include <unistd.h>   // For pipe(), read(), write()
#define MSGSIZE 16 

char* msg1 = "hello, world #1"; 
char* msg2 = "hello, world #2"; 
char* msg3 = "hello, world #3"; 

int main() { 
    char inbuf[MSGSIZE]; 
    int p[2], i; 

    // Create a pipe and check for errors
    if (pipe(p) < 0) { 
        perror("pipe"); 
        exit(1); 
    } 

    // Write to the pipe
    write(p[1], msg1, MSGSIZE); 
    write(p[1], msg2, MSGSIZE); 
    write(p[1], msg3, MSGSIZE); 
    
    // Read from the pipe
    for (i = 0; i < 3; i++) { 
        read(p[0], inbuf, MSGSIZE); 
        printf("%s\n", inbuf); 
    } 
    sleep(1000);
    // close(p[1]); 
    // close(p[0]);

    return 0; 
}