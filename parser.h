//
// Created by Ahmed Elrefaey on 12/02/2025.
//

#ifndef PARSER_H
#define PARSER_H

#include <string.h>
#include "request.h"

int Parse(const char *data, size_t n, Request *parser);

#endif //PARSER_H
