#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "../../common/Message.h"
#include "./Menu.h"
#include "./Messenger.h"
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>

class Client
{
private:
    Messenger *messenger;

    void handleRegister()
    {
        std::string username, password;
        printf("Enter username: ");
        std::cin >> username;
        printf("Enter password: ");
        std::cin >> password;
        LoginRequestMessage loginMessage(messenger->getConnectionId(), username, password);
        messenger->sendMessageToServer(loginMessage);
    }

    void handleLogin()
    {
        std::string username, password;
        printf("Enter username: ");
        std::cin >> username;
        printf("Enter password: ");
        std::cin >> password;
        std::string loginMessage = CLIENT_TO_SERVER_PREFIX + " " + CLIENT_LOGIN_PREFIX + " " + username + " " + password;
        Message msg(messenger->getConnectionId(), loginMessage);
        messenger->sendMessageToServer(msg);
    }

    void handleStop()
    {
        messenger->stop();
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

    void loginHandler(const Message &msg)
    {
        std::cout << "Pattern matched: " << msg.content << std::endl;
    }

public:
    Client() : Client(8080) {}
    Client(unsigned int port) : messenger(new Messenger(port))
    {
        std::string login_handler_pattern = CLIENT_TO_SERVER_PREFIX + " " + CLIENT_LOGIN_PREFIX;
        messenger->registerHandler(login_handler_pattern, std::bind(&Client::loginHandler, this, std::placeholders::_1));
        messenger->start();
        handleMainMenu();
    }

    ~Client()
    {
        delete messenger;
    }
};

#endif // _CLIENT_H_
