//
// Created by Ahmed Elrefaey on 15/02/2025.
//

#ifndef HEADERS_H
#define HEADERS_H

typedef struct FLNHeader {
    char *header;
    char *value;
} FLNHeader;

typedef struct FLNHeaderMap {
    FLNHeader *headers;
    int count;
    int capacity;
} FLNHeaderMap;

void FLNHeaderMapCreate(FLNHeaderMap *map);
void FLNHeaderMapFree(FLNHeaderMap *map);

char *FLNHeaderMapGet(const FLNHeaderMap *map, const char *header);
int FLNHeaderMapSet(FLNHeaderMap *map, const char *header, const char *value);

int FLNHeaderMapCount(const FLNHeaderMap *map);
FLNHeader FLNHeaderMapGetAt(const FLNHeaderMap *map, int index);

#endif //HEADERS_H
