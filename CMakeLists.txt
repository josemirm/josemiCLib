cmake_minimum_required(VERSION 3.4)
project(josemiCLib VERSION 0.1.0
                    DESCRIPTION "josemiCLib"
                    LANGUAGES C)

include(CTest)
enable_testing()

add_library(josemiCLib src/unix/unixfifo.c src/unix/unixutils.c src/windows/winfifo.c
    src/windows/winutils.c src/condVar.c src/fifo.c src/mutex.c src/semaphore.c src/threads.c
    src/utils.c)
target_include_directories(josemiCLib PUBLIC src/unix src/windows)

include(CPack)
