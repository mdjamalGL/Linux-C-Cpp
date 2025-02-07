#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/un.h>
#include <poll.h>
#include <cstring>

char *socketPath;

void connectionHandler(int peerSockFd)
{
    char buf[120];
    struct pollfd pfd;
    pfd.fd = peerSockFd;
    pfd.events = POLLIN;

    int pollRes;

    while (1)
    {
        pollRes = poll(&pfd, 1, -1);
        if (pollRes > 0)
        {
            if (pfd.revents & POLLIN)
            {
                int readRT = read(peerSockFd, buf, sizeof(buf));
                if (readRT == -1)
                {
                    std::cerr << "Server: Error while reading from client" << std::endl;
                    return;
                }
                else if(readRT == 0)
                {
                    break;
                }
                else
                {
                    buf[readRT] = '\0';
                    std::cout << "fileDesc. : " << peerSockFd << " Server: Received size: " << readRT << std::endl;
                    std::cout << "fileDesc. : " << peerSockFd << " Server: Received: " << buf << std::endl;
                    std::cout << "fileDesc. : " << peerSockFd << " Starting Time-Intensive Task" << std::endl;
                    sleep(20);
                    std::cout << "fileDesc. : " << peerSockFd << " Task Completed" << std::endl;
                }
            }
        }
        else
        {
            std::cerr << "Server: Poll error or timeout occurred" << std::endl;
        }
    }
}

int main()
{
    strcpy(socketPath, "/tmp/sockDemo.sock");
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        std::cerr << "Cannot create socket" << std::endl;
        return 1;
    }

    remove(socketPath);
    struct sockaddr_un sockAddress = {0};
    sockAddress.sun_family = AF_UNIX;
    strcpy(sockAddress.sun_path, socketPath);

    if (bind(sockfd, (struct sockaddr *)&sockAddress, sizeof(sockAddress)) == -1)
    {
        std::cerr << "Server: Bind to path failed" << std::endl;
        return 1;
    }

    if (listen(sockfd, 10) == -1)
    {
        std::cerr << "Server: Error in making socket passive" << std::endl;
        return 1;
    }
    else
    {
        std::cout << "Server is listening on path: " << socketPath << std::endl;
    }

    while (1)
    {
        int acptRet = accept(sockfd, NULL, NULL);
        if (acptRet == -1)
        {
            std::cerr << "Server: Error while accepting passive open" << std::endl;
            continue;
        }
        else
        {
            std::cout << "Client Connected to fd: " << acptRet << std::endl;
            connectionHandler(acptRet);
            close(acptRet);
        }
    }

    close(sockfd);
    return 0;
}
