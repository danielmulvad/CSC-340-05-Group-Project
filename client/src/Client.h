#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "../../common/Message.h"
#include "./ClientMessenger.h"
#include "./Menu.h"

class Client
{
private:
    ClientMessenger *messenger;

    std::thread messagingThread;

    std::mutex exitMutex;
    std::condition_variable exitConditionVariable;
    bool finished = false;

    void handleRegister()
    {
        std::string username, password;
        printf("Enter username: ");
        getline(std::cin, username);
        printf("Enter password: ");
        getline(std::cin, password);
        RegisterRequestMessage loginMessage(messenger->getConnectionId(), username, password);
        messenger->sendMessageToServer(loginMessage);
    }

    void handleLogin()
    {
        std::string username, password;
        printf("Enter username: ");
        getline(std::cin, username);
        printf("Enter password: ");
        getline(std::cin, password);
        LoginRequestMessage loginMessage(messenger->getConnectionId(), username, password);
        messenger->sendMessageToServer(loginMessage);
    }

    void handleStop()
    {
        DropConnectionRequestMessage dropConnectionRequestMessage(messenger->getConnectionId());
        messenger->sendMessageToServer(dropConnectionRequestMessage);
    }

    void handleMainMenu()
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

    void startMessaging()
    {
        while (true)
        {
            std::string message;
            printf("Enter message (type '/exit' to quit): ");
            getline(std::cin, message);
            if (message == "/exit")
            {
                handleStop();
                return;
            }
            BroadcastRequestMessage msg(messenger->getConnectionId(), message);
            messenger->sendMessageToServer(msg);
        }
    }

    void loginHandler(const int &connectionId, const Message &msg)
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
            messagingThread = std::thread(&Client::startMessaging, this);
            break;
        case LoginResponseCode::LOGIN_FAILED:
        default:
            printf("Login failed\n");
            handleMainMenu();
            break;
        }
    }

    void registerHandler(const int &connectionId, const Message &msg)
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
            handleMainMenu();
            break;
        }
    }

    void establishConnectionHandler(const int &connectionId, const Message &msg)
    {
        std::string content = msg.content;
        std::string delimiter = " ";
        content.erase(0, content.find(delimiter) + delimiter.length());
        content.erase(0, content.find(delimiter) + delimiter.length());
        int newConnectionId = std::stoi(content);
        messenger->setConnectionId(newConnectionId);
    }

    void dropConnectionHandler(const int &connectionId, const Message &msg)
    {
        std::unique_lock<std::mutex> lock(exitMutex);
        finished = true;
        exitConditionVariable.notify_one(); // Notify exit

        if (messagingThread.joinable())
        {
            messagingThread.join();
        }
    }

    void broadcastHandler(const int &connectionId, const Message &msg)
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
        Message broadcastMessage = Message(msg.user_id, MessageTarget::BROADCAST, msg.timestamp, content);

        std::cout << broadcastMessage << std::endl;
    }

public:
    Client() : Client(8080) {}
    Client(unsigned int port) : messenger(new ClientMessenger("0.0.0.0", port))
    {
    }

    int start()
    {
        // Establish Connection
        std::string establish_connection_handler_pattern = SERVER_ESTABLISH_CONNECTION_RESPONSE_PREFIX;
        auto establish_connection_handler = std::bind(&Client::establishConnectionHandler, this, std::placeholders::_1, std::placeholders::_2);
        messenger->registerHandler(establish_connection_handler_pattern, establish_connection_handler);

        // Drop Connection
        std::string drop_connection_handler_pattern = SERVER_DROP_CONNECTION_RESPONSE_PREFIX;
        auto drop_connection_handler = std::bind(&Client::dropConnectionHandler, this, std::placeholders::_1, std::placeholders::_2);
        messenger->registerHandler(drop_connection_handler_pattern, drop_connection_handler);

        // Login
        std::string login_handler_pattern = SERVER_LOGIN_RESPONSE_PREFIX;
        auto login_handler = std::bind(&Client::loginHandler, this, std::placeholders::_1, std::placeholders::_2);
        messenger->registerHandler(login_handler_pattern, login_handler);

        // Register
        std::string register_handler_pattern = SERVER_REGISTER_RESPONSE_PREFIX;
        auto register_handler = std::bind(&Client::registerHandler, this, std::placeholders::_1, std::placeholders::_2);
        messenger->registerHandler(register_handler_pattern, register_handler);

        // Broadcast
        std::string broadcast_handler_pattern = SERVER_BROADCAST_RESPONSE_PREFIX;
        auto broadcast_handler = std::bind(&Client::broadcastHandler, this, std::placeholders::_1, std::placeholders::_2);
        messenger->registerHandler(broadcast_handler_pattern, broadcast_handler);

        messenger->start();

        handleMainMenu();

        // Wait for the exit signal
        std::unique_lock<std::mutex> lock(exitMutex);
        exitConditionVariable.wait(lock, [this]
                                   { return finished; });

        return EXIT_SUCCESS;
    }

    int stop()
    {

        if (messagingThread.joinable())
        {
            messagingThread.join();
        }
        return messenger->stop();
    }

    ~Client()
    {
        delete messenger;
    }
};

#endif // _CLIENT_H_
