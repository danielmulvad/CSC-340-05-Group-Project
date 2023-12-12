#ifndef _TEST_SOCKET_H_
#define _TEST_SOCKET_H_

#include "../../common/Socket.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

class MockSocket : public Socket
{
public:
    MOCK_METHOD(int, acceptSocket, (int sockfd, struct sockaddr *addr, socklen_t *addrlen), (override));
    MOCK_METHOD(int, bindSocket, (int sockfd, const struct sockaddr *addr, socklen_t addrlen), (override));
    MOCK_METHOD(int, closeSocket, (int sockfd), (override));
    MOCK_METHOD(int, connectSocket, (int sockfd, const struct sockaddr *addr, socklen_t addrlen), (override));
    MOCK_METHOD(int, createSocket, (int domain, int type, int protocol), (override));
    MOCK_METHOD(int, listenSocket, (int sockfd, int backlog), (override));
    MOCK_METHOD(int, sendSocket, (int sockfd, const void *buf, size_t len, int flags), (override));
    MOCK_METHOD(int, setSocketOptions, (int sockfd, int level, int optname, const void *optval, socklen_t optlen), (override));
    MOCK_METHOD(ssize_t, readSocket, (int sockfd, void *buf, size_t count), (override));
};

class TestSocket : public ::testing::Test
{
protected:
    MockSocket *mockSocket;

    void SetUp() override
    {
        mockSocket = new MockSocket();
    }

    void TearDown() override
    {
        delete mockSocket;
    }
};

#endif // _TEST_SOCKET_H_
