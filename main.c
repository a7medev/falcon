#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "net.h"

int main(void) {
    Listener listener;

    const int PORT = 4221;
    int result = Listen(&listener, PORT);
    if (result != 0) {
        fprintf(stderr, "Failed to listen on port %d\n", PORT);
        exit(1);
    }

    printf("Listening on port %d\n", PORT);

    while (true) {
        Connection connection;
        if (Accept(&listener, &connection) != 0) {
            fprintf(stderr, "Failed to accept connection\n");
            continue;
        }

        printf("Accepted connection %s:%d\n", connection.address, connection.port);

        const char buf[] = "HTTP/1.1 200 OK\r\n\r\n";
        Write(&connection, buf, sizeof(buf) - 1);
        Close(&connection);
    }

    return 0;
}