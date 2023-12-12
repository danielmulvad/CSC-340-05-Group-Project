#include "./Socket.h"
#include <netinet/in.h>
#include <stdexcept>
#include <unistd.h>

/**
 * Accepts a connection on a socket.
 *
 * @param sockfd Socket file descriptor.
 * @param addr Pointer to a sockaddr structure that receives the client's address.
 * @param addrlen Pointer to a socklen_t variable that on input specifies the size of the structure pointed to by addr.
 * @return New socket file descriptor on success.
 * @throws std::runtime_error If accept fails.
 */
int Socket::acceptSocket(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    int status = accept(sockfd, addr, addrlen);
    if (status < 0)
    {
        throw std::runtime_error("accept");
    }
    return status;
}

/**
 * Binds a socket to an address.
 *
 * @param sockfd Socket file descriptor.
 * @param addr Pointer to a sockaddr structure containing the address to bind to.
 * @param addrlen Size of the sockaddr structure.
 * @return Zero on success.
 * @throws std::runtime_error If bind fails.
 */
int Socket::bindSocket(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    int status = bind(sockfd, addr, addrlen);
    if (status < 0)
    {
        throw std::runtime_error("bind failed");
    }
    return status;
}

/**
 * Closes a socket.
 *
 * @param sockfd Socket file descriptor.
 * @return Zero on success.
 * @throws std::runtime_error If close fails.
 */
int Socket::closeSocket(int sockfd)
{
    int status = close(sockfd);
    if (status < 0)
    {
        throw std::runtime_error("close");
    }
    return status;
}

/**
 * Establishes a connection to a specified socket.
 *
 * @param sockfd Socket file descriptor.
 * @param addr Pointer to a sockaddr structure containing the destination address.
 * @param addrlen Size of the sockaddr structure.
 * @return Zero on success.
 * @throws std::runtime_error If connect fails.
 */
int Socket::connectSocket(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    int status = connect(sockfd, addr, addrlen);
    if (status < 0)
    {
        throw std::runtime_error("connect");
    }
    return status;
}

/**
 * Creates a new socket.
 *
 * @param domain Specifies the communication domain.
 * @param type Specifies the communication semantics.
 * @param protocol Specifies a particular protocol to be used with the socket.
 * @return Socket file descriptor on success.
 * @throws std::runtime_error If socket creation fails.
 */
int Socket::createSocket(int domain, int type, int protocol)
{
    int status = socket(domain, type, protocol);
    if (status < 0)
    {
        throw std::runtime_error("Socket creation failed");
    }
    return status;
}

/**
 * Listens for connections on a socket.
 *
 * @param sockfd Socket file descriptor.
 * @param backlog Maximum length for the queue of pending connections.
 * @return Zero on success.
 * @throws std::runtime_error If listen fails.
 */
int Socket::listenSocket(int sockfd, int backlog)
{
    int status = listen(sockfd, backlog);
    if (status < 0)
    {
        throw std::runtime_error("listen");
    }
    return status;
}

/**
 * Sends data on a connected socket.
 *
 * @param sockfd Socket file descriptor.
 * @param buf Pointer to the buffer containing the message to send.
 * @param len Length of the message in bytes.
 * @param flags Flags for the send operation.
 * @return Number of bytes sent on success.
 * @throws std::runtime_error If send fails.
 */
int Socket::sendSocket(int sockfd, const void *buf, size_t len, int flags)
{
    int status = send(sockfd, buf, len, flags);
    if (status < 0)
    {
        throw std::runtime_error("send");
    }
    return status;
}

/**
 * Sets options on a socket.
 *
 * @param sockfd Socket file descriptor.
 * @param level The level at which the option is defined.
 * @param optname The name of the option.
 * @param optval Pointer to the option value.
 * @param optlen Size of the option value.
 * @return Zero on success.
 * @throws std::runtime_error If setsockopt fails.
 */
int Socket::setSocketOptions(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
{
    int status = setsockopt(sockfd, level, optname, optval, optlen);
    if (status < 0)
    {
        throw std::runtime_error("setsockopt");
    }
    return status;
}

/**
 * Reads data from a socket.
 *
 * @param sockfd Socket file descriptor.
 * @param buf Buffer to store the read data.
 * @param count Maximum number of bytes to read.
 * @return Number of bytes read on success.
 * @throws std::runtime_error If read fails.
 */
ssize_t Socket::readSocket(int sockfd, void *buf, size_t count)
{
    ssize_t status = read(sockfd, buf, count);
    if (status < 0)
    {
        throw std::runtime_error("read");
    }
    return status;
}
