cmake_minimum_required(VERSION 3.27)
project(untitled1 C)

set(CMAKE_C_STANDARD 11)

add_executable(CC main.c
        pathfinding.h
        pathfinding.c
        loc.c
        loc.h
        moves.c
        moves.h
        map.c
        map.h
        queue.c
        queue.h
        stack.c
        stack.h
        tree.h
        tree.c
        starter.c
        starter.h
)
