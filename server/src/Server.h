#ifndef _SERVER_H_
#define _SERVER_H_

#include "../../common/Message.h"
#include <netinet/in.h>
#include <signal.h>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

void signalHandler(int signum);

class Server
{
private:
    const unsigned int port;
    int server_fd;

    void setupSocket()
    {
        struct sockaddr_in address;
        int opt = 1;

        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }

        if (listen(server_fd, 3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }
    }

    void handleClient(int clientSocket)
    {
        char buffer[1024] = {0};
        ssize_t valread = read(clientSocket, buffer, sizeof(buffer) - 1);
        if (valread < 0)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }

        Message receivedMsg = Message::deserialize(std::string(buffer));
        printf("Received: %s\n", receivedMsg.content.c_str());

        Message replyMsg(getCurrentTimestamp(), "Hello from server");
        std::string serializedMsg = replyMsg.serialize();
        send(clientSocket, serializedMsg.c_str(), serializedMsg.length(), 0);
        printf("Reply sent\n");

        close(clientSocket);
    }

public:
    Server(unsigned int port) : port(port)
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
        setupSocket();

        while (true)
        {
            struct sockaddr_in address;
            socklen_t addrlen = sizeof(address);

            int new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
            if (new_socket < 0)
            {
                perror("accept");
                continue; // Continue accepting other connections even if one fails
            }

            handleClient(new_socket);
        }

        close(server_fd);
        return 0;
    }
};

#endif