#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    const char *fifo_path = "/tmp/myfifo";

    int fd = open(fifo_path, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open FIFO for reading");
        return 1;
    }

    char buffer[128];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0'; // Null-terminate the string
        printf("Received message: %s\n", buffer);
    } else {
        perror("Read failed");
    }
    close(fd);

    // Clean up
    // unlink(fifo_path); // Remove the FIFO file
    return 0;
}
