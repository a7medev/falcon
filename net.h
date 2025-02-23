//
// Created by Ahmed Elrefaey on 12/02/2025.
//

#ifndef NET_H
#define NET_H

#include <stdio.h>

typedef struct FLNListener {
    int socket_fd;
} FLNListener;

typedef struct FLNConnection {
    int conn_fd;
    int port;
    char* address;
} FLNConnection;

int FLNListen(FLNListener *listener, int port);
int FLNAccept(const FLNListener *listener, FLNConnection *connection);
int FLNClose(const FLNConnection *connection);

ssize_t FLNRead(const FLNConnection *connection, void *buffer, size_t size);
ssize_t FLNWrite(const FLNConnection *connection, const void *buffer, size_t size);
int FLNWriteFormat(const FLNConnection *connection, const char *format, ...);

#endif //NET_H
