#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "../../common/Message.h"
#include "./ClientMessenger.h"
#include "./Menu.h"

class Client
{
private:
    ClientMessenger *messenger;

    void handleRegister()
    {
        std::string username, password;
        printf("Enter username: ");
        std::cin >> username;
        printf("Enter password: ");
        std::cin >> password;
        RegisterRequestMessage loginMessage(messenger->getConnectionId(), username, password);
        messenger->sendMessageToServer(loginMessage);
    }

    void handleLogin()
    {
        std::string username, password;
        printf("Enter username: ");
        std::cin >> username;
        printf("Enter password: ");
        std::cin >> password;
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
        do
        {
            Menu::showLoginMenu();
            printf("Enter option: ");
            std::cin >> option;
            switch (option)
            {
            case LOGIN:
                handleLogin();
                break;
            case REGISTER:
                handleRegister();
                break;
            case QUIT_LOGIN:
                handleStop();
                break;
            default:
                printf("Invalid option\n");
                break;
            }
        } while (option != QUIT_LOGIN);
    }

    void loginHandler(const int &connectionId, const Message &msg)
    {
        std::cout << "Pattern matched: " << connectionId << msg << std::endl;

        // TODO: Handle possible error code. Once logged in, move to chat
    }

    void registerHandler(const int &connectionId, const Message &msg)
    {
        std::cout << "Pattern matched: " << connectionId << msg << std::endl;

        // TODO: Handle possible error code
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
        std::cout << "Pattern matched: " << msg << std::endl;
        messenger->stop();
    }

public:
    Client() : Client(8080) {}
    Client(unsigned int port) : messenger(new ClientMessenger("0.0.0.0", port))
    {
    }

    int start()
    {
        std::string establish_connection_handler_pattern = SERVER_TO_CLIENT_PREFIX + " " + SERVER_ESTABLISH_CONNECTION_RESPONSE_PREFIX;
        auto establish_connection_handler = std::bind(&Client::establishConnectionHandler, this, std::placeholders::_1, std::placeholders::_2);
        messenger->registerHandler(establish_connection_handler_pattern, establish_connection_handler);

        std::string drop_connection_handler_pattern = SERVER_TO_CLIENT_PREFIX + " " + SERVER_DROP_CONNECTION_RESPONSE_PREFIX;
        auto drop_connection_handler = std::bind(&Client::dropConnectionHandler, this, std::placeholders::_1, std::placeholders::_2);
        messenger->registerHandler(drop_connection_handler_pattern, drop_connection_handler);

        std::string login_handler_pattern = SERVER_TO_CLIENT_PREFIX + " " + SERVER_LOGIN_RESPONSE_PREFIX;
        auto login_handler = std::bind(&Client::loginHandler, this, std::placeholders::_1, std::placeholders::_2);
        messenger->registerHandler(login_handler_pattern, login_handler);

        std::string register_handler_pattern = SERVER_TO_CLIENT_PREFIX + " " + SERVER_REGISTER_RESPONSE_PREFIX;
        auto register_handler = std::bind(&Client::registerHandler, this, std::placeholders::_1, std::placeholders::_2);
        messenger->registerHandler(register_handler_pattern, register_handler);

        messenger->start();

        handleMainMenu();
        return EXIT_SUCCESS;
    }

    int stop()
    {

        return messenger->stop();
    }

    ~Client()
    {
        delete messenger;
    }
};

#endif // _CLIENT_H_
