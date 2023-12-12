#include "../../common/messages/Broadcast.h"
#include "../../common/messages/DropConnection.h"
#include "../../common/messages/EstablishConnection.h"
#include "../../common/messages/Login.h"
#include "../../common/messages/Register.h"
#include "../../common/messages/Search.h"
#include "./Client.h"

/**
 * Default constructor for Client.
 * Initializes the Client with a default port value.
 */
Client::Client() : Client(8080) {}

/**
 * Constructor for Client with a specified port.
 * Initializes the Client with the provided port and sets up the messenger and handlers.
 *
 * @param port The port number to be used for the messenger.
 */
Client::Client(unsigned int port) : messenger(new ClientMessenger("0.0.0.0", port))
{
    handlers = new Handlers(this->messenger);
}

/**
 * Destructor for Client.
 * Cleans up resources, specifically deletes the handlers instance.
 */
Client::~Client()
{
    delete this->handlers;
}

/**
 * Starts the client.
 * Registers all necessary message handlers and starts the messenger.
 *
 * @return The result of the messenger's start operation.
 */
int Client::start()
{
    // Establish Connection
    std::string establish_connection_handler_pattern = SERVER_ESTABLISH_CONNECTION_RESPONSE_PREFIX;
    auto establish_connection_handler = std::bind(&Handlers::establishConnectionHandler, handlers, std::placeholders::_1, std::placeholders::_2);
    this->messenger->registerHandler(establish_connection_handler_pattern, establish_connection_handler);

    // Drop Connection
    std::string drop_connection_handler_pattern = SERVER_DROP_CONNECTION_RESPONSE_PREFIX;
    auto drop_connection_handler = std::bind(&Handlers::dropConnectionHandler, handlers, std::placeholders::_1, std::placeholders::_2);
    this->messenger->registerHandler(drop_connection_handler_pattern, drop_connection_handler);

    // Login
    std::string login_handler_pattern = SERVER_LOGIN_RESPONSE_PREFIX;
    auto login_handler = std::bind(&Handlers::loginHandler, handlers, std::placeholders::_1, std::placeholders::_2);
    this->messenger->registerHandler(login_handler_pattern, login_handler);

    // Register
    std::string register_handler_pattern = SERVER_REGISTER_RESPONSE_PREFIX;
    auto register_handler = std::bind(&Handlers::registerHandler, handlers, std::placeholders::_1, std::placeholders::_2);
    this->messenger->registerHandler(register_handler_pattern, register_handler);

    // Broadcast
    std::string broadcast_handler_pattern = SERVER_BROADCAST_RESPONSE_PREFIX;
    auto broadcast_handler = std::bind(&Handlers::broadcastHandler, handlers, std::placeholders::_1, std::placeholders::_2);
    this->messenger->registerHandler(broadcast_handler_pattern, broadcast_handler);

    // Search
    std::string search_handler_pattern = SERVER_SEARCH_RESPONSE_PREFIX;
    auto search_handler = std::bind(&Handlers::searchHandler, handlers, std::placeholders::_1, std::placeholders::_2);
    this->messenger->registerHandler(search_handler_pattern, search_handler);

    return this->messenger->start();
}

/**
 * Stops the client.
 * Stops the messenger and performs any necessary cleanup.
 *
 * @return The result of the messenger's stop operation.
 */
int Client::stop()
{

    return this->messenger->stop();
}
