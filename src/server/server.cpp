#include "server.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

#include "../resolve/resolve.h"
#include "luaplugins/luaplugins.h"

namespace server
{
int sockfd;

bool Listen(unsigned short port)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(port);
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sockfd, (struct sockaddr *)&sockAddr, sizeof(sockAddr)) == -1)
    {
        perror("Error");
        return false;
    }
    if (listen(sockfd, QUEUE) == -1)
    {
        perror("Error");
        return false;
    }
    return true;
}

void Handle(const char* path, const char* dir)
{
    luaplugins::initialize(path, dir);

    char buffer[BUFFER_SIZE] = { 0 };
    struct sockaddr_in clientAddr;
    socklen_t length = sizeof(clientAddr);
    while (1)
    {
        int conn = accept(sockfd, (struct sockaddr*)&clientAddr, &length);
        if (conn < 0)
        {
            perror("failed to connect.");
            exit(1);
        }
        memset(buffer, 0, sizeof(buffer));
        recv(conn, buffer, sizeof(buffer), 0);
//        printf("%s\n", inet_ntoa(clientAddr.sin_addr));
//        printf("%s\n", buffer);
        if (resolve::GetRequestType(buffer) == "HTTP")
        {
            auto type = resolve::GetHTTPRequestType(buffer);
            auto dir = resolve::GetHTTPRequestDir(buffer);
            auto post = resolve::GetHTTPRequestPost(buffer);
            luaplugins::resolveDirectory(conn, type, dir, post);
            //std::string s = "HTTP/1.1 200 OK\r\n\r\nok";
            //send(conn, s.c_str(), s.size(), 0);
        }
        close(conn);
    }

    luaplugins::terminate();
}
}
