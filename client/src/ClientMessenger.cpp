#include "../../common/messages/EstablishConnection.h"
#include "./ClientMessenger.h"

ClientMessenger::ClientMessenger(const std::string &serverAddress, unsigned int port)
    : BaseMessenger(), serverAddress(serverAddress), port(port)
{
}

ClientMessenger::~ClientMessenger()
{
    stop();
}

void ClientMessenger::setupConnection()
{
    struct sockaddr_in serv_addr;
    this->socket_fd = createSocket(AF_INET, SOCK_STREAM, 0);
    if (this->socket_fd < 0)
    {
        throw std::runtime_error("Socket creation failed");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, this->serverAddress.c_str(), &serv_addr.sin_addr) <= 0)
    {
        throw std::runtime_error("Invalid address or address not supported");
    }

    int socketId = connectSocket(this->socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (socketId < 0)
    {
        throw std::runtime_error("Connection Failed");
    }
}

void ClientMessenger::listenForMessages()
{
    char buffer[1024];
    while (running)
    {
        ssize_t valread = readSocket(this->socket_fd, buffer, sizeof(buffer) - 1);
        if (valread > 0)
        {
            buffer[valread] = '\0';
            Message receivedMsg = Message::deserialize(std::string(buffer));
            processReceivedMessage(connectionId, receivedMsg, false);
        }
    }
}

int ClientMessenger::start()
{
    setupConnection();
    running = true;
    listenerThread = std::thread(&ClientMessenger::listenForMessages, this);

    EstablishConnectionRequestMessage connectionMessage(connectionId, "");
    sendMessageToServer(connectionMessage);

    std::mutex mutex;
    std::unique_lock<std::mutex> lock(mutex);
    exitConditionVariable.wait(lock);

    return EXIT_SUCCESS;
}

int ClientMessenger::stop()
{
    if (running)
    {
        running = false;

        if (std::this_thread::get_id() != listenerThread.get_id() && listenerThread.joinable())
        {
            listenerThread.join();
        }

        if (this->socket_fd > 0)
        {
            closeSocket(this->socket_fd);
        }
    }

    exitConditionVariable.notify_all();
    return EXIT_SUCCESS;
}

void ClientMessenger::sendMessageToServer(const Message &msg)
{
    std::string serializedMsg = msg.serialize();
    sendSocket(this->socket_fd, serializedMsg.c_str(), serializedMsg.length(), 0);
}

void ClientMessenger::registerHandler(const std::string &pattern, MessageHandler handler)
{
    BaseMessenger::registerHandler(pattern, std::move(handler));
}

int ClientMessenger::getConnectionId()
{
    return connectionId;
}

void ClientMessenger::setConnectionId(const int &connectionId)
{
    this->connectionId = connectionId;
}