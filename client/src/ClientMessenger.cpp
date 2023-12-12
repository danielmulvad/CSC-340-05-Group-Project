#include "../../common/messages/EstablishConnection.h"
#include "./ClientMessenger.h"

/**
 * Constructor for ClientMessenger.
 * Initializes the messenger with the server's address and port.
 *
 * @param serverAddress The IP address of the server.
 * @param port The port number on which the server is listening.
 */
ClientMessenger::ClientMessenger(const std::string &serverAddress, unsigned int port)
    : BaseMessenger(), socket(new Socket()), connectionId(-1), socket_fd(-1), serverAddress(serverAddress), port(port) {}

/**
 * Destructor for ClientMessenger.
 * Stops the messenger and cleans up resources, specifically deletes the socket instance.
 */
ClientMessenger::~ClientMessenger()
{
    stop();
    delete socket;
}

/**
 * Sets up the connection to the server.
 * Initializes the socket and connects to the server at the specified address and port.
 * @throws std::runtime_error If socket creation or connection fails.
 */
void ClientMessenger::setupConnection()
{
    struct sockaddr_in serv_addr;
    this->socket_fd = socket->createSocket(AF_INET, SOCK_STREAM, 0);
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

    int socketId = socket->connectSocket(this->socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (socketId < 0)
    {
        throw std::runtime_error("Connection Failed");
    }
}

/**
 * Listens for messages from the server.
 * Continuously reads messages from the socket and processes them until the messenger is stopped.
 */
void ClientMessenger::listenForMessages()
{
    char buffer[1024];
    while (running)
    {
        ssize_t valread = socket->readSocket(this->socket_fd, buffer, sizeof(buffer) - 1);
        if (valread > 0)
        {
            buffer[valread] = '\0';
            Message receivedMsg = Message::deserialize(std::string(buffer));
            processReceivedMessage(connectionId, receivedMsg, false);
        }
    }
}

/**
 * Starts the messenger.
 * Sets up the connection, starts the message listener thread, and sends an establish connection message to the server.
 *
 * @return EXIT_SUCCESS on successful start.
 */
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

/**
 * Stops the messenger.
 * Stops the message listener thread, closes the socket, and notifies the exit condition variable.
 *
 * @return EXIT_SUCCESS on successful stop.
 */
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
            socket->closeSocket(this->socket_fd);
        }
    }

    exitConditionVariable.notify_all();
    return EXIT_SUCCESS;
}

/**
 * Sends a message to the server.
 * Serializes the given message and sends it over the socket.
 *
 * @param msg The message to be sent to the server.
 */
void ClientMessenger::sendMessageToServer(const Message &msg)
{
    std::string serializedMsg = msg.serialize();
    socket->sendSocket(this->socket_fd, serializedMsg.c_str(), serializedMsg.length(), 0);
}

/**
 * Registers a message handler.
 * Associates a handler function with a specific message pattern.
 *
 * @param pattern The pattern to associate with the handler.
 * @param handler The MessageHandler function to be called for the associated pattern.
 */
void ClientMessenger::registerHandler(const std::string &pattern, MessageHandler handler)
{
    BaseMessenger::registerHandler(pattern, std::move(handler));
}

/**
 * Gets the connection ID of the messenger.
 *
 * @return The current connection ID.
 */
int ClientMessenger::getConnectionId()
{
    return connectionId;
}

/**
 * Sets the connection ID of the messenger.
 *
 * @param connectionId The new connection ID to be set.
 */
void ClientMessenger::setConnectionId(const int &connectionId)
{
    this->connectionId = connectionId;
}