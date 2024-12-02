cmake_minimum_required(VERSION 3.29)
project(P01 C)

set(CMAKE_C_STANDARD 99)

add_executable(P01 main.c
        Functions.c
        Functions.h
        CommandList.c
        CommandList.h
        HistoryList.c
        HistoryList.h
        FileList.c
        FileList.h
        Commands.c
        Commands.h
        MemoryBlockList.c
        MemoryBlockList.h
        ProcessesList.h
        ProcessesList.c
        DirectoryList.h
        DirectoryList.c)
