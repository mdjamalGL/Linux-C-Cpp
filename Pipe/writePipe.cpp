#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    const char *fifo_path = "/tmp/myfifo";

    int fd = open(fifo_path, O_WRONLY);
    if (fd == -1) {
        perror("Failed to open FIFO for writing");
        return 1;
    }

    const char *message = "Hello, named pipe!";
    write(fd, message, sizeof(message));
    close(fd);
    printf("Message written to FIFO.\n");
    // Clean up
    // unlink(fifo_path);
    return 0;
}
