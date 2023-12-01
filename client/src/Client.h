#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "../../common/Message.h"
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>

class Client
{
private:
    bool isConnectionEstablished = false;
    bool running;
    const unsigned int port;
    int connectionId;
    int socket_fd;
    std::condition_variable connectionEstablishedCondition;
    std::mutex connectionMutex;
    std::mutex consoleMutex;

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
        return;
    }

    void sendEstablishConnectionRequest()
    {
        std::string connectionMessage = CLIENT_TO_SERVER_PREFIX + " " + CLIENT_ESTABLISH_CONNECTION_REQUEST_PREFIX;
        Message helloMsg(connectionId, connectionMessage);
        sendMessageToServer(helloMsg);
        printf("Establish connection request sent\n");
    }

    bool handleServerToClientMessage(const Message &msg)
    {
        std::string connectionResponseMessage = SERVER_TO_CLIENT_PREFIX + " " + SERVER_ESTABLISH_CONNECTION_RESPONSE_PREFIX;
        if (msg.content.find(connectionResponseMessage) != std::string::npos)
        {
            std::string connectionIdStr = msg.content.substr(connectionResponseMessage.length() + 1);
            connectionId = std::stoi(connectionIdStr);
            printf("Connection established with id %d\n", connectionId);

            std::lock_guard<std::mutex> lock(connectionMutex);
            isConnectionEstablished = true;
            connectionEstablishedCondition.notify_one();

            return true;
        }
        return false;
    }

    void listenForMessages()
    {
        fd_set readfds;
        struct timeval tv;

        while (running)
        {
            FD_ZERO(&readfds);
            FD_SET(socket_fd, &readfds);

            // Set timeout to 1 second
            tv.tv_sec = 1;
            tv.tv_usec = 0;

            int activity = select(socket_fd + 1, &readfds, NULL, NULL, &tv);

            if (activity < 0 && errno != EINTR)
            {
                perror("select error");
                break;
            }

            if (FD_ISSET(socket_fd, &readfds))
            {
                char buffer[1024] = {0};
                ssize_t valread = read(socket_fd, buffer, sizeof(buffer) - 1);
                if (valread > 0)
                {
                    Message receivedMsg = Message::deserialize(std::string(buffer));
                    if (!handleServerToClientMessage(receivedMsg))
                    {
                        // Not a server to client message
                        printf("\n%s\n", receivedMsg.toString().c_str());
                    }
                    fflush(stdout);
                }
            }
        }
    }

public:
    Client() : Client(8080) {}
    Client(unsigned int port) : running(true), port(port), connectionId(0)
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

    ~Client()
    {
        running = false;
        if (socket_fd > 0)
        {
            close(socket_fd);
        }
    }

    void sendMessageToServer(const Message &msg)
    {
        std::string serializedMsg = msg.serialize();
        send(socket_fd, serializedMsg.c_str(), serializedMsg.length(), 0);
    }

    void checkForInput()
    {
        while (running)
        {
            std::unique_lock<std::mutex> lock(consoleMutex);
            std::cout << "Enter message (type 'exit' to quit): ";
            std::string inputMsg;
            if (std::getline(std::cin, inputMsg))
            {
                sendMessageToServer(Message(connectionId, inputMsg));
            }
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    int run()
    {
        setupConnection();

        std::thread listenerThread(&Client::listenForMessages, this);

        sendEstablishConnectionRequest();

        {
            std::unique_lock<std::mutex> lock(connectionMutex);
            connectionEstablishedCondition.wait(lock, [this]()
                                                { return isConnectionEstablished; });
        }

        while (true)
        {
            std::string inputMsg;
            std::cout << "Enter message (type 'exit' to quit): ";
            std::getline(std::cin, inputMsg);

            if (inputMsg == "exit")
                break;

            Message helloMsg(connectionId, inputMsg);
            sendMessageToServer(helloMsg);
        }

        running = false;
        listenerThread.join();
        return 0;
    }
};

#endif
