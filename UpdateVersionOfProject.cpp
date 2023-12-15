#include <iostream>
#include "userManagementSystem.h"
#include "User.h"

using namespace std;

enum Actions {
    QUIT,
    LOGIN,
};
const int max_menu_option = LOGIN;
UserManagementSystem mySystem;
int getMenuChoice() {
    int choice;

    do {
        cout << "--------------------------------------------\n";
        cout << "                 *Welcome!*                      \n";
        cout << "--------------------------------------------\n";
        cout << "By logging in, you agree to our \nterms and conditions, and acknowledge \nour privacy policy.\n";
        cout << "--------------------------------------------\n";
        cout << "Option Menu:\n";
        cout << "  |" << QUIT << "| Logout\n";
        cout << "  |" << LOGIN << "| Login\n";
        cout << "Enter a number here: ";

        if (!(cin >> choice) || choice < 0 || choice > max_menu_option) {
            cout << "Error! Input must be a number between 1 and " << max_menu_option << ", or 0 to exit.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear the input buffer
        }
        else {
            break;
        }
    } while (true);

    return choice;
}

int main() {
    int choice;

    do {
        choice = getMenuChoice();

        switch (choice) {
        case QUIT:
            cout << "Thank you! Bye!\n";
            break;
        case LOGIN:
            mySystem.displayMenu();
            break;
        default:
            cout << "Coming soon!\n";
            break;
        }
    } while (choice != QUIT);

    return 0;
}
