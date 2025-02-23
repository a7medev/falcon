//
// Created by Ahmed Elrefaey on 15/02/2025.
//

#include "string.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void StringBufferCreate(StringBuffer *buffer) {
    buffer->string = malloc(sizeof(char));
    buffer->size = 0;
    buffer->capacity = 1;
}

void StringBufferFree(StringBuffer *buffer) {
    free(buffer->string);
    buffer->string = NULL;
    buffer->size = 0;
    buffer->capacity = 0;
}

int StringBufferAppend(StringBuffer *buffer, const char ch) {
    if (buffer->capacity == buffer->size) {
        const int newCapacity = 2 * buffer->capacity;
        char *newString = realloc(buffer->string, newCapacity * sizeof(char));

        if (newString == NULL) {
            return -1;
        }

        buffer->capacity = newCapacity;
        buffer->string = newString;
    }

    buffer->string[buffer->size] = ch;
    buffer->size++;

    return 0;
}

/**
 * Calculates the next 2^k >= n in O(1).
 * This is used to calculate the new capacity of the string buffer when appending a string.
 */
static int NextPowerOfTwo(const int32_t n) {
    if (n == 0) {
        return 1;
    }

    return 1 << (32 - __builtin_clz(n - 1));
}

int StringBufferAppendString(StringBuffer *buffer, const char *s, const int n) {
    const int newSize = n + buffer->size;
    if (buffer->capacity < newSize) {
        const int newCapacity = NextPowerOfTwo(newSize);
        char *newString = realloc(buffer->string, newCapacity * sizeof(char));

        if (newString == NULL) {
            return -1;
        }

        buffer->capacity = newCapacity;
        buffer->string = newString;
    }

    for (int i = 0; i < n; i++) {
        buffer->string[buffer->size + i] = s[i];
    }

    buffer->size = newSize;

    return 0;
}

char *StringBufferToString(StringBuffer *buffer) {
    char *result = strndup(buffer->string, buffer->size);
    StringBufferFree(buffer);
    return result;
}

char *StringLower(char *source) {
    const size_t length = strlen(source);

    for (int i = 0; i < length; i++) {
        source[i] = (char)tolower(source[i]);
    }

    return source;
}

char *StringTrim(const char *source) {
    const size_t length = strlen(source);
    size_t start = 0, end = length - 1;

    while (isspace(source[start])) {
        start++;
    }

    while (isspace(source[end])) {
        end--;
    }

    const size_t trimmedSize = end - start + 2;
    char *result = malloc(trimmedSize * sizeof(char));
    strlcpy(result, source + start, trimmedSize);

    return result;
}

