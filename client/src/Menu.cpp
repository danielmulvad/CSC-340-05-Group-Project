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
        std::cout << "  |3| Logout" << std::endl;
        std::cout << "--------------------------------------------\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
}

void Menu::showMessagingMenu()
{
    std::cout << "--------------------------------------------\n";
        std::cout << "Messaging Menu:" << std::endl;
        std::cout << "--------------------------------------------\n";
        std::cout << "  |1| Start Messaging" << std::endl;
        std::cout << "  |2| Exit" << std::endl;
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

TEST(BaseMessengerTest, ProcessReceivedMessage)
{
    MockMessenger mockMessenger;

    // Define a flag to track if the test handler is called
    bool testHandlerCalled = false;

    // Define a test handler function
    auto testHandler = [&testHandlerCalled](const int& connectionId, const Message& msg) {
        // Perform some test-specific actions
        testHandlerCalled = true;

        // Add assertions to check parameters received by the handler
        ASSERT_EQ(connectionId, 1);
        ASSERT_EQ(msg.user_id, 1);  // Adjust user_id based on your test case
        ASSERT_EQ(msg.target, MessageTarget::CLIENT_TO_SERVER);
        ASSERT_EQ(msg.content, "testContent");
    };

    // Register the test handler
    mockMessenger.router["testPattern"] = testHandler;

    // Create a test message
    Message testMessage(1, MessageTarget::CLIENT_TO_SERVER, "testContent");

    // Process the test message
    mockMessenger.processReceivedMessage(1, testMessage, false);

    // Check that the test handler was called
    ASSERT_TRUE(testHandlerCalled);

    // If you can add more test cases as needed
}
