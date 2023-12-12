#include "./Test_Broadcast.h"

TEST_F(BroadcastMessageTest, BroadcastRequestMessageSerialization)
{
    Message request = Message::deserialize(broadcastRequest->serialize());
    EXPECT_EQ(request.connectionId, 123);
    EXPECT_EQ(request.username, "");
    EXPECT_EQ(request.target, MessageTarget::CLIENT_TO_SERVER);
    EXPECT_EQ(request.content, "BROADCAST_REQUEST Request Message");
}

TEST_F(BroadcastMessageTest, BroadcastResponseMessageSerialization)
{
    Message response = Message::deserialize(broadcastResponse->serialize());
    EXPECT_EQ(response.connectionId, 123);
    EXPECT_EQ(response.username, "");
    EXPECT_EQ(response.target, MessageTarget::BROADCAST);
    EXPECT_EQ(response.content, "BROADCAST_RESPONSE Response Message");
}
