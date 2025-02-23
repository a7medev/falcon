//
// Created by Ahmed Elrefaey on 15/02/2025.
//

#include <stdlib.h>
#include <string.h>

#include "headers.h"
#include "string.h"

void FLNHeaderMapCreate(FLNHeaderMap *map) {
    map->headers = malloc(sizeof(FLNHeader));
    map->count = 0;
    map->capacity = 1;
}

void FLNHeaderMapFree(FLNHeaderMap *map) {
    for (int i = 0; i < map->count; i++) {
        free(map->headers[i].header);
        free(map->headers[i].value);
    }

    free(map->headers);
    map->headers = NULL;
    map->count = 0;
    map->capacity = 0;
}

char *FLNHeaderMapGet(const FLNHeaderMap *map, const char *header) {
    char *canonical = FLNStringLower(FLNStringTrim(header));

    for (int i = 0; i < map->count; i++) {
        const FLNHeader current = map->headers[i];
        if (strcmp(current.header, canonical) == 0) {
            free(canonical);
            return current.value;
        }
    }

    free(canonical);
    return NULL;
}

int FLNHeaderMapSet(FLNHeaderMap *map, const char *header, const char *value) {
    char *canonical = FLNStringLower(FLNStringTrim(header));

    for (int i = 0; i < map->count; i++) {
        FLNHeader *current = &map->headers[i];
        if (strcmp(current->header, canonical) == 0) {
            free(current->value);
            free(canonical);

            current->value = strdup(value);
            return 0;
        }
    }

    if (map->capacity == map->count) {
        const int newCapacity = 2 * map->capacity;
        FLNHeader *newHeaders = realloc(map->headers, newCapacity * sizeof(FLNHeader));

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

int FLNHeaderMapCount(const FLNHeaderMap *map) {
    return map->count;
}

FLNHeader FLNHeaderMapGetAt(const FLNHeaderMap *map, const int index) {
    return map->headers[index];
}
