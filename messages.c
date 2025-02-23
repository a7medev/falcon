//
// Created by Ahmed Elrefaey on 22/02/2025.
//

#include <stdio.h>

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
    response->status = HTTP_OK;
    response->body = NULL;
    HeaderMapCreate(&response->headers);
}

void ResponseFree(Response *response) {
    free(response->body);
    HeaderMapFree(&response->headers);
}

void ResponseSetStatus(Response *response, HttpStatusCode status) {
    response->status = status;
}

void ResponseSetHeader(Response *response, const char *header, const char *value) {
    HeaderMapSet(&response->headers, header, value);
}

void ResponseSetBody(Response *response, const char *body) {
    free(response->body);

    response->body = strdup(body);

    char length[10];
    snprintf(length, sizeof(length), "%lu", strlen(body));

    ResponseSetHeader(response, "Content-Length", length);
}
