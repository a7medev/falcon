//
// Created by Ahmed Elrefaey on 15/02/2025.
//

#ifndef STRING_H
#define STRING_H

typedef struct StringBuffer {
    char *string;
    int size;
    int capacity;
} StringBuffer;

void StringBufferCreate(StringBuffer *buffer);
void StringBufferFree(StringBuffer *buffer);
int StringBufferAppend(StringBuffer *buffer, char ch);
int StringBufferAppendString(StringBuffer *buffer, const char *s, int n);
char *StringBufferToString(StringBuffer *buffer);

/// Turns all characters in the source string into lowercase characters.
char *StringLower(char *source);

/// Trims space at the beginning and end of the source string.
/// @warning The string returned is heap allocated and must be freed after it's no longer used.
char *StringTrim(const char *source);

#endif //STRING_H
