//
// Created by Ahmed Elrefaey on 22/02/2025.
//

#include "messages.h"

void RequestCreate(Request *request) {
    request->method = NULL;
    request->version = NULL;
    request->url = NULL;
    request->body = NULL;

    HeaderMapCreate(&request->headers);
}

void RequestFree(Request *request) {
    HeaderMapFree(&request->headers);

    free(request->method);
    free(request->version);
    free(request->url);
    free(request->body);
}

void ResponseCreate(Response *response) {
    response->status = 0;
    response->body = NULL;
    HeaderMapCreate(&response->headers);
}

void ResponseFree(Response *response) {
    free(response->body);
    HeaderMapFree(&response->headers);
}
