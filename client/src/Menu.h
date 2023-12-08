#pragma once
#ifndef __MENU_H__
#define __MENU_H__

#include <iostream>

enum LoginMenuOption
{
    LOGIN = 1,
    REGISTER = 2,
    LOGOUT = 3
};

enum MainMenuOption
{
    START_MESSAGING = 1,
    VIEW_PROFILE = 2,
    RUN_TESTS = 3,
    LOGOUT = 4,
    QUIT = 5
};

enum TestMenuOption
{
    RUN_BASE_MESSENGER_TESTS = 1,
    RUN_HANDLERS_TESTS = 2,
    BACK_TO_MAIN_MENU = 3
};

class Menu
{
public:
    static void showLoginMenu();
    static void showMessagingMenu();
    static void showTestMenu();
    static void runTests();

private:
    static void runTestSubMenu();
    static void runBaseMessengerTests();
    static void runHandlersTests();
};

#endif // __MENU_H__
