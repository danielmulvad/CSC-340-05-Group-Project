#include "Menu.h"
#include "BaseMessenger.h"
#include "Handlers.h"

void Menu::showLoginMenu()
{
    std::cout << "--------------------------------------------\n";
        std::cout << "Login Menu:" << std::endl;
        std::cout << "--------------------------------------------\n";
        std::cout << "  |1| Login" << std::endl;
        std::cout << "  |2| Register" << std::endl;
        std::cout << "  |3| Exit" << std::endl;
        std::cout << "--------------------------------------------\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
}

void Menu::showMessagingMenu()
{
    std::cout << "Main Menu:" << std::endl;
        std::cout << "--------------------------------------------\n";
        std::cout << "  |1| Start Messaging" << std::endl;
        std::cout << "  |2| View Profile" << std::endl;
        std::cout << "  |3| Run Tests" << std::endl;
        std::cout << "  |4| Logout" << std::endl;
        std::cout << "  |5| Exit" << std::endl;
        std::cout << "--------------------------------------------\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
}

void Menu::showTestMenu()
{
    std::cout << "--------------------------------------------\n";
    std::cout << "Test Menu:\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "   |" << RUN_BASE_MESSENGER_TESTS << "|" << " Run BaseMessenger Tests\n";
    std::cout << "   |" << RUN_HANDLERS_TESTS << "|" << " Run Handlers Tests\n";
    std::cout << "   |" << BACK_TO_MAIN_MENU << "|" << " Back to Main Menu\n";
    std::cout << "--------------------------------------------\n";
}

void Menu::runTests()
{
    int choice;

    do
    {
        std::cout << "--------------------------------------------\n";
        std::cout << "Menu:" << std::endl;
        std::cout << "--------------------------------------------\n";
        std::cout << "  |1| Show Login Menu" << std::endl;
        std::cout << "  |2| Show Main Menu" << std::endl;
        std::cout << "  |3| Run Tests" << std::endl;
        std::cout << "  |4| Exit" << std::endl;
        std::cout << "--------------------------------------------\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            showLoginMenu();
            break;
        case 2:
            showMainMenu();
            break;
        case 3:
            showTestMenu();
            int testChoice;
            std::cout << "Enter your choice: ";
            std::cin >> testChoice;

            switch (testChoice)
            {
            case RUN_BASE_MESSENGER_TESTS:
                runBaseMessengerTests();
                break;
            case RUN_HANDLERS_TESTS:
                runHandlersTests();
                break;
            case BACK_TO_MAIN_MENU:
                std::cout << "Returning to Main Menu..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
            }

            break;
        case 0:
            std::cout << "Exiting..." << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
            break;
        }

    } while (choice != 0);
}

void Menu::runBaseMessengerTests()
{
    // Code to run BaseMessenger tests
    std::cout << "Running BaseMessenger tests..." << std::endl;
        BaseMessenger baseMessenger;
        assert(baseMessenger.add(2, 3) == 5);
    std::cout << "BaseMessenger tests completed." << std::endl;
}

void Menu::runHandlersTests()
{
    // Code to run Handlers tests
    std::cout << "Running Handlers tests..." << std::endl;
        Handlers handlers;
        assert(handlers.multiply(2, 3) == 6);
    std::cout << "Handlers tests completed." << std::endl;
}
