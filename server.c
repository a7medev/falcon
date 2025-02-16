//
// Created by Ahmed Elrefaey on 12/02/2025.
//

#include <stdlib.h>
#include <stdbool.h>
#include "server.h"

#include "parser.h"

int CreateServer(Server *server, const int port) {
    return Listen(&server->listener, port);
}

static void *HandleConnection(void *arg) {
    RequestContext *context = arg;

    const int MAX_REQUEST_BUFFER = 1024;
    char *data = malloc(MAX_REQUEST_BUFFER * sizeof(char));
    size_t n = Read(&context->connection, data, MAX_REQUEST_BUFFER);
    if (n == 0) {
        fprintf(stderr, "Failed to read request\n");
        return NULL;
    }

    if (Parse(data, n, &context->request) != 0) {
        fprintf(stderr, "Failed to parse request\n");
        return NULL;
    }

    context->handler(context);

    return NULL;
}

void HandleRequests(const Server *server, const RequestHandler handler) {
    while (true) {
        RequestContext *context = malloc(sizeof(RequestContext));

        if (Accept(&server->listener, &context->connection) != 0) {
            fprintf(stderr, "Failed to accept connection\n");
            continue;
        }

        context->handler = handler;
        pthread_create(&context->thread, NULL, HandleConnection, context);
    }
}

void SetStatus(RequestContext *context, int status) {
    // TODO: Not implemented
}
void AddHeader(RequestContext *context, char *header, char *value) {
    // TODO: Not implemented
}
void SetBody(RequestContext *context, char *body) {
    // TODO: Not implemented
}
int EndRequest(RequestContext *context) {
    // TODO: Not implemented
    return -1;
}
