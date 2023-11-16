#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <arpa/inet.h>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

class Client
{
private:
    const unsigned int port = 8080;

public:
    Client(const unsigned int &port) : port(port)
    {
        if (port > 65565)
        {
            throw std::invalid_argument("Port must be less than 65565");
        }
        else if (port < 1024)
        {
            throw std::invalid_argument("Port must be greater than 1024");
        }
    }

    int run()
    {

        int status, valread, client_fd;
        struct sockaddr_in serv_addr;
        std::string hello = "Hello from client";
        char buffer[1024] = {0};
        if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            printf("\n Socket creation error \n");
            return -1;
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(this->port);

        // Convert IPv4 and IPv6 addresses from text to binary form
        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
        {
            printf(
                "\nInvalid address/ Address not supported \n");
            return -1;
        }

        if ((status = connect(client_fd, (struct sockaddr *)&serv_addr,
                              sizeof(serv_addr))) < 0)
        {
            printf("\nConnection Failed \n");
            return -1;
        }

        send(client_fd, hello.c_str(), strlen(hello.c_str()), 0);
        printf("Hello message sent\n");
        valread = read(client_fd, buffer, 1024 - 1);
        printf("%s\n", buffer);

        close(client_fd);
        return 0;
    };
};

#endif