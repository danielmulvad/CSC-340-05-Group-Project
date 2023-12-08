#ifndef __MENU_H__
#define __MENU_H__

#include <iostream>
#include "BaseMessengerTest.h" // Include the test file

enum LoginMenuOption
{
    LOGIN = 1,
    REGISTER = 2,
    LOGOUT = 3
};

enum MainMenuOption
{
    START_MESSAGING = 1,
    QUIT = 2
};

enum TestMenuOption
{
    RUN_BASE_MESSENGER_TESTS = 1,
    BACK_TO_MAIN_MENU = 2
};

class Menu
{
public:
    static void showLoginMenu();
    static void showMessagingMenu();
    static void showTestMenu();
    static void runTests();

private:
    static void runBaseMessengerTests();
    static void runHandlersTests();
};

#endif // __MENU_H__
