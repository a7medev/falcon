//
// Created by Ahmed Elrefaey on 12/02/2025.
//

#ifndef NET_H
#define NET_H

#include <stdio.h>

typedef struct Listener {
    int socket_fd;
} Listener;

typedef struct Connection {
    int conn_fd;
    int port;
    char* address;
} Connection;

int Listen(Listener *listener,int port);
int Accept(const Listener *listener, Connection *connection);
int Close(const Connection *connection);

size_t Read(const Connection *connection, void *buffer, size_t size);
size_t Write(const Connection *connection, const void *buffer, size_t size);

#endif //NET_H
