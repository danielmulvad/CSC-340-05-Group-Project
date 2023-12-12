#include "./Test_ServerMessenger.h"
#include <thread>

using testing::_;
using testing::Return;

TEST_F(ServerMessengerTest, ServerMessengerStartAndStop)
{
    std::thread serverThread([&]()
                             { try {serverMessenger->start();} catch (std::exception &e) {} });

    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    ASSERT_NO_THROW(serverMessenger->stop());

    if (serverThread.joinable())
    {
        serverThread.join();
    }
}
