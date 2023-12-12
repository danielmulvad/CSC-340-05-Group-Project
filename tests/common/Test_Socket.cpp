#include "./Test_Socket.h"

TEST_F(TestSocket, TestAcceptSocket)
{
    int sockfd = 1;
    struct sockaddr *addr = nullptr;
    socklen_t *addrlen = nullptr;
    EXPECT_CALL(*mockSocket, acceptSocket(sockfd, addr, addrlen)).Times(1);
    mockSocket->acceptSocket(sockfd, addr, addrlen);
}

TEST_F(TestSocket, TestBindSocket)
{
    int sockfd = 1;
    const struct sockaddr *addr = nullptr;
    socklen_t addrlen = 0;
    EXPECT_CALL(*mockSocket, bindSocket(sockfd, addr, addrlen)).Times(1);
    mockSocket->bindSocket(sockfd, addr, addrlen);
}

TEST_F(TestSocket, TestCloseSocket)
{
    int sockfd = 1;
    EXPECT_CALL(*mockSocket, closeSocket(sockfd)).Times(1);
    mockSocket->closeSocket(sockfd);
}

TEST_F(TestSocket, TestConnectSocket)
{
    int sockfd = 1;
    const struct sockaddr *addr = nullptr;
    socklen_t addrlen = 0;
    EXPECT_CALL(*mockSocket, connectSocket(sockfd, addr, addrlen)).Times(1);
    mockSocket->connectSocket(sockfd, addr, addrlen);
}

TEST_F(TestSocket, TestCreateSocket)
{
    int domain = 1;
    int type = 1;
    int protocol = 1;
    EXPECT_CALL(*mockSocket, createSocket(domain, type, protocol)).Times(1);
    mockSocket->createSocket(domain, type, protocol);
}

TEST_F(TestSocket, TestListenSocket)
{
    int sockfd = 1;
    int backlog = 1;
    EXPECT_CALL(*mockSocket, listenSocket(sockfd, backlog)).Times(1);
    mockSocket->listenSocket(sockfd, backlog);
}

TEST_F(TestSocket, TestSendSocket)
{
    int sockfd = 1;
    const void *buf = nullptr;
    size_t len = 0;
    int flags = 0;
    EXPECT_CALL(*mockSocket, sendSocket(sockfd, buf, len, flags)).Times(1);
    mockSocket->sendSocket(sockfd, buf, len, flags);
}

TEST_F(TestSocket, TestSetSocketOptions)
{
    int sockfd = 1;
    int level = 1;
    int optname = 1;
    const void *optval = nullptr;
    socklen_t optlen = 0;
    EXPECT_CALL(*mockSocket, setSocketOptions(sockfd, level, optname, optval, optlen)).Times(1);
    mockSocket->setSocketOptions(sockfd, level, optname, optval, optlen);
}

TEST_F(TestSocket, TestReadSocket)
{
    int sockfd = 1;
    void *buf = nullptr;
    size_t count = 0;
    EXPECT_CALL(*mockSocket, readSocket(sockfd, buf, count)).Times(1);
    mockSocket->readSocket(sockfd, buf, count);
}
