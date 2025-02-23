//
// Created by Ahmed Elrefaey on 12/02/2025.
//

#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdarg.h>

#include "net.h"

int Listen(Listener *listener, int port) {
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    const int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        return -1;
    }

    listener->socket_fd = socket_fd;

    // Allow reusing the address. This avoids the "Address already in use" error.
    const int option = 1;
    int result = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if (result != 0) {
        return -1;
    }

    result = bind(socket_fd, (struct sockaddr *) &address, sizeof(address));
    if (result != 0) {
        return -1;
    }

    result = listen(socket_fd, 128);
    if (result != 0) {
        return -1;
    }

    return 0;
}

int Accept(const Listener *listener, Connection *connection) {
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    const int conn_fd = accept(listener->socket_fd, (struct sockaddr *) &client_addr, &client_addr_len);
    if (conn_fd < 0) {
        return -1;
    }

    connection->conn_fd = conn_fd;
    connection->port = client_addr.sin_port;

    const char *address = inet_ntoa(client_addr.sin_addr);
    connection->address = strdup(address);

    return 0;
}

int Close(const Connection *connection) {
    const int result = shutdown(connection->conn_fd, SHUT_WR);
    if (result != 0) {
        return -1;
    }
    free(connection->address);
    return 0;
}

ssize_t Read(const Connection *connection, void *buffer, size_t size) {
    return read(connection->conn_fd, buffer, size);
}

ssize_t Write(const Connection *connection, const void *buffer, const size_t size) {
    return write(connection->conn_fd, buffer, size);
}

int WriteFormat(const Connection *connection, const char *format, ...) {
    va_list args;
    va_start(args, format);
    const int n = vdprintf(connection->conn_fd, format, args);
    va_end(args);
    return n;
}

