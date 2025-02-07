#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    const char *fifoPath = "/tmp/myfifo";

    // Create a named pipe (FIFO)
    if (mkfifo(fifoPath, 0666) == -1) {
        perror("mkfifo");
        return 1;
    }

    printf("Named pipe created at %s\n", fifoPath);

    return 0;
}
