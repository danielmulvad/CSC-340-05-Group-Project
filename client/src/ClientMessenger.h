#ifndef _CLIENTMESSENGER_H_
#define _CLIENTMESSENGER_H_

#include "../../common/BaseMessenger.h"
#include "../../common/Message.h"
#include <arpa/inet.h>

class ClientMessenger : public BaseMessenger
{
private:
    std::string serverAddress;
    unsigned int port;
    std::mutex sendMutex;
    int socketId = 0;
    int connectionId = 0;

    void setupConnection() override
    {
        struct sockaddr_in serv_addr;
        socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_fd < 0)
        {
            throw std::runtime_error("Socket creation failed");
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);

        if (inet_pton(AF_INET, serverAddress.c_str(), &serv_addr.sin_addr) <= 0)
        {
            throw std::runtime_error("Invalid address/ Address not supported");
        }

        int socketId = connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        if (socketId < 0)
        {
            throw std::runtime_error("Connection Failed");
        }
        this->socketId = socketId;
    }

    void listenForMessages()
    {
        char buffer[1024];
        while (running)
        {
            ssize_t valread = read(socket_fd, buffer, sizeof(buffer) - 1);
            if (valread > 0)
            {
                buffer[valread] = '\0';
                Message receivedMsg = Message::deserialize(std::string(buffer));
                processReceivedMessage(connectionId, receivedMsg);
            }
        }
    }

public:
    ClientMessenger(const std::string &serverAddress, unsigned int port)
        : BaseMessenger(), serverAddress(serverAddress), port(port) {}

    virtual ~ClientMessenger()
    {
        stop();
    }

    int start() override
    {
        setupConnection();
        running = true;
        listenerThread = std::thread(&ClientMessenger::listenForMessages, this);

        EstablishConnectionRequestMessage connectionMessage(connectionId);
        sendMessageToServer(connectionMessage);

        return EXIT_SUCCESS;
    }

    int stop() override
    {
        if (running)
        {
            running = false;
            if (listenerThread.joinable())
            {
                listenerThread.join();
            }
            close(socket_fd);
        }
        return EXIT_SUCCESS;
    }

    void sendMessageToServer(const Message &msg)
    {
        std::lock_guard<std::mutex> lock(sendMutex);
        sendMessage(msg);
    }

    void registerHandler(const std::string &pattern, MessageHandler handler)
    {
        BaseMessenger::registerHandler(pattern, std::move(handler));
    }

    int getConnectionId()
    {
        return connectionId;
    }

    void setConnectionId(const int &connectionId)
    {
        this->connectionId = connectionId;
    }
};

#endif // _CLIENTMESSENGER_H_