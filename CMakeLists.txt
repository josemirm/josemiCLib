cmake_minimum_required(VERSION 3.4)
project(josemiCLib	VERSION 0.1.0
					DESCRIPTION "josemiCLib"
					LANGUAGES C
)

# Set project global include directory
include_directories(BEFORE include)

# Set utilities library
add_library(josemiCUtils STATIC src/utils.c)

# Set FIFO library
add_library(josemiFifoLib STATIC src/fifo.c)

# Show more warnings and analyze the code looking for common mistakes
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g -fanalyzer")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")
endif()

if (UNIX)
	set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
	set(THREADS_PREFER_PTHREAD_FLAG TRUE)
	find_package(Threads REQUIRED)

	# Get UNIX utilities and FIFO specific libraries and link them with fifo.c and utils.c
	# libraries.
	add_library(josemiUnixUtils STATIC src/unix/unixutils.c)
	target_include_directories(josemiUnixUtils PUBLIC include/unix)

	add_library(josemiUnixFifo STATIC src/unix/unixfifo.c)
	target_include_directories(josemiUnixFifo PUBLIC include/unix)

	target_link_libraries(josemiCUtils PUBLIC josemiUnixUtils)
	target_link_libraries(josemiFifoLib PUBLIC josemiUnixFifo josemiCUtils)
endif()

if (WIN32)
	# Get Windows utilities and FIFO specific libraries and link them with fifo.c and utils.c
	# libraries.
	add_library(josemiWinUtils STATIC src/windows/winutils.c)
	target_include_directories(josemiWinUtils PUBLIC include/windows)

	add_library(josemiWinFifo STATIC src/windows/winfifo.c)
	target_include_directories(josemiWinFifo PUBLIC include/windows)

	target_link_libraries(josemiCUtils PUBLIC josemiWinUtils)
	target_link_libraries(josemiFifoLib PUBLIC josemiWinFifo josemiCUtils)
endif()

# Set mutex library
add_library(josemiMutexLib STATIC src/mutex.c)
target_link_libraries(josemiMutexLib PUBLIC josemiCUtils)

# Set monitor library
add_library(josemiMonitorLib STATIC src/monitor.c)
target_link_libraries(josemiMonitorLib PUBLIC josemiCUtils josemiMutexLib)

# Set semaphore library
add_library(josemiSemaphoreLib STATIC src/sem.c)
target_link_libraries(josemiSemaphoreLib PUBLIC josemiCUtils)

# Set threads library
add_library(josemiThreadsLib STATIC src/threads.c)

# pthread library needed only for UNIX OSs, not for Windows
if (UNIX)
	target_link_libraries(josemiThreadsLib PUBLIC pthread josemiCUtils)
elseif(WIN32)
	target_link_libraries(josemiThreadsLib PUBLIC josemiCUtils)
endif()

# Set test files
include(CTest)
enable_testing()

add_executable(mutexTest tests/mutexTest.c)
target_link_libraries(mutexTest PRIVATE josemiCUtils josemiMutexLib josemiThreadsLib)
add_test(NAME MutexTest
		 COMMAND $<TARGET_FILE:mutexTest>
)

add_executable(monitorTest tests/monitorTest.c)
target_link_libraries(monitorTest PRIVATE josemiCUtils josemiMonitorLib josemiThreadsLib)
add_test(NAME MonitorTest
		 COMMAND $<TARGET_FILE:monitorTest>
)
