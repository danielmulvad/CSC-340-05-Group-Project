#include "../../common/messages/Broadcast.h"
#include "../../common/messages/DropConnection.h"
#include "../../common/messages/EstablishConnection.h"
#include "../../common/messages/Login.h"
#include "../../common/messages/Register.h"
#include "../../common/messages/Search.h"
#include "./Server.h"

/**
 * Default constructor for Server.
 * Initializes the Server with a default port value.
 */
Server::Server() : Server(8080) {}

/**
 * Constructor for Server with a specified port.
 * Initializes the Server with the provided port, sets up the database and the messenger, and initializes handlers.
 *
 * @param port The port number to be used for the server messenger.
 */
Server::Server(const unsigned int &port) : database(new Database()), messenger(new ServerMessenger(port))
{
    this->handlers = new Handlers(this->database, this->messenger);
}

/**
 * Destructor for Server.
 * Cleans up resources, specifically deletes the database and messenger instances.
 */
Server::~Server()
{
    delete this->database;
    delete this->messenger;
}

/**
 * Starts the server.
 * Registers all necessary message handlers and starts the messenger.
 *
 * @return The result of the messenger's start operation.
 */
int Server::start()
{
    // Establish connection
    std::string establish_connection_response_handler_path = CLIENT_ESTABLISH_CONNECTION_REQUEST_PREFIX;
    auto establish_connection_response_handler = std::bind(&Handlers::handleEstablishConnectionRequest, handlers, std::placeholders::_1, std::placeholders::_2);
    this->messenger->registerHandler(establish_connection_response_handler_path, establish_connection_response_handler);

    // Drop connection
    std::string drop_connection_response_handler_path = CLIENT_DROP_CONNECTION_REQUEST_PREFIX;
    auto drop_connection_response_handler = std::bind(&Handlers::handleDropConnectionRequest, handlers, std::placeholders::_1, std::placeholders::_2);
    this->messenger->registerHandler(drop_connection_response_handler_path, drop_connection_response_handler);

    // Register
    std::string register_response_handler_path = CLIENT_REGISTER_REQUEST_PREFIX;
    auto register_response_handler = std::bind(&Handlers::handleRegisterRequest, handlers, std::placeholders::_1, std::placeholders::_2);
    this->messenger->registerHandler(register_response_handler_path, register_response_handler);

    // Login
    std::string login_response_handler_path = CLIENT_LOGIN_REQUEST_PREFIX;
    auto login_response_handler = std::bind(&Handlers::handleLoginRequest, handlers, std::placeholders::_1, std::placeholders::_2);
    this->messenger->registerHandler(login_response_handler_path, login_response_handler);

    // Broadcast
    std::string broadcast_handler_path = CLIENT_BROADCAST_REQUEST_PREFIX;
    auto broadcast_handler = std::bind(&Handlers::handleBroadcastRequest, handlers, std::placeholders::_1, std::placeholders::_2);
    this->messenger->registerHandler(broadcast_handler_path, broadcast_handler);

    // Search
    std::string search_handler_path = CLIENT_SEARCH_REQUEST_PREFIX;
    auto search_handler = std::bind(&Handlers::handleSearchRequest, handlers, std::placeholders::_1, std::placeholders::_2);
    this->messenger->registerHandler(search_handler_path, search_handler);

    return this->messenger->start();
}

/**
 * Stops the server.
 * Stops the messenger and performs any necessary cleanup.
 *
 * @return The result of the messenger's stop operation.
 */
int Server::stop()
{
    return this->messenger->stop();
}
