//
// Created by Ahmed Elrefaey on 15/02/2025.
//

#include "string.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

char *StringLower(const char *source) {
    const size_t length = strlen(source);
    char *result = malloc(length * sizeof(char));

    for (int i = 0; i < length; i++) {
        result[i] = (char)tolower(source[i]);
    }

    return result;
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

