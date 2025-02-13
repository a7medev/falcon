#include <stdio.h>
#include <stdlib.h>

#include "server.h"

static void OnRequestHandler(RequestContext *context) {
    printf("Connection from %s:%d\n", context->connection.address, context->connection.port);

    const char buf[] = "HTTP/1.1 200 OK\r\n\r\n";
    Write(&context->connection, buf, sizeof(buf) - 1);
    Close(&context->connection);

    // SetStatus(context, 200);
    // AddHeader(context, "Content-Type", "application/json");
    // SetBody(context, "{ \"status\": \"ok\" }");
    // EndRequest(context);
}

int main(void) {
    const int PORT = 4221;
    Server server;
    int result = CreateServer(&server, PORT);
    if (result != 0) {
        fprintf(stderr, "Failed to listen on port %d\n", PORT);
        exit(1);
    }
    printf("Listening on port %d\n", PORT);

    HandleRequests(&server, OnRequestHandler);

    return 0;
}
