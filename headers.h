//
// Created by Ahmed Elrefaey on 15/02/2025.
//

#ifndef HEADERS_H
#define HEADERS_H

#include <stdlib.h>
#include <string.h>

#include "string.h"

typedef struct Header {
    char *header;
    char *value;
} Header;

typedef struct HeaderMap {
    Header *headers;
    int count;
    int capacity;
} HeaderMap;

void HeaderMapCreate(HeaderMap *map);
void HeaderMapFree(HeaderMap *map);

char *HeaderMapGet(const HeaderMap *map, const char *header);
int HeaderMapSet(HeaderMap *map, const char *header, const char *value);

int HeaderMapCount(const HeaderMap *map);
Header HeaderMapGetAt(const HeaderMap *map, int index);

#endif //HEADERS_H
