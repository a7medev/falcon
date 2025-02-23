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

static void *HandleConnection(void *extContext) {
    RequestContext *context = extContext;

    if (Parse(&context->reader, &context->request) != 0) {
        fprintf(stderr, "Failed to parse request\n");
        return NULL;
    }

    context->handler(context);

    return NULL;
}

static RequestContext *RequestContextAllocate() {
    RequestContext *context = malloc(sizeof(RequestContext));

    RequestCreate(&context->request);
    ResponseCreate(&context->response);

    return context;
}

void RequestContextFree(RequestContext *context) {
    RequestFree(&context->request);
    ResponseFree(&context->response);
    Close(&context->connection);
    free(context);
}

void HandleRequests(const Server *server, const RequestHandler handler) {
    while (true) {
        RequestContext *context = RequestContextAllocate();

        if (Accept(&server->listener, &context->connection) != 0) {
            fprintf(stderr, "Failed to accept connection\n");
            continue;
        }

        ReaderCreate(&context->reader, context->connection.conn_fd);

        context->handler = handler;
        pthread_create(&context->thread, NULL, HandleConnection, context);
    }
}

static const char* StatusToReasonPhrase(const HttpStatusCode status) {
    switch (status) {
    case HTTP_CONTINUE:
        return "Continue";
    case HTTP_SWITCHING_PROTOCOLS:
        return "Switching Protocols";
    case HTTP_PROCESSING:
        return "Processing";
    case HTTP_EARLY_HINTS:
        return "Early Hints";
    case HTTP_OK:
        return "OK";
    case HTTP_CREATED:
        return "Created";
    case HTTP_ACCEPTED:
        return "Accepted";
    case HTTP_NON_AUTHORITATIVE_INFORMATION:
        return "Non-Authoritative Information";
    case HTTP_NO_CONTENT:
        return "No Content";
    case HTTP_RESET_CONTENT:
        return "Reset Content";
    case HTTP_PARTIAL_CONTENT:
        return "Partial Content";
    case HTTP_MULTI_STATUS:
        return "Multi-Status";
    case HTTP_ALREADY_REPORTED:
        return "Already Reported";
    case HTTP_IM_USED:
        return "IM Used";
    case HTTP_MULTIPLE_CHOICES:
        return "Multiple Choices";
    case HTTP_MOVED_PERMANENTLY:
        return "Moved Permanently";
    case HTTP_FOUND:
        return "Found";
    case HTTP_SEE_OTHER:
        return "See Other";
    case HTTP_NOT_MODIFIED:
        return "Not Modified";
    case HTTP_USE_PROXY:
        return "Use Proxy";
    case HTTP_TEMPORARY_REDIRECT:
        return "Temporary Redirect";
    case HTTP_PERMANENT_REDIRECT:
        return "Permanent Redirect";
    case HTTP_BAD_REQUEST:
        return "Bad Request";
    case HTTP_UNAUTHORIZED:
        return "Unauthorized";
    case HTTP_PAYMENT_REQUIRED:
        return "Payment Required";
    case HTTP_FORBIDDEN:
        return "Forbidden";
    case HTTP_NOT_FOUND:
        return "Not Found";
    case HTTP_METHOD_NOT_ALLOWED:
        return "Method Not Allowed";
    case HTTP_NOT_ACCEPTABLE:
        return "Not Acceptable";
    case HTTP_PROXY_AUTHENTICATION_REQUIRED:
        return "Proxy Authentication Required";
    case HTTP_REQUEST_TIMEOUT:
        return "Request Timeout";
    case HTTP_CONFLICT:
        return "Conflict";
    case HTTP_GONE:
        return "Gone";
    case HTTP_LENGTH_REQUIRED:
        return "Length Required";
    case HTTP_PRECONDITION_FAILED:
        return "Precondition Failed";
    case HTTP_CONTENT_TOO_LARGE:
        return "Content Too Large";
    case HTTP_URI_TOO_LONG:
        return "URI Too Long";
    case HTTP_UNSUPPORTED_MEDIA_TYPE:
        return "Unsupported Media Type";
    case HTTP_RANGE_NOT_SATISFIABLE:
        return "Range Not Satisfiable";
    case HTTP_EXPECTATION_FAILED:
        return "Expectation Failed";
    case HTTP_IM_A_TEAPOT:
        return "I'm a teapot";
    case HTTP_MISDIRECTED_REQUEST:
        return "Misdirected Request";
    case HTTP_UNPROCESSABLE_CONTENT:
        return "Unprocessable Content";
    case HTTP_LOCKED:
        return "Locked";
    case HTTP_FAILED_DEPENDENCY:
        return "Failed Dependency";
    case HTTP_TOO_EARLY:
        return "Too Early";
    case HTTP_UPGRADE_REQUIRED:
        return "Upgrade Required";
    case HTTP_PRECONDITION_REQUIRED:
        return "Precondition Required";
    case HTTP_TOO_MANY_REQUESTS:
        return "Too Many Requests";
    case HTTP_REQUEST_HEADER_FIELDS_TOO_LARGE:
        return "Request Header Fields Too Large";
    case HTTP_UNAVAILABLE_FOR_LEGAL_REASONS:
        return "Unavailable For Legal Reasons";
    case HTTP_INTERNAL_SERVER_ERROR:
        return "Internal Server Error";
    case HTTP_NOT_IMPLEMENTED:
        return "Not Implemented";
    case HTTP_BAD_GATEWAY:
        return "Bad Gateway";
    case HTTP_SERVICE_UNAVAILABLE:
        return "Service Unavailable";
    case HTTP_GATEWAY_TIMEOUT:
        return "Gateway Timeout";
    case HTTP_HTTP_VERSION_NOT_SUPPORTED:
        return "HTTP Version Not Supported";
    case HTTP_VARIANT_ALSO_NEGOTIATES:
        return "Variant Also Negotiates";
    case HTTP_INSUFFICIENT_STORAGE:
        return "Insufficient Storage";
    case HTTP_LOOP_DETECTED:
        return "Loop Detected";
    case HTTP_NOT_EXTENDED:
        return "Not Extended";
    case HTTP_NETWORK_AUTHENTICATION_REQUIRED:
        return "Network Authentication Required";
    default:
        return NULL;
    }
}

void SetStatus(RequestContext *context, HttpStatusCode status) {
    context->response.status = status;
}

void AddHeader(RequestContext *context, const char *header, const char *value) {
    HeaderMapSet(&context->response.headers, header, value);
}

void SetBody(RequestContext *context, const char *body) {
    free(context->response.body);
    context->response.body = strdup(body);
    char length[10];
    snprintf(length, sizeof(length), "%lu", strlen(body));
    AddHeader(context, "Content-Length", length);
}

int EndRequest(const RequestContext *context) {
    const Response *response = &context->response;

    const int MAX_STATUS_LINE = 100;
    char statusLine[MAX_STATUS_LINE];

    int n = snprintf(statusLine, MAX_STATUS_LINE, "HTTP/1.1 %d %s\r\n", response->status, StatusToReasonPhrase(response->status));

    Write(&context->connection, statusLine, n);

    const int headersCount = HeaderMapCount(&context->response.headers);
    for (int i = 0; i < headersCount; i++) {
        const Header header = HeaderMapGetAt(&context->response.headers, i);

        // TODO: Simplify this... Maybe add write with format to bufio or a similar place
        const size_t size = strlen(header.header) + strlen(header.value) + 5;
        char *temp = malloc(size * sizeof(char));

        n = snprintf(temp, size, "%s: %s\r\n", header.header, header.value);

        Write(&context->connection, temp, n);

        free(temp);
    }

    Write(&context->connection, "\r\n", 2);

    Write(&context->connection, response->body, strlen(response->body));

    return -1;
}
