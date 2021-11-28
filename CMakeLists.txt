cmake_minimum_required(VERSION 3.4)
project(josemiCLib VERSION 0.1.0
                    DESCRIPTION "josemiCLib"
                    LANGUAGES C)

include(CTest)
enable_testing()


add_library(josemiCUtils src/unix/unixutils.c src/windows/winutils.c src/utils.c)
target_include_directories(josemiCUtils PUBLIC include include/unix include/windows)

add_library(josemiCLib src/unix/unixfifo.c src/windows/winfifo.c
    src/monitor.c src/fifo.c src/mutex.c src/semaphore.c src/threads.c)
target_include_directories(josemiCLib PUBLIC include include/unix include/windows)
target_link_libraries(josemiCLib josemiCUtils)

include(CPack)
