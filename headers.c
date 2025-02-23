//
// Created by Ahmed Elrefaey on 15/02/2025.
//

#include "headers.h"

void HeaderMapCreate(HeaderMap *map) {
    map->headers = malloc(sizeof(Header));
    map->count = 0;
    map->capacity = 1;
}

void HeaderMapFree(HeaderMap *map) {
    for (int i = 0; i < map->count; i++) {
        free(map->headers[i].header);
        free(map->headers[i].value);
    }

    free(map->headers);
    map->headers = NULL;
    map->count = 0;
    map->capacity = 0;
}

char *HeaderMapGet(const HeaderMap *map, const char *header) {
    char *canonical = StringLower(StringTrim(header));

    for (int i = 0; i < map->count; i++) {
        const Header current = map->headers[i];
        if (strcmp(current.header, canonical) == 0) {
            free(canonical);
            return current.value;
        }
    }

    free(canonical);
    return NULL;
}

int HeaderMapSet(HeaderMap *map, const char *header, const char *value) {
    char *canonical = StringLower(StringTrim(header));

    for (int i = 0; i < map->count; i++) {
        Header *current = &map->headers[i];
        if (strcmp(current->header, canonical) == 0) {
            free(current->value);
            free(canonical);

            current->value = strdup(value);
            return 0;
        }
    }

    if (map->capacity == map->count) {
        const int newCapacity = 2 * map->capacity;
        Header *newHeaders = realloc(map->headers, newCapacity * sizeof(Header));

        if (newHeaders == NULL) {
            free(canonical);
            return -1;
        }

        map->capacity = newCapacity;
        map->headers = newHeaders;
    }

    map->headers[map->count].header = canonical;
    map->headers[map->count].value = strdup(value);
    map->count++;

    return 0;
}

int HeaderMapCount(const HeaderMap *map) {
    return map->count;
}

Header HeaderMapGetAt(const HeaderMap *map, const int index) {
    return map->headers[index];
}
