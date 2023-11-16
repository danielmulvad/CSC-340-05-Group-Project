#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "../../common/Message.h"
#include <arpa/inet.h>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

class Client
{
private:
    const unsigned int port;
    int socket_fd;

    void setupConnection()
    {
        struct sockaddr_in serv_addr;
        socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_fd < 0)
        {
            perror("Socket creation error");
            exit(EXIT_FAILURE);
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);

        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
        {
            perror("Invalid address/ Address not supported");
            exit(EXIT_FAILURE);
        }

        if (connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            perror("Connection Failed");
            exit(EXIT_FAILURE);
        }
    }

public:
    Client(unsigned int port) : port(port)
    {
        if (port > 65535)
        {
            throw std::invalid_argument("Port must be less than 65535");
        }
        else if (port < 1024)
        {
            throw std::invalid_argument("Port must be greater than 1024");
        }
    }

    int run()
    {
        setupConnection();

        Message helloMsg(getCurrentTimestamp(), "Hello from client");
        std::string serializedMsg = helloMsg.serialize();
        send(socket_fd, serializedMsg.c_str(), serializedMsg.length(), 0);
        printf("Hello message sent\n");

        char buffer[1024] = {0};
        ssize_t valread = read(socket_fd, buffer, sizeof(buffer) - 1);
        if (valread < 0)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }

        Message receivedMsg = Message::deserialize(std::string(buffer));
        printf("Received: %s\n", receivedMsg.content.c_str());

        close(socket_fd);
        return 0;
    }
};

#endif