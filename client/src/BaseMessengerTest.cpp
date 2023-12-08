#include <gtest/gtest.h>
#include "../../common/BaseMessenger.h"
#include "../../common/Message.h"
// Mock class for testing purposes
class MockMessenger : public BaseMessenger
{
public:
    using BaseMessenger::router; // Expose router for testing
    using BaseMessenger::processReceivedMessage; // Expose processReceivedMessage for testing
};

TEST(BaseMessengerTest, RegisterHandler)
{
    MockMessenger mockMessenger;

    // Define a test handler function
    auto testHandler = [](const int& connectionId, const Message& msg) {
        ASSERT_EQ(connectionId, 1);
        ASSERT_EQ(msg.user_id, 2);
        ASSERT_EQ(msg.target, MessageTarget::CLIENT_TO_SERVER);
        ASSERT_EQ(msg.content, "testContent");
    };

    // Register the test handler
    mockMessenger.registerHandler("testPattern", testHandler);

    // Check if the handler is registered
    ASSERT_EQ(mockMessenger.router.count("testPattern"), 1);
    ASSERT_EQ(mockMessenger.router["testPattern"], testHandler);
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

