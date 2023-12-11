#include "./Test_Client.h"

TEST_F(ClientTest, ConstructorAndDestructor)
{
    ASSERT_NO_THROW(Client());
}

TEST_F(ClientTest, StartMethod)
{
    // client->start() should fail with Connection Failed
    ASSERT_THROW(client->start(), std::runtime_error);
}

TEST_F(ClientTest, StopMethod)
{
    ASSERT_NO_THROW(client->stop());
}
