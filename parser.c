//
// Created by Ahmed Elrefaey on 12/02/2025.
//

#include "parser.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "bufio.h"
#include "server.h"
#include "string.h"

#pragma mark - Read utilities

static void DiscardAll(FLNReader *reader, const char ch) {
    while (FLNReaderPeekByte(reader) == ch) {
        FLNReaderReadByte(reader);
    }
}

// TODO: Refactor?? Is it too much??
static bool SkipCLRF(FLNReader *reader) {
    if (FLNReaderPeekByte(reader) != '\r') {
        return false;
    }

    FLNReaderReadByte(reader);
    if (FLNReaderPeekByte(reader) != '\n') {
        FLNReaderBackByte(reader);
        return false;
    }

    FLNReaderReadByte(reader);

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

int FLNParseRequest(FLNReader *reader, FLNRequest *request) {
    // Method
    // TODO: Validate method and turn it into an enum.
    if (FLNReaderReadUntil(reader, ' ', &request->method) != 0) {
        return -1;
    }

    DiscardAll(reader, ' ');

    // URL
    if (FLNReaderReadUntil(reader, ' ', &request->url) != 0) {
        return -1;
    }

    DiscardAll(reader, ' ');

    // Version & CR
    if (FLNReaderReadUntil(reader, '\r', &request->version) != 0) {
        return -1;
    }

    // LF
    if (FLNReaderReadByte(reader) != '\n') {
        return -1;
    }

    // Headers
    while (!SkipCLRF(reader)) {
        char *header;
        char *value;

        if (FLNReaderReadUntil(reader, ':', &header) != 0) {
            free(header);
            return -1;
        }

        DiscardAll(reader, ' ');

        if (FLNReaderReadUntil(reader, '\r', &value) != 0) {
            free(header);
            free(value);
            return -1;
        }

        // LF
        if (FLNReaderReadByte(reader) != '\n') {
            free(header);
            free(value);
            return -1;
        }

        FLNHeaderMapSet(&request->headers, header, value);

        free(header);
        free(value);
    }

    // TODO: Handle body
    // TODO: without, read all remaining

    // TODO: Move into separate method
    char *transferEncoding = FLNStringLower(FLNStringTrim(FLNHeaderMapGet(&request->headers, "Transfer-Encoding")));

    if (transferEncoding != NULL && strcmp(transferEncoding, "chunked") == 0) {
        long chunkLength = 0;

        FLNStringBuffer buffer;
        FLNStringBufferCreate(&buffer);

        do {
            char *chunkLengthValue;
            if (FLNReaderReadUntil(reader, '\r', &chunkLengthValue) < 0) {
                free(transferEncoding);
                free(chunkLengthValue);
                return -1;
            }

            // LF
            if (FLNReaderReadByte(reader) != '\n') {
                free(transferEncoding);
                free(chunkLengthValue);
                return -1;
            }

            chunkLength = strtol(chunkLengthValue, NULL, 16);

            if (chunkLength == 0) {
                request->body = FLNStringBufferToString(&buffer);
                break;
            }

            char *chunk = malloc((chunkLength + 1) * sizeof(char));
            if (FLNReaderRead(reader, chunk, chunkLength) < 0) {
                free(transferEncoding);
                free(chunk);
                free(chunkLengthValue);
                return -1;
            }
            chunk[chunkLength] = '\0';
            FLNStringBufferAppendString(&buffer, chunk, (int)chunkLength);

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

    const char *contentLengthValue = FLNHeaderMapGet(&request->headers, "Content-Length");
    if (contentLengthValue != NULL) {
        const long contentLength = strtol(contentLengthValue, NULL, 10);
        request->body = malloc((contentLength + 1) * sizeof(char));
        if (FLNReaderRead(reader, request->body, contentLength) < 0) {
            return -1;
        }
    }

    return 0;
}
