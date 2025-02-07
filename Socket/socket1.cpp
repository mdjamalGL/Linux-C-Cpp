#include <sys/socket.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/un.h>

int main()
{
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un sockAddress = {0};
    sockAddress.sun_family = AF_UNIX;

    char *socketPath;
    strcpy(socketPath, "/tmp/sockDemo.sock");
    strcpy(sockAddress.sun_path, socketPath);

    if (sockfd == -1)
    {
        std::cout<<"Cannot create socket"<<std::endl;
    }
    
    if(connect(sockfd, (struct sockaddr *) &sockAddress, sizeof(sockAddress)) == -1)
    {
        std::cout<<"P1 : Connection error"<<std::endl;
    }
    else
    {
        std::cout<<"Connection to Server"<<std::endl;
        char buf[120];
        while(1)
        {
            std::cout<<"Enter String to send to server : ";
            std::string sendData;
            std::cin>>sendData;
            strcpy(buf, sendData.c_str());
            write(sockfd, buf, sizeof(char)*120);
            std::cout<<"Sent Successful"<<std::endl;
        }
    }
    return 0;
}