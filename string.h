//
// Created by Ahmed Elrefaey on 15/02/2025.
//

#ifndef STRING_H
#define STRING_H

typedef struct FLNStringBuffer {
    char *string;
    int size;
    int capacity;
} FLNStringBuffer;

void FLNStringBufferCreate(FLNStringBuffer *buffer);
void FLNStringBufferFree(FLNStringBuffer *buffer);
int FLNStringBufferAppend(FLNStringBuffer *buffer, char ch);
int FLNStringBufferAppendString(FLNStringBuffer *buffer, const char *s, int n);
char *FLNStringBufferToString(FLNStringBuffer *buffer);

/// Turns all characters in the source string into lowercase characters.
char *FLNStringLower(char *source);

/// Trims space at the beginning and end of the source string.
/// @warning The string returned is heap allocated and must be freed after it's no longer used.
char *FLNStringTrim(const char *source);

#endif //STRING_H
