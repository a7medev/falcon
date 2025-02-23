//
// Created by Ahmed Elrefaey on 12/02/2025.
//

#include "parser.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bufio.h"
#include "server.h"

#pragma mark - Read utilities

static void DiscardAll(Reader *reader, const char ch) {
    while (ReaderPeekByte(reader) == ch) {
        ReaderReadByte(reader);
    }
}

// TODO: Refactor?? Is it too much??
static bool SkipCLRF(Reader *reader) {
    if (ReaderPeekByte(reader) != '\r') {
        return false;
    }

    ReaderReadByte(reader);
    if (ReaderPeekByte(reader) != '\n') {
        ReaderBackByte(reader);
        return false;
    }

    ReaderReadByte(reader);

    return true;
}

#pragma mark - Parser

int Parse(Reader *reader, Request *request) {
    // Method
    // TODO: Validate method and turn it into an enum.
    if (ReaderReadUntil(reader, ' ', &request->method) != 0) {
        return -1;
    }

    DiscardAll(reader, ' ');

    // URL
    if (ReaderReadUntil(reader, ' ', &request->url) != 0) {
        return -1;
    }

    DiscardAll(reader, ' ');

    // Version & CR
    if (ReaderReadUntil(reader, '\r', &request->version) != 0) {
        return -1;
    }

    // LF
    if (ReaderReadByte(reader) != '\n') {
        return -1;
    }

    // Headers
    while (!SkipCLRF(reader)) {
        char *header;
        char *value;

        if (ReaderReadUntil(reader, ':', &header) != 0) {
            return -1;
        }

        DiscardAll(reader, ' ');

        if (ReaderReadUntil(reader, '\r', &value) != 0) {
            return -1;
        }

        // LF
        if (ReaderReadByte(reader) != '\n') {
            return -1;
        }

        HeaderMapSet(&request->headers, header, value);

        free(header);
        free(value);
    }

    // TODO: Handle body
    // TODO: with Transfer-Encoding: chunked
    // TODO: without, read all remaining

    const char *contentLengthValue = HeaderMapGet(&request->headers, "Content-Length");
    if (contentLengthValue != NULL) {
        const long contentLength = strtol(contentLengthValue, NULL, 10);
        request->body = malloc((contentLength + 1) * sizeof(char));
        if (ReaderRead(reader, request->body, contentLength) < 0) {
            return -1;
        }
    }

    return 0;
}
