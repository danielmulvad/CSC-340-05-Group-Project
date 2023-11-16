#ifndef _SERVER_H_
#define _SERVER_H_

#include <netinet/in.h>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include "../../common/Message.h"

class Server
{
private:
    const unsigned int port = 8080;

public:
    Server(const unsigned int &port) : port(port)
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
        int server_fd, new_socket;
        struct sockaddr_in address;
        int opt = 1;
        socklen_t addrlen = sizeof(address);
        std::string hello = "Hello from server";

        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        // Attaching socket to the port 8080
        if (setsockopt(server_fd, SOL_SOCKET,
                       SO_REUSEADDR, &opt,
                       sizeof(opt)))
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(this->port);

        // Forcefully attaching socket to the port 8080
        if (bind(server_fd, (struct sockaddr *)&address,
                 sizeof(address)) < 0)
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }

        if (listen(server_fd, 3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        char buffer[1024] = {0};
        ssize_t valread = read(new_socket, buffer, 1024 - 1);
        if (valread < 0)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }
        Message receivedMsg = Message::deserialize(std::string(buffer));
        printf("%s\n", receivedMsg.content.c_str());

        std::string currentTimestamp = getCurrentTimestamp();
        Message helloMsg(currentTimestamp, "Hello from server");
        std::string serializedMsg = helloMsg.serialize();
        send(new_socket, serializedMsg.c_str(), serializedMsg.length(), 0);
        printf("Hello message sent\n");

        // closing the connected socket
        close(new_socket);
        // closing the listening socket
        close(server_fd);
        return 0;
    }
};

#endif