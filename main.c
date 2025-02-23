#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "server.h"

static void OnRequestHandler(RequestContext *context) {
    printf("Connection from %s:%d\n", context->connection.address, context->connection.port);

    if (strcmp(context->request.url, "/user-agent") == 0) {
        if (strcmp(context->request.method, "GET") != 0) {
            ResponseSetStatus(&context->response, HTTP_METHOD_NOT_ALLOWED);
            RequestContextEnd(context);
            RequestContextFree(context);
            return;
        }

        ResponseSetStatus(&context->response, HTTP_OK);
        ResponseSetHeader(&context->response, "Content-Type", "application/json");
        ResponseSetBody(&context->response, HeaderMapGet(&context->request.headers, "User-Agent"));
        RequestContextEnd(context);
    } else {
        ResponseSetStatus(&context->response, HTTP_NOT_FOUND);
        RequestContextEnd(context);
    }

    RequestContextFree(context);
}

int main(void) {
    const int PORT = 4221;
    Server server;
    int result = CreateServer(&server, PORT);
    if (result != 0) {
        fprintf(stderr, "Failed to listen on port %d. Error: %s\n", PORT, strerror(errno));
        exit(1);
    }
    printf("Listening on port %d\n", PORT);

    HandleRequests(&server, OnRequestHandler);

    return 0;
}
