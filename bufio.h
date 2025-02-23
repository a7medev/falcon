//
// Created by Ahmed Elrefaey on 22/02/2025.
//

#ifndef BUFIO_H
#define BUFIO_H

#include <string.h>
#include <stdbool.h>

#define FLN_READER_BUFFER_SIZE 4096

typedef struct FLNReader {
    char buffer[FLN_READER_BUFFER_SIZE];
    size_t size;
    int pos;
    int fd;
    bool finished;
} FLNReader;

void FLNReaderCreate(FLNReader *reader, int fd);

/**
 * Reads a single byte and changes the position pointer to move to the next byte.
 * @returns The character read if there are bytes to read and there's no error, and -1 otherwise.
 */
char FLNReaderReadByte(FLNReader *reader);

/**
 * Reads a single byte and keeps the position pointer unchanged.
 * @returns The character read if there are bytes to read and there's no error, and -1 otherwise.
 */
char FLNReaderPeekByte(FLNReader *reader);

/**
 * Moves the position pointer back to the previous byte.
 * @warning This can be used *only* once after a `ReaderPeekByte` call and twice after a `ReaderReadByte`.
 * @return 0 on success, otherwise returns -1.
 */
int FLNReaderBackByte(FLNReader *reader);

int FLNReaderReadUntil(FLNReader *reader, char delim, char **dest);

int FLNReaderRead(FLNReader *reader, char *buf, size_t n);

#endif //BUFIO_H
