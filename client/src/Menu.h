#ifndef __MENU_H__
#define __MENU_H__

#include <stdio.h>

enum LoginMenuOption
{
    LOGIN = 1,
    REGISTER = 2,
    QUIT_LOGIN = 3
};

enum MainMenuOption
{
    START_MESSAGING = 1,
    QUIT = 2
};

class Menu
{
public:
    static void showLoginMenu()
    {
        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. Quit\n");
    }

    static void showRegistrationMenu()
    {
        printf("1. Register\n");
        printf("2. Quit\n");
    }
};

#endif // __MENU_H__