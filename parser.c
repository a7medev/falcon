//
// Created by Ahmed Elrefaey on 12/02/2025.
//

#include "parser.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma mark - Read utilities

static int ReadUntil(const char *data, const size_t n, const char ch, int *pos, char **dest) {
    const int start = *pos;
    int i = start, size = 1;

    while (i < n && data[i++] != ch) {
        size++;
    }

    if (i == n) {
        return -1;
    }

    char *content = malloc(size * sizeof(char));

    if (content == NULL) {
        return -1;
    }

    strlcpy(content, data + start, size);

    *dest = content;
    *pos = i;

    return 0;
}

static void DiscardAll(const char *data, const size_t n, const char ch, int *pos) {
    int i = *pos;
    while (i < n && data[i] == ch) {
        i++;
    }

    *pos = i;
}

static bool SkipCLRF(const char *data, const size_t n, int *pos) {
    const int i = *pos;
    if (i + 1 >= n) {
        return false;
    }

    if (data[i] == '\r' && data[i + 1] == '\n') {
        *pos = i + 2;
        return true;
    }

    return false;
}

#pragma mark - Parser

int Parse(const char *data, const size_t n, Request *parser) {
    int i = 0;

    // Method
    if (ReadUntil(data, n, ' ', &i, &parser->method) != 0) {
        return -1;
    }

    DiscardAll(data, n, ' ', &i);

    // URL
    if (ReadUntil(data, n, ' ', &i, &parser->url) != 0) {
        return -1;
    }

    DiscardAll(data, n, ' ', &i);

    // Version & CR
    if (ReadUntil(data, n, '\r', &i, &parser->version) != 0) {
        return -1;
    }

    if (i == n) {
        return -1;
    }

    // LF
    if (data[i++] != '\n') {
        return -1;
    }

    // Headers
    while (!SkipCLRF(data, n, &i)) {
        char *header;
        char *value;

        if (ReadUntil(data, n, ':', &i, &header) != 0) {
            return -1;
        }

        DiscardAll(data, n, ' ', &i);

        if (ReadUntil(data, n, '\r', &i, &value) != 0) {
            return -1;
        }

        if (i == n) {
            return -1;
        }

        // LF
        if (data[i++] != '\n') {
            return -1;
        }

        printf("Header: %s: %s\n", header, value);
        free(header);
        free(value);
    }

    // Whichever remains is the body...
    printf("Body: %s", data + i);

    return 0;
}
