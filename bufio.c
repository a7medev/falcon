//
// Created by Ahmed Elrefaey on 22/02/2025.
//

#include "bufio.h"

#include <unistd.h>

#include "string.h"

void FLNReaderCreate(FLNReader *reader, const int fd) {
    reader->fd = fd;
    reader->size = 0;
    reader->pos = 0;
    reader->finished = false;
}

char FLNReaderPeekByte(FLNReader *reader) {
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
    const ssize_t n = read(reader->fd, reader->buffer + 1, FLN_READER_BUFFER_SIZE - 1);
    if (n <= 0) {
        reader->finished = true;
        return -1;
    }

    reader->size = n + 1;
    reader->pos = 1;

    return reader->buffer[reader->pos];
}

char FLNReaderReadByte(FLNReader *reader) {
    const char result = FLNReaderPeekByte(reader);

    if (result >= 0) {
        reader->pos++;
    }

    return result;
}

int FLNReaderBackByte(FLNReader *reader) {
    if (reader->pos < 1) {
        return -1;
    }

    reader->pos--;
    return 0;
}

int FLNReaderReadUntil(FLNReader *reader, const char delim, char **dest) {
    FLNStringBuffer buffer;
    FLNStringBufferCreate(&buffer);

    char ch;
    while ((ch = FLNReaderReadByte(reader)) > 0 && ch != delim) {
        FLNStringBufferAppend(&buffer, ch);
    }

    *dest = FLNStringBufferToString(&buffer);

    if (ch < 0) {
        return -1;
    }

    return 0;
}

int FLNReaderRead(FLNReader *reader, char *buf, const size_t n) {
    int i = 0;
    char ch = 0;
    while (i < n && (ch = FLNReaderReadByte(reader)) > 0) {
        buf[i++] = ch;
    }

    if (ch < 0) {
        return -1;
    }

    return i;
}
