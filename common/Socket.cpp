#include "./Socket.h"
#include <netinet/in.h>
#include <stdexcept>
#include <unistd.h>

int Socket::acceptSocket(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    int status = accept(sockfd, addr, addrlen);
    if (status < 0)
    {
        throw std::runtime_error("accept");
    }
    return status;
}

int Socket::bindSocket(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    int status = bind(sockfd, addr, addrlen);
    if (status < 0)
    {
        throw std::runtime_error("bind failed");
    }
    return status;
}

int Socket::closeSocket(int sockfd)
{
    int status = close(sockfd);
    if (status < 0)
    {
        throw std::runtime_error("close");
    }
    return status;
}

int Socket::connectSocket(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    int status = connect(sockfd, addr, addrlen);
    if (status < 0)
    {
        throw std::runtime_error("connect");
    }
    return status;
}

int Socket::createSocket(int domain, int type, int protocol)
{
    int status = socket(domain, type, protocol);
    if (status < 0)
    {
        throw std::runtime_error("Socket creation failed");
    }
    return status;
}

int Socket::listenSocket(int sockfd, int backlog)
{
    int status = listen(sockfd, backlog);
    if (status < 0)
    {
        throw std::runtime_error("listen");
    }
    return status;
}

int Socket::sendSocket(int sockfd, const void *buf, size_t len, int flags)
{
    int status = send(sockfd, buf, len, flags);
    if (status < 0)
    {
        throw std::runtime_error("send");
    }
    return status;
}

int Socket::setSocketOptions(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
{
    int status = setsockopt(sockfd, level, optname, optval, optlen);
    if (status < 0)
    {
        throw std::runtime_error("setsockopt");
    }
    return status;
}

ssize_t Socket::readSocket(int sockfd, void *buf, size_t count)
{
    ssize_t status = read(sockfd, buf, count);
    if (status < 0)
    {
        throw std::runtime_error("read");
    }
    return status;
}
