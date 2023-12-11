#include "./Test_ServerMessenger.h"

using testing::_;
using testing::Return;

TEST_F(ServerMessengerTest, ServerMessengerStartAndStop)
{
    EXPECT_CALL(*mockServerMessenger, createSocket(_, _, _)).Times(1).WillOnce(Return(0));
    EXPECT_CALL(*mockServerMessenger, setSocketOptions(_, _, _, _, _)).Times(1).WillOnce(Return(0));
    EXPECT_CALL(*mockServerMessenger, bindSocket(_, _, _)).Times(1).WillOnce(Return(0));
    EXPECT_CALL(*mockServerMessenger, listenSocket(_, _)).Times(1).WillOnce(Return(0));
    EXPECT_CALL(*mockServerMessenger, acceptSocket(_, _, _)).WillRepeatedly(Return(0));
    std::thread serverThread([&]()
                             { mockServerMessenger->start(); });

    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    EXPECT_CALL(*mockServerMessenger, closeSocket(_)).Times(1).WillOnce(Return(0));

    ASSERT_NO_THROW(mockServerMessenger->stop());

    if (serverThread.joinable())
    {
        serverThread.join();
    }
}
