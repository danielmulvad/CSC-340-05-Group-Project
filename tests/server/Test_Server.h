#ifndef _TEST_SERVER_H_
#define _TEST_SERVER_H_

#include <gtest/gtest.h>
#include "../../server/src/Server.h"

class ServerTest : public ::testing::Test
{
protected:
    Server *server;

    virtual void SetUp()
    {
        server = new Server();
    }

    virtual void TearDown()
    {
        delete server;
    }
};

#endif // _TEST_SERVER_H_
