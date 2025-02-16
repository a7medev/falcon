//
// Created by Ahmed Elrefaey on 13/02/2025.
//

#ifndef REQUEST_H
#define REQUEST_H

typedef struct Request {
    char *method;
    char *url;
    char *version;
    // FIXME: Make a Headers container
    // Header headers[10];
    // char *body;
} Request;

#endif //REQUEST_H
