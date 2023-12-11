#ifndef _TEST_SERVERMESSENGER_H_
#define _TEST_SERVERMESSENGER_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "../../server/src/ServerMessenger.h"

class MockServerMessenger : public ServerMessenger
{
public:
    using ServerMessenger::ServerMessenger; // Inherit constructors

public:
    MOCK_METHOD(int, createSocket, (int domain, int type, int protocol), (override));
    MOCK_METHOD(int, setSocketOptions, (int sockfd, int level, int optname, const void *optval, socklen_t optlen), (override));
    MOCK_METHOD(int, bindSocket, (int sockfd, const struct sockaddr *addr, socklen_t addrlen), (override));
    MOCK_METHOD(int, listenSocket, (int sockfd, int backlog), (override));
    MOCK_METHOD(int, acceptSocket, (int sockfd, struct sockaddr *addr, socklen_t *addrlen), (override));
    MOCK_METHOD(ssize_t, readSocket, (int sockfd, void *buf, size_t count), (override));
    MOCK_METHOD(int, closeSocket, (int sockfd), (override));
};

class ServerMessengerTest : public ::testing::Test
{

protected:
    MockServerMessenger *mockServerMessenger;
    virtual void SetUp()
    {
        mockServerMessenger = new MockServerMessenger(8080);
    }

    virtual void TearDown()
    {
        delete mockServerMessenger;
    }
};

#endif // _TEST_SERVERMESSENGER_H_