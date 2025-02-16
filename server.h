//
// Created by Ahmed Elrefaey on 12/02/2025.
//

#ifndef HTTP_H
#define HTTP_H

#include <pthread.h>

#include "net.h"
#include "request.h"

struct RequestContext;

typedef void (*RequestHandler)(struct RequestContext *context);

typedef struct RequestContext {
    Connection connection;
    pthread_t thread;
    RequestHandler handler;
    Request request;
    // TODO: Add method, URL, headers, body, query parameters, etc...
} RequestContext;

typedef struct Server {
    Listener listener;
    RequestHandler handler;
} Server;

typedef enum HttpStatusCode {
    HTTP_CONTINUE = 100,
    HTTP_SWITCHING_PROTOCOLS = 101,
    HTTP_PROCESSING = 102,
    HTTP_EARLY_HINTS = 103,
    HTTP_OK = 200,
    HTTP_CREATED = 201,
    HTTP_ACCEPTED = 202,
    HTTP_NON_AUTHORITATIVE_INFORMATION = 203,
    HTTP_NO_CONTENT = 204,
    HTTP_RESET_CONTENT = 205,
    HTTP_PARTIAL_CONTENT = 206,
    HTTP_MULTI_STATUS = 207,
    HTTP_ALREADY_REPORTED = 208,
    HTTP_IM_USED = 226,
    HTTP_MULTIPLE_CHOICES = 300,
    HTTP_MOVED_PERMANENTLY = 301,
    HTTP_FOUND = 302,
    HTTP_SEE_OTHER = 303,
    HTTP_NOT_MODIFIED = 304,
    HTTP_USE_PROXY = 305,
    HTTP_TEMPORARY_REDIRECT = 307,
    HTTP_PERMANENT_REDIRECT = 308,
    HTTP_BAD_REQUEST = 400,
    HTTP_UNAUTHORIZED = 401,
    HTTP_PAYMENT_REQUIRED = 402,
    HTTP_FORBIDDEN = 403,
    HTTP_NOT_FOUND = 404,
    HTTP_METHOD_NOT_ALLOWED = 405,
    HTTP_NOT_ACCEPTABLE = 406,
    HTTP_PROXY_AUTHENTICATION_REQUIRED = 407,
    HTTP_REQUEST_TIMEOUT = 408,
    HTTP_CONFLICT = 409,
    HTTP_GONE = 410,
    HTTP_LENGTH_REQUIRED = 411,
    HTTP_PRECONDITION_FAILED = 412,
    HTTP_CONTENT_TOO_LARGE = 413,
    HTTP_URI_TOO_LONG = 414,
    HTTP_UNSUPPORTED_MEDIA_TYPE = 415,
    HTTP_RANGE_NOT_SATISFIABLE = 416,
    HTTP_EXPECTATION_FAILED = 417,
    HTTP_IM_A_TEAPOT = 418,
    HTTP_MISDIRECTED_REQUEST = 421,
    HTTP_UNPROCESSABLE_CONTENT = 422,
    HTTP_LOCKED = 423,
    HTTP_FAILED_DEPENDENCY = 424,
    HTTP_TOO_EARLY = 425,
    HTTP_UPGRADE_REQUIRED = 426,
    HTTP_PRECONDITION_REQUIRED = 428,
    HTTP_TOO_MANY_REQUESTS = 429,
    HTTP_REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
    HTTP_UNAVAILABLE_FOR_LEGAL_REASONS = 451,
    HTTP_INTERNAL_SERVER_ERROR = 500,
    HTTP_NOT_IMPLEMENTED = 501,
    HTTP_BAD_GATEWAY = 502,
    HTTP_SERVICE_UNAVAILABLE = 503,
    HTTP_GATEWAY_TIMEOUT = 504,
    HTTP_HTTP_VERSION_NOT_SUPPORTED = 505,
    HTTP_VARIANT_ALSO_NEGOTIATES = 506,
    HTTP_INSUFFICIENT_STORAGE = 507,
    HTTP_LOOP_DETECTED = 508,
    HTTP_NOT_EXTENDED = 510,
    HTTP_NETWORK_AUTHENTICATION_REQUIRED = 511
} HttpStatusCode;

static const char* HttpStatusCodeToReasonPhrase(const HttpStatusCode status) {
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
        return "OK";
    }
}

#pragma mark - Setup
int CreateServer(Server *server, int port);
void HandleRequests(const Server *server, RequestHandler handler);

#pragma mark - Response
void AddHeader(RequestContext *context, char *header, char *value);
void SetBody(RequestContext *context, char *body);
void SetStatus(RequestContext *context, int status);
int EndRequest(RequestContext *context);

#endif //HTTP_H
