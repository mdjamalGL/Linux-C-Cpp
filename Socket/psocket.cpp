#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/un.h>
#include <cmath>
#include <poll.h>
#include <cstring>

char *socketPath;

void *connectionHandler(void *arg)
{
    int peerSockFd = *(int *)arg;
    delete (int *)arg;
    char buf[120];
    struct pollfd pfd;
    pfd.fd = peerSockFd;
    pfd.events = POLLIN;


    int pollRes;
    while(1)
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
            }
            else if(readRT == 0)
            {
                break;
            }
            else
            {
                buf[readRT] = '\0';
                std::cout <<gettid()<<" Server: Received size: " << readRT << std::endl;
                std::cout <<gettid()<< " Server: Received: " << buf << std::endl;
                std::cout <<gettid()<< " Starting Time-Intensive Task" << std::endl;
                sleep(20);
                std::cout<<gettid()<<" Task Done"<<std::endl<< std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Server: Poll error or timeout occurred" << std::endl;
    }
    }
    return NULL;
}
int main()
{
    strcpy(socketPath, "/tmp/sockDemo.sock");
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        std::cout<<"Cannot create socket"<<std::endl;
    }

    remove(socketPath);
    struct sockaddr_un sockAddress = {0};
    sockAddress.sun_family = AF_UNIX;
    strcpy(sockAddress.sun_path, socketPath);

    if(bind(sockfd, (struct sockaddr *) &sockAddress, sizeof(sockAddress)) == -1)
    {
        std::cout<<"Server : Bind to path failed"<<std::endl;
        return 0;
    }
    if(listen(sockfd,10) == -1)
    {
        std::cout<<"Server : Error in making socket passive"<<std::endl;
        return 0;
    }
    else
    {
        std::cout << "Server is listening on path: " << socketPath << std::endl<< std::endl;
    }
    while(1)
    {
        int *acptRet = new int;
        *acptRet = accept(sockfd, NULL, NULL);
        if(*acptRet == -1)
        {
            std::cout<<"Server: Error while accepting passive open"<<std::endl;
            return 0;
        }
        else
        {
            std::cout<<"Client Connected to fd : "<<*acptRet<<std::endl<< std::endl;
            pthread_t thread;
            pthread_create(&thread, NULL, &connectionHandler, (void *)acptRet);
            pthread_detach(thread);
        }
    }
    return 0;
}