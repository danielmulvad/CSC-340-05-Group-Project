#ifndef __MENU_H__
#define __MENU_H__

enum LoginMenuOption
{
    LOGIN = 1,
    REGISTER = 2,
    QUIT_LOGIN = 3,
    PROMPT_UNIT_TEST = 4
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
        printf("4. Start Unit Test\n");
    }
};

#endif // __MENU_H__