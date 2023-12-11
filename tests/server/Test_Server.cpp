#include "./Test_Server.h"

TEST_F(ServerTest, ConstructorAndDestructor)
{
    ASSERT_NO_THROW(Server());
}

TEST_F(ServerTest, StartAndStopServer)
{
    std::thread serverThread([&]()
                             { server->start(); });

    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    ASSERT_NO_THROW(server->stop());

    if (serverThread.joinable())
    {
        serverThread.join();
    }
}
