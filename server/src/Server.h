#ifndef _SERVER_H_
#define _SERVER_H_

#include "../../common/Message.h"
#include "./LinkedList.h"
#include <netinet/in.h>
#include <thread>
#include <unistd.h>

void signalHandler(int signum);

class Server
{
private:
    const unsigned int port;
    int server_fd;
    LinkedList<int> *connectedClients;
    LinkedList<Message> *messages;

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

    ssize_t sendMessageToClient(int clientSocket, Message message)
    {
        std::string serializedMsg = message.serialize();
        return send(clientSocket, serializedMsg.c_str(), serializedMsg.length(), 0);
    }

    void broadcastMessage(int senderId, Message message)
    {
        Node<int> *current = this->connectedClients->getHead();
        while (current != nullptr)
        {
            if (current->getValue() != senderId)
            {
                sendMessageToClient(current->getValue(), message);
            }
            current = current->getNext();
        }
    }

    bool handleServerMessage(int clientSocket, Message message)
    {
        if (message.content.find(CLIENT_TO_SERVER_PREFIX) != std::string::npos)
        {
            if (message.content.find(CLIENT_ESTABLISH_CONNECTION_REQUEST_PREFIX) != std::string::npos)
            {
                // Establish connection request
                int newConnectionId = clientSocket;
                EstablishConnectionResponseMessage connectionMessage(newConnectionId);
                sendMessageToClient(clientSocket, connectionMessage);
                std::cout << connectionMessage << std::endl;
                return true;
            }
        }
        return false;
    }

    void handleClient(int clientSocket)
    {
        this->connectedClients->add(clientSocket);
        while (true)
        {
            char buffer[1024] = {0};
            ssize_t valread = read(clientSocket, buffer, sizeof(buffer) - 1);
            if (valread <= 0)
            {
                // Either an error occurred or the client disconnected
                if (valread < 0)
                    perror("read");
                break;
            }

            buffer[valread] = '\0'; // Ensuring null-termination

            std::string bufferString = std::string(buffer);
            Message receivedMsg = Message::deserialize(bufferString);
            this->messages->add(receivedMsg);

            std::cout << receivedMsg << std::endl;

            if (!handleServerMessage(clientSocket, receivedMsg))
            {
                // Not a server message, so it's a client message
                broadcastMessage(clientSocket, receivedMsg);
            }

            // reset buffer
            memset(buffer, 0, sizeof(buffer));
        }

        close(clientSocket);
    }

public:
    Server() : Server(8080) {}
    Server(unsigned int port) : port(port), connectedClients(new LinkedList<int>()), messages(new LinkedList<Message>())
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
    ~Server()
    {
        delete connectedClients;
        delete messages;
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
                continue;
            }

            std::thread clientThread(&Server::handleClient, this, new_socket);
            clientThread.detach();
        }

        close(server_fd);
        return 0;
    }
};

#endif // _SERVER_H_