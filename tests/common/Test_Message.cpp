#include "./Test_Message.h"

TEST_F(MessageTest, deserialize)
{
    std::string serialized = "123;;3;2023-04-20 12:00:00;Hello World";
    Message msg = Message::deserialize(serialized);
    EXPECT_EQ(msg.connectionId, 123);
    EXPECT_EQ(msg.username, "");
    EXPECT_EQ(msg.target, MessageTarget::BROADCAST);
    EXPECT_EQ(msg.timestamp, "2023-04-20 12:00:00");
    EXPECT_EQ(msg.content, "Hello World");
}

TEST_F(MessageTest, Serialize)
{
    int connectionId = 123;
    std::string username = "";
    MessageTarget target = MessageTarget::BROADCAST;
    std::string timestamp = "2023-04-20 12:00:00";
    std::string content = "Hello World";
    Message msg(connectionId, username, target, timestamp, content);

    std::string serialized = msg.serialize();

    std::string expected = "123;;3;2023-04-20 12:00:00;Hello World";

    EXPECT_EQ(serialized, expected);
}

TEST_F(MessageTest, toString)
{
    Message msg(123, "username", MessageTarget::BROADCAST, "2023-04-20 12:00:00", "Hello World");
    std::string expected = "[123][2023-04-20 12:00:00] username: Hello World";
    EXPECT_EQ(msg.toString(), expected);
}
