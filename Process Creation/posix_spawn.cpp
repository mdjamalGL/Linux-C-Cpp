#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <spawn.h>
#include <sys/wait.h>

extern char **environ;

int main() {
    pid_t pid;
    char *argv[] = {"/bin/ls", "-l", NULL}; 
    char *envp[] = {
        "PATH=/usr/bin:/bin:/usr/local/bin",
        NULL
    };
    int status = posix_spawnp(&pid, "ls", NULL, NULL, argv, envp);
    // int status = posix_spawn(&pid, "ls", NULL, NULL, argv, environ);
    
    if (status == 0) {
        printf("Spawned process with PID %d\n", pid);
        
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid failed");
            return 1;
        }

        printf("Child process exited with status %d\n", WEXITSTATUS(status));
    } else {
        fprintf(stderr, "posix_spawn failed: %d\n", status);
        return 1;
    }

    return 0;
}
