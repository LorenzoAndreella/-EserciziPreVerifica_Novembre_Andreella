cmake_minimum_required(VERSION 4.2)
project(es3 C)

set(CMAKE_C_STANDARD 11)

add_executable(es3 main.c
        produttore_filtro.c
        collettore.c)
