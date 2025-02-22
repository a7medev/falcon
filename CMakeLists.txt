cmake_minimum_required(VERSION 3.30)
project(http C)

set(CMAKE_C_STANDARD 11)

add_executable(http main.c
        net.c
        net.h
        server.c
        server.h
        parser.c
        parser.h
        messages.h
        string.c
        string.h
        headers.c
        headers.h
        messages.c)
