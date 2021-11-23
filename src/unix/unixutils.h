// José Miguel Rodríguez Marchena (@josemirm)

#pragma once

#include "../utils.h"


#ifdef __cplusplus
extern "C" {
#endif

    #ifdef __UNIX_PLATFORM__

    #include <unistd.h>
    #include <errno.h>

    ssize_t write_n(const int fd, const void* buffer, const size_t count);
    ssize_t read_n(const int fd, void* buffer, const size_t count);

    #endif

#ifdef __cplusplus
}
#endif