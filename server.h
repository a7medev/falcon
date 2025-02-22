//
// Created by Ahmed Elrefaey on 12/02/2025.
//

#ifndef HTTP_H
#define HTTP_H

#include <pthread.h>

#include "net.h"
#include "messages.h"

struct RequestContext;

typedef void (*RequestHandler)(struct RequestContext *context);

typedef struct RequestContext {
    Connection connection;
    pthread_t thread;
    RequestHandler handler;
    Request request;
} RequestContext;

typedef struct Server {
    Listener listener;
    RequestHandler handler;
} Server;

#pragma mark - Setup
int CreateServer(Server *server, int port);
void HandleRequests(const Server *server, RequestHandler handler);

#pragma mark - Request Context
void RequestContextFree(RequestContext *context);

#pragma mark - Response
void AddHeader(RequestContext *context, char *header, char *value);
void SetBody(RequestContext *context, char *body);
void SetStatus(RequestContext *context, int status);
int EndRequest(RequestContext *context);

#endif //HTTP_H
