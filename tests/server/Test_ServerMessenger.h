#ifndef _TEST_SERVERMESSENGER_H_
#define _TEST_SERVERMESSENGER_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "../../server/src/ServerMessenger.h"

class ServerMessengerTest : public ::testing::Test
{
protected:
    ServerMessenger *serverMessenger;

    virtual void SetUp()
    {
        serverMessenger = new ServerMessenger(8080);
    }

    virtual void TearDown()
    {
        delete serverMessenger;
    }
};

#endif // _TEST_SERVERMESSENGER_H_