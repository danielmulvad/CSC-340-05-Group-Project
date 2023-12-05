#pragma once
#ifndef __MENU_H__
#define __MENU_H__

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

class Menu
{
public:
    static void showLoginMenu()
    {
        cout << "--------------------------------" << endl;
        cout << "Login Menu:\n";
        cout << "--------------------------------" << endl;
        cout << "   |" << LOGIN << "|" << " Login\n";
        cout << "   |" << REGISTER << "|" << " Register\n";
        cout << "   |" << LOGOUT << "|" << " Logout\n";
        cout << "--------------------------------" << endl;
    }

    static void showMainMenu()
    {
        cout << "--------------------------------" << endl;
        cout << "Main Menu:\n";
        cout << "--------------------------------" << endl;
        cout << "   |" << START_MESSAGING << "|" << " Start Messaging\n";
        cout << "   |" << QUIT << "|" << " Quit\n";
        cout << "--------------------------------" << endl;
    }
};

#endif // __MENU_H__