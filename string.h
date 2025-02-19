//
// Created by Ahmed Elrefaey on 15/02/2025.
//

#ifndef STRING_H
#define STRING_H

/// Turns all characters in the source string into lowercase characters.
/// @warning The string returned is heap allocated and must be freed after it's no longer used.
char *StringLower(const char *source);

/// Trims space at the beginning and end of the source string.
/// @warning The string returned is heap allocated and must be freed after it's no longer used.
char *StringTrim(const char *source);

#endif //STRING_H
