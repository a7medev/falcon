//
// Created by Ahmed Elrefaey on 22/02/2025.
//

#include "bufio.h"

#include <unistd.h>

#include "string.h"

void ReaderCreate(Reader *reader, const int fd) {
    reader->fd = fd;
    reader->size = 0;
    reader->pos = 0;
    reader->finished = false;
}

char ReaderPeekByte(Reader *reader) {
    if (reader->finished) {
        return -1;
    }

    if (reader->pos < reader->size) {
        return reader->buffer[reader->pos];
    }

    /**
     * Save the last byte at the start of the new buffer to allow for ReaderBackByte
     * to work *once* after a read/peek operation.
     */
    if (reader->size > 0) {
        reader->buffer[0] = reader->buffer[reader->size - 1];
    }
    const ssize_t n = read(reader->fd, reader->buffer + 1, FLCN_READER_BUFFER_SIZE - 1);
    if (n <= 0) {
        reader->finished = true;
        return -1;
    }

    reader->size = n + 1;
    reader->pos = 1;

    return reader->buffer[reader->pos];
}

char ReaderReadByte(Reader *reader) {
    const char result = ReaderPeekByte(reader);

    if (result >= 0) {
        reader->pos++;
    }

    return result;
}

int ReaderBackByte(Reader *reader) {
    if (reader->pos < 1) {
        return -1;
    }

    reader->pos--;
    return 0;
}

int ReaderReadUntil(Reader *reader, const char delim, char **dest) {
    StringBuffer buffer;
    StringBufferCreate(&buffer);

    char ch;
    while ((ch = ReaderReadByte(reader)) > 0 && ch != delim) {
        StringBufferAppend(&buffer, ch);
    }

    *dest = StringBufferToString(&buffer);

    if (ch < 0) {
        return -1;
    }

    return 0;
}

int ReaderRead(Reader *reader, char *buf, const size_t n) {
    int i = 0;
    char ch = 0;
    while (i < n && (ch = ReaderReadByte(reader)) > 0) {
        buf[i++] = ch;
    }

    if (ch < 0) {
        return -1;
    }

    return i;
}
