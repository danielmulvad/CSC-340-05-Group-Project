#include "../../common/messages/Broadcast.h"
#include "../../common/messages/DropConnection.h"
#include "../../common/messages/EstablishConnection.h"
#include "../../common/messages/Login.h"
#include "../../common/messages/Register.h"
#include "../../common/messages/Search.h"
#include "./Handlers.h"

Handlers::Handlers(ClientMessenger *messenger) : messenger(messenger) {}
Handlers::~Handlers()
{
    delete messenger;
}

void Handlers::handleRegister()
{
    std::string username, password;
    printf("Enter username: ");
    getline(std::cin, username);
    printf("Enter password: ");
    getline(std::cin, password);
    RegisterRequestMessage loginMessage(messenger->getConnectionId(), username, password);
    messenger->sendMessageToServer(loginMessage);
}

void Handlers::handleLogin()
{
    std::string username, password;
    printf("Enter username: ");
    getline(std::cin, username);
    printf("Enter password: ");
    getline(std::cin, password);
    LoginRequestMessage loginMessage(messenger->getConnectionId(), username, password);
    messenger->sendMessageToServer(loginMessage);
}

void Handlers::handleStop()
{
    DropConnectionRequestMessage dropConnectionRequestMessage(messenger->getConnectionId(), username);
    messenger->sendMessageToServer(dropConnectionRequestMessage);
}

void Handlers::handleSearch(std::string &message)
{
    std::string searchQuery = message.substr(strlen("/search") + 1);
    SearchRequestMessage searchRequestMessage(messenger->getConnectionId(), username, searchQuery);
    messenger->sendMessageToServer(searchRequestMessage);
}

void Handlers::startMessaging()
{
    printf("Enter message (type '/exit' to quit):\n");
    while (true)
    {
        std::string message;
        getline(std::cin, message);
        if (message == "/exit")
        {
            handleStop();
            return;
        }
        if (message.find("/search") != std::string::npos)
        {
            handleSearch(message);
            continue;
        }
        BroadcastRequestMessage msg(messenger->getConnectionId(), username, message);
        messenger->sendMessageToServer(msg);
    }
}

void Handlers::handleMainMenu()
{
    int option;

    Menu::showLoginMenu();
    printf("Enter option: ");
    std::cin >> option;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    switch (option)
    {
    case LoginMenuOption::LOGIN:
        handleLogin();
        break;
    case LoginMenuOption::REGISTER:
        handleRegister();
        break;
    case LoginMenuOption::QUIT_LOGIN:
        handleStop();
        break;
    default:
        printf("Invalid option\n");
        handleMainMenu();
        break;
    }
}

void Handlers::loginHandler(const int &connectionId, const Message &msg)
{
    std::vector<std::string> parts;
    std::stringstream ss(msg.content);
    std::string part;

    while (std::getline(ss, part, ' '))
    {
        parts.push_back(part);
    }

    const size_t expectedParts = 2;
    if (parts.size() != expectedParts)
    {
        printf("Invalid serialized message format for %s\n", msg.serialize().c_str());
        return;
    }

    LoginResponseCode responseCode = static_cast<LoginResponseCode>(std::stoi(parts[1]));
    switch (responseCode)
    {
    case LoginResponseCode::LOGIN_SUCCESS:
        printf("Login successful\n");
        messagingThread = std::thread(&Handlers::startMessaging, this);
        this->username = msg.username;
        break;
    case LoginResponseCode::LOGIN_FAILED:
    default:
        printf("Login failed\n");
        handleMainMenu();
        break;
    }
}

void Handlers::registerHandler(const int &connectionId, const Message &msg)
{
    std::vector<std::string> parts;
    std::stringstream ss(msg.content);
    std::string part;

    while (std::getline(ss, part, ' '))
    {
        parts.push_back(part);
    }

    const size_t expectedParts = 2;
    if (parts.size() != expectedParts)
    {
        printf("Invalid serialized message format for %s\n", msg.serialize().c_str());
        return;
    }

    RegisterResponseCode responseCode = static_cast<RegisterResponseCode>(std::stoi(parts[1]));
    switch (responseCode)
    {
    case RegisterResponseCode::REGISTER_SUCCESS:
        printf("Register successful\n");
        break;
    case RegisterResponseCode::REGISTER_FAILED:
    default:
        printf("Register failed\n");
        break;
    }
    handleMainMenu();
}

void Handlers::establishConnectionHandler(const int &connectionId, const Message &msg)
{
    std::string content = msg.content;
    std::string delimiter = " ";
    content.erase(0, content.find(delimiter) + delimiter.length());
    content.erase(0, content.find(delimiter) + delimiter.length());
    int newConnectionId = std::stoi(content);
    messenger->setConnectionId(newConnectionId);
    handleMainMenu();
}

void Handlers::dropConnectionHandler(const int &connectionId, const Message &msg)
{
    if (messagingThread.joinable() && std::this_thread::get_id() != messagingThread.get_id())
    {
        messagingThread.join();
    }
    messenger->stop();
}

void Handlers::broadcastHandler(const int &connectionId, const Message &msg)
{
    std::vector<std::string> parts;
    std::stringstream ss(msg.content);
    std::string part;

    while (std::getline(ss, part, ' '))
    {
        parts.push_back(part);
    }

    std::string content = msg.content;
    content.erase(0, strlen(SERVER_BROADCAST_RESPONSE_PREFIX.c_str()) + 1);
    Message broadcastMessage = Message(msg.connectionId, msg.username, MessageTarget::BROADCAST, msg.timestamp, content);

    std::cout << broadcastMessage << std::endl;
}

void Handlers::searchHandler(const int &connectionId, const Message &msg)
{
    std::vector<std::string> parts;
    std::stringstream ss(msg.content);
    std::string part;

    while (std::getline(ss, part, ' '))
    {
        parts.push_back(part);
    }

    std::string content = msg.content;
    content.erase(0, strlen(SERVER_SEARCH_RESPONSE_PREFIX.c_str()) + 1);
    Message searchMessage = Message(msg.connectionId, msg.username, MessageTarget::BROADCAST, msg.timestamp, content);

    std::cout << searchMessage << std::endl;
}
