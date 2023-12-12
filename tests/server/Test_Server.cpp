#include "./Test_Server.h"
#include <thread>

TEST_F(ServerTest, ConstructorAndDestructor)
{
    ASSERT_NO_THROW(Server());
}

TEST_F(ServerTest, StartAndStopServer)
{
    std::thread serverThread([&]()
                             { try {server->start();} catch (std::exception &e) {} });

    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    ASSERT_NO_THROW(server->stop());

    if (serverThread.joinable())
    {
        serverThread.join();
    }
}
