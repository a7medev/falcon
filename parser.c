//
// Created by Ahmed Elrefaey on 12/02/2025.
//

#include "parser.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

// void print_escaped(const char *str) {
//     while (*str) {
//         if (*str == '\r') {
//             printf("\\r");
//         } else if (*str == '\n') {
//             printf("\\n\n");
//         } else {
//             putchar(*str);
//         }
//         str++;
//     }
// }

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
            free(header);
            return -1;
        }

        DiscardAll(reader, ' ');

        if (ReaderReadUntil(reader, '\r', &value) != 0) {
            free(header);
            free(value);
            return -1;
        }

        // LF
        if (ReaderReadByte(reader) != '\n') {
            free(header);
            free(value);
            return -1;
        }

        HeaderMapSet(&request->headers, header, value);

        free(header);
        free(value);
    }

    // TODO: Handle body
    // TODO: without, read all remaining

    // TODO: Move into separate method
    char *transferEncoding =  StringLower(StringTrim(HeaderMapGet(&request->headers, "Transfer-Encoding")));

    if (strcmp(transferEncoding, "chunked") == 0) {
        long chunkLength = 0;

        StringBuffer buffer;
        StringBufferCreate(&buffer);

        do {
            char *chunkLengthValue;
            if (ReaderReadUntil(reader, '\r', &chunkLengthValue) < 0) {
                free(transferEncoding);
                free(chunkLengthValue);
                return -1;
            }

            // LF
            if (ReaderReadByte(reader) != '\n') {
                free(transferEncoding);
                free(chunkLengthValue);
                return -1;
            }

            chunkLength = strtol(chunkLengthValue, NULL, 16);

            if (chunkLength == 0) {
                request->body = StringBufferToString(&buffer);
                break;
            }

            char *chunk = malloc((chunkLength + 1) * sizeof(char));
            if (ReaderRead(reader, chunk, chunkLength) < 0) {
                free(transferEncoding);
                free(chunk);
                free(chunkLengthValue);
                return -1;
            }
            chunk[chunkLength] = '\0';
            StringBufferAppendString(&buffer, chunk, (int)chunkLength);

            free(chunk);
            free(chunkLengthValue);

            if (!SkipCLRF(reader)) {
                free(transferEncoding);
                return -1;
            }
        } while (true);

        free(transferEncoding);

        if (!SkipCLRF(reader)) {
            return -1;
        }

        return 0;
    }

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
