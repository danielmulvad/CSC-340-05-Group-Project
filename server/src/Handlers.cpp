#include "./Handlers.h"
#include <thread>

/**
 * Constructor for Handlers.
 * Initializes Handlers with references to the database and the server messenger.
 *
 * @param database Pointer to the database used for user and message management.
 * @param messenger Pointer to the server messenger for communication with clients.
 */
Handlers::Handlers(Database *database, ServerMessenger *messenger) : database(database), messenger(messenger) {}

/**
 * Destructor for Handlers.
 * Cleans up resources by deleting the database and messenger instances.
 */
Handlers::~Handlers()
{
    delete database;
    delete messenger;
}

/**
 * Handles the establish connection request from a client.
 * Sends an establish connection response back to the client.
 *
 * @param connectionId The ID of the client connection.
 * @param msg The received message.
 */
void Handlers::handleEstablishConnectionRequest(const int &connectionId, const Message &msg)
{
    EstablishConnectionResponseMessage response(connectionId, msg.username);
    messenger->sendMessageToClient(connectionId, response);
    return;
}

/**
 * Handles the drop connection request from a client.
 * Sends a drop connection response back to the client.
 *
 * @param connectionId The ID of the client connection.
 * @param msg The received message.
 */
void Handlers::handleDropConnectionRequest(const int &connectionId, const Message &msg)
{
    DropConnectionResponseMessage response(connectionId, msg.username);
    messenger->sendMessageToClient(connectionId, response);
    return;
}

/**
 * Handles the register request from a client.
 * Processes the registration details and sends a register response to the client.
 *
 * @param connectionId The ID of the client connection.
 * @param msg The received message containing registration details.
 */
void Handlers::handleRegisterRequest(const int &connectionId, const Message &msg)
{
    std::vector<std::string> parts;
    std::stringstream ss(msg.content);
    std::string part;

    while (std::getline(ss, part, ' '))
    {
        parts.push_back(part);
    }

    const size_t expectedParts = 3;
    if (parts.size() != expectedParts)
    {
        printf("Invalid serialized message format for %s\n", msg.serialize().c_str());
        RegisterResponseMessage response(connectionId, msg.username, RegisterResponseCode::REGISTER_FAILED);
        messenger->sendMessageToClient(connectionId, response);
        return;
    }

    std::string username = parts[1];
    std::string password = parts[2];
    if (username.empty() || password.empty())
    {
        printf("Invalid username or password\n");
        RegisterResponseMessage response(connectionId, msg.username, RegisterResponseCode::REGISTER_FAILED);
        messenger->sendMessageToClient(connectionId, response);
        return;
    }

    bool ok = this->database->add(username, password);
    if (!ok)
    {
        RegisterResponseMessage response(connectionId, username, RegisterResponseCode::REGISTER_FAILED);
        messenger->sendMessageToClient(connectionId, response);
        return;
    }

    RegisterResponseMessage response(connectionId, username, RegisterResponseCode::REGISTER_SUCCESS);
    messenger->sendMessageToClient(connectionId, response);
}

/**
 * Handles the login request from a client.
 * Verifies login credentials and sends a login response to the client.
 *
 * @param connectionId The ID of the client connection.
 * @param msg The received message containing login details.
 */
void Handlers::handleLoginRequest(const int &connectionId, const Message &msg)
{

    std::vector<std::string> parts;
    std::stringstream ss(msg.content);
    std::string part;

    while (std::getline(ss, part, ' '))
    {
        parts.push_back(part);
    }

    const size_t expectedParts = 3;
    if (parts.size() != expectedParts)
    {
        printf("Invalid serialized message format for %s\n", msg.serialize().c_str());
        LoginResponseMessage response(connectionId, msg.username, LoginResponseCode::LOGIN_FAILED);
        messenger->sendMessageToClient(connectionId, response);
        return;
    }

    std::string username = parts[1];
    std::string password = parts[2];
    if (username.empty() || password.empty())
    {
        printf("Invalid username or password\n");
        LoginResponseMessage response(connectionId, msg.username, LoginResponseCode::LOGIN_FAILED);
        messenger->sendMessageToClient(connectionId, response);
        return;
    }
    bool ok = this->database->contains(username, password);
    if (!ok)
    {
        LoginResponseMessage response(connectionId, username, LoginResponseCode::LOGIN_FAILED);
        messenger->sendMessageToClient(connectionId, response);
        return;
    }

    LoginResponseMessage response(connectionId, username, LoginResponseCode::LOGIN_SUCCESS);
    messenger->sendMessageToClient(connectionId, response);
}

/**
 * Handles the broadcast request from a client.
 * Adds the message to the server's message list and broadcasts it to other clients.
 *
 * @param connectionId The ID of the client connection.
 * @param msg The message to be broadcast.
 */
void Handlers::handleBroadcastRequest(const int &connectionId, const Message &msg)
{
    std::string content = msg.content;
    content.erase(0, strlen(CLIENT_BROADCAST_REQUEST_PREFIX.c_str()) + 1);

    this->messenger->addMessage(msg);

    BroadcastResponseMessage broadcastResponse(connectionId, msg.username, content);
    messenger->broadcastMessage(connectionId, broadcastResponse);
}

/**
 * Handles the search request from a client.
 * Searches for messages containing the specified content and sends them back to the client.
 *
 * @param connectionId The ID of the client connection.
 * @param msg The message containing the search query.
 */
void Handlers::handleSearchRequest(const int &connectionId, const Message &msg)
{
    std::string content = msg.content;
    content.erase(0, strlen(CLIENT_SEARCH_REQUEST_PREFIX.c_str()) + 1);

    std::vector<Message> messages = this->messenger->getMessages(content);
    for (auto message : messages)
    {
        SearchResponseMessage searchResponse(message);
        messenger->sendMessageToClient(connectionId, searchResponse);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
