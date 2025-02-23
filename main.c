#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "server.h"

static void OnRequestHandler(FLNRequestContext *context) {
    printf("Connection from %s:%d\n", context->connection.address, context->connection.port);

    if (strcmp(context->request.url, "/user-agent") == 0) {
        if (strcmp(context->request.method, "GET") != 0) {
            FLNResponseSetStatus(&context->response, FLN_HTTP_METHOD_NOT_ALLOWED);
            FLNRequestContextEnd(context);
            FLNRequestContextFree(context);
            return;
        }

        FLNResponseSetStatus(&context->response, FLN_HTTP_OK);
        FLNResponseSetHeader(&context->response, "Content-Type", "application/json");
        FLNResponseSetBody(&context->response, FLNHeaderMapGet(&context->request.headers, "User-Agent"));
        FLNRequestContextEnd(context);
    } else {
        FLNResponseSetStatus(&context->response, FLN_HTTP_NOT_FOUND);
        FLNRequestContextEnd(context);
    }

    FLNRequestContextFree(context);
}

int main(void) {
    const int PORT = 4221;
    FLNServer server;
    int result = FLNServerCreate(&server, PORT);
    if (result != 0) {
        fprintf(stderr, "Failed to listen on port %d. Error: %s\n", PORT, strerror(errno));
        exit(1);
    }
    printf("Listening on port %d\n", PORT);

    FLNServerHandleRequests(&server, OnRequestHandler);

    return 0;
}
