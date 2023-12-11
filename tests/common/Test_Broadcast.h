#ifndef _TEST_BROADCAST_H_
#define _TEST_BROADCAST_H_

#include <gtest/gtest.h>
#include "../../common/messages/Broadcast.h"

class BroadcastMessageTest : public ::testing::Test
{
protected:
    BroadcastRequestMessage *broadcastRequest;
    BroadcastResponseMessage *broadcastResponse;

    virtual void SetUp()
    {
        broadcastRequest = new BroadcastRequestMessage(123, "Request Message");
        broadcastResponse = new BroadcastResponseMessage(123, "Response Message");
    }

    virtual void TearDown()
    {
        delete broadcastRequest;
        delete broadcastResponse;
    }
};

#endif // _TEST_BROADCAST_H_