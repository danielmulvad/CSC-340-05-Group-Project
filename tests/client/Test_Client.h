#ifndef _TEST_CLIENT_H_
#define _TEST_CLIENT_H_

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../client/src/Client.h"

class ClientTest : public ::testing::Test
{
protected:
    Client *client;

    virtual void SetUp()
    {
        client = new Client();
    }

    virtual void TearDown()
    {
        delete client;
    }
};

#endif // _TEST_CLIENT_H_