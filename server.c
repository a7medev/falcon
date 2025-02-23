//
// Created by Ahmed Elrefaey on 12/02/2025.
//

#include <stdlib.h>
#include <stdbool.h>
#include "server.h"

#include "parser.h"

int FLNServerCreate(FLNServer *server, const int port) {
    return FLNListen(&server->listener, port);
}

static void *HandleConnection(void *extContext) {
    FLNRequestContext *context = extContext;

    if (FLNParseRequest(&context->reader, &context->request) != 0) {
        fprintf(stderr, "Failed to parse request\n");
        return NULL;
    }

    context->handler(context);

    // TODO: Handle Connection header (keep-alive and close).
    // TODO: Handle compression

    return NULL;
}

static FLNRequestContext *FLNRequestContextAllocate() {
    FLNRequestContext *context = malloc(sizeof(FLNRequestContext));

    FLNRequestCreate(&context->request);
    FLNResponseCreate(&context->response);

    return context;
}

void FLNRequestContextFree(FLNRequestContext *context) {
    FLNRequestFree(&context->request);
    FLNResponseFree(&context->response);
    FLNClose(&context->connection);
    free(context);
}

void FLNServerHandleRequests(const FLNServer *server, const FLNRequestHandler handler) {
    while (true) {
        FLNRequestContext *context = FLNRequestContextAllocate();

        if (FLNAccept(&server->listener, &context->connection) != 0) {
            fprintf(stderr, "Failed to accept connection\n");
            continue;
        }

        FLNReaderCreate(&context->reader, context->connection.conn_fd);

        context->handler = handler;
        pthread_create(&context->thread, NULL, HandleConnection, context);
    }
}

static const char* StatusToReasonPhrase(const FLNHttpStatusCode status) {
    switch (status) {
    case FLN_HTTP_CONTINUE:
        return "Continue";
    case FLN_HTTP_SWITCHING_PROTOCOLS:
        return "Switching Protocols";
    case FLN_HTTP_PROCESSING:
        return "Processing";
    case FLN_HTTP_EARLY_HINTS:
        return "Early Hints";
    case FLN_HTTP_OK:
        return "OK";
    case FLN_HTTP_CREATED:
        return "Created";
    case FLN_HTTP_ACCEPTED:
        return "Accepted";
    case FLN_HTTP_NON_AUTHORITATIVE_INFORMATION:
        return "Non-Authoritative Information";
    case FLN_HTTP_NO_CONTENT:
        return "No Content";
    case FLN_HTTP_RESET_CONTENT:
        return "Reset Content";
    case FLN_HTTP_PARTIAL_CONTENT:
        return "Partial Content";
    case FLN_HTTP_MULTI_STATUS:
        return "Multi-Status";
    case FLN_HTTP_ALREADY_REPORTED:
        return "Already Reported";
    case FLN_HTTP_IM_USED:
        return "IM Used";
    case FLN_HTTP_MULTIPLE_CHOICES:
        return "Multiple Choices";
    case FLN_HTTP_MOVED_PERMANENTLY:
        return "Moved Permanently";
    case FLN_HTTP_FOUND:
        return "Found";
    case FLN_HTTP_SEE_OTHER:
        return "See Other";
    case FLN_HTTP_NOT_MODIFIED:
        return "Not Modified";
    case FLN_HTTP_USE_PROXY:
        return "Use Proxy";
    case FLN_HTTP_TEMPORARY_REDIRECT:
        return "Temporary Redirect";
    case FLN_HTTP_PERMANENT_REDIRECT:
        return "Permanent Redirect";
    case FLN_HTTP_BAD_REQUEST:
        return "Bad Request";
    case FLN_HTTP_UNAUTHORIZED:
        return "Unauthorized";
    case FLN_HTTP_PAYMENT_REQUIRED:
        return "Payment Required";
    case FLN_HTTP_FORBIDDEN:
        return "Forbidden";
    case FLN_HTTP_NOT_FOUND:
        return "Not Found";
    case FLN_HTTP_METHOD_NOT_ALLOWED:
        return "Method Not Allowed";
    case FLN_HTTP_NOT_ACCEPTABLE:
        return "Not Acceptable";
    case FLN_HTTP_PROXY_AUTHENTICATION_REQUIRED:
        return "Proxy Authentication Required";
    case FLN_HTTP_REQUEST_TIMEOUT:
        return "Request Timeout";
    case FLN_HTTP_CONFLICT:
        return "Conflict";
    case FLN_HTTP_GONE:
        return "Gone";
    case FLN_HTTP_LENGTH_REQUIRED:
        return "Length Required";
    case FLN_HTTP_PRECONDITION_FAILED:
        return "Precondition Failed";
    case FLN_HTTP_CONTENT_TOO_LARGE:
        return "Content Too Large";
    case FLN_HTTP_URI_TOO_LONG:
        return "URI Too Long";
    case FLN_HTTP_UNSUPPORTED_MEDIA_TYPE:
        return "Unsupported Media Type";
    case FLN_HTTP_RANGE_NOT_SATISFIABLE:
        return "Range Not Satisfiable";
    case FLN_HTTP_EXPECTATION_FAILED:
        return "Expectation Failed";
    case FLN_HTTP_IM_A_TEAPOT:
        return "I'm a teapot";
    case FLN_HTTP_MISDIRECTED_REQUEST:
        return "Misdirected Request";
    case FLN_HTTP_UNPROCESSABLE_CONTENT:
        return "Unprocessable Content";
    case FLN_HTTP_LOCKED:
        return "Locked";
    case FLN_HTTP_FAILED_DEPENDENCY:
        return "Failed Dependency";
    case FLN_HTTP_TOO_EARLY:
        return "Too Early";
    case FLN_HTTP_UPGRADE_REQUIRED:
        return "Upgrade Required";
    case FLN_HTTP_PRECONDITION_REQUIRED:
        return "Precondition Required";
    case FLN_HTTP_TOO_MANY_REQUESTS:
        return "Too Many Requests";
    case FLN_HTTP_REQUEST_HEADER_FIELDS_TOO_LARGE:
        return "Request Header Fields Too Large";
    case FLN_HTTP_UNAVAILABLE_FOR_LEGAL_REASONS:
        return "Unavailable For Legal Reasons";
    case FLN_HTTP_INTERNAL_SERVER_ERROR:
        return "Internal Server Error";
    case FLN_HTTP_NOT_IMPLEMENTED:
        return "Not Implemented";
    case FLN_HTTP_BAD_GATEWAY:
        return "Bad Gateway";
    case FLN_HTTP_SERVICE_UNAVAILABLE:
        return "Service Unavailable";
    case FLN_HTTP_GATEWAY_TIMEOUT:
        return "Gateway Timeout";
    case FLN_HTTP_HTTP_VERSION_NOT_SUPPORTED:
        return "HTTP Version Not Supported";
    case FLN_HTTP_VARIANT_ALSO_NEGOTIATES:
        return "Variant Also Negotiates";
    case FLN_HTTP_INSUFFICIENT_STORAGE:
        return "Insufficient Storage";
    case FLN_HTTP_LOOP_DETECTED:
        return "Loop Detected";
    case FLN_HTTP_NOT_EXTENDED:
        return "Not Extended";
    case FLN_HTTP_NETWORK_AUTHENTICATION_REQUIRED:
        return "Network Authentication Required";
    default:
        return NULL;
    }
}

int FLNRequestContextEnd(const FLNRequestContext *context) {
    const FLNResponse *response = &context->response;

    ssize_t result = FLNWriteFormat(&context->connection, "HTTP/1.1 %d %s\r\n", response->status, StatusToReasonPhrase(response->status));
    if (result < 0) {
        return -1;
    }

    const int headersCount = FLNHeaderMapCount(&context->response.headers);
    for (int i = 0; i < headersCount; i++) {
        const FLNHeader header = FLNHeaderMapGetAt(&context->response.headers, i);

        result = FLNWriteFormat(&context->connection, "%s: %s\r\n", header.header, header.value);
        if (result < 0) {
            return -1;
        }
    }

    result = FLNWrite(&context->connection, "\r\n", 2);
    if (result < 0) {
        return -1;
    }

    if (response->body == NULL) {
        return 0;
    }

    result = FLNWrite(&context->connection, response->body, strlen(response->body));
    if (result < 0) {
        return -1;
    }

    return 0;
}
