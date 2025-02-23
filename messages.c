//
// Created by Ahmed Elrefaey on 22/02/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "messages.h"

void FLNRequestCreate(FLNRequest *request) {
    request->method = NULL;
    request->version = NULL;
    request->url = NULL;
    request->body = NULL;

    FLNHeaderMapCreate(&request->headers);
}

void FLNRequestFree(FLNRequest *request) {
    FLNHeaderMapFree(&request->headers);

    free(request->method);
    free(request->version);
    free(request->url);
    free(request->body);
}

void FLNResponseCreate(FLNResponse *response) {
    response->status = FLN_HTTP_OK;
    response->body = NULL;
    FLNHeaderMapCreate(&response->headers);
}

void FLNResponseFree(FLNResponse *response) {
    free(response->body);
    FLNHeaderMapFree(&response->headers);
}

void FLNResponseSetStatus(FLNResponse *response, FLNHttpStatusCode status) {
    response->status = status;
}

void FLNResponseSetHeader(FLNResponse *response, const char *header, const char *value) {
    FLNHeaderMapSet(&response->headers, header, value);
}

void FLNResponseSetBody(FLNResponse *response, const char *body) {
    free(response->body);

    response->body = strdup(body);

    char length[10];
    snprintf(length, sizeof(length), "%lu", strlen(body));

    FLNResponseSetHeader(response, "Content-Length", length);
}
