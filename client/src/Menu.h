#ifndef _MENU_H_
#define _MENU_H_

#include <iostream>

enum LoginMenuOption
{
    LOGIN = 1,
    REGISTER = 2,
    QUIT_LOGIN = 3
};

class Menu
{
public:
    static void showLoginMenu();
};

#endif // _MENU_H_