#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <unordered_set>
#include <mutex>

class Socket
{
public:
    Socket() = default;
    virtual ~Socket() = default;

    virtual int acceptSocket(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
    virtual int bindSocket(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    virtual int closeSocket(int sockfd);
    virtual int connectSocket(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    virtual int createSocket(int domain, int type, int protocol);
    virtual int listenSocket(int sockfd, int backlog);
    virtual int sendSocket(int sockfd, const void *buf, size_t len, int flags);
    virtual int setSocketOptions(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
    virtual ssize_t readSocket(int sockfd, void *buf, size_t count);
};

#endif // SOCKET_H