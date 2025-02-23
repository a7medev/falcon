//
// Created by Ahmed Elrefaey on 12/02/2025.
//

#ifndef HTTP_H
#define HTTP_H

#include <pthread.h>

#include "bufio.h"
#include "net.h"
#include "messages.h"

struct FLNRequestContext;

typedef void (*FLNRequestHandler)(struct FLNRequestContext *context);

typedef struct FLNRequestContext {
    FLNConnection connection;
    pthread_t thread;
    FLNRequestHandler handler;
    FLNRequest request;
    FLNResponse response;
    FLNReader reader;
} FLNRequestContext;

typedef struct server {
    FLNListener listener;
    FLNRequestHandler handler;
} FLNServer;

#pragma mark - Setup
int FLNServerCreate(FLNServer *server, int port);
void FLNServerHandleRequests(const FLNServer *server, FLNRequestHandler handler);

#pragma mark - Request Context
void FLNRequestContextFree(FLNRequestContext *context);
int FLNRequestContextEnd(const FLNRequestContext *context);

#endif //HTTP_H
