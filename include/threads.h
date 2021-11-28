// José Miguel Rodríguez Marchena (@josemirm)

#pragma once

#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

    #ifdef __WIN_PLATFORM__
        #include "windows/winutils.h"
        typedef HANDLE Thread;
    #endif

    #ifdef __UNIX_PLATFORM__
        #include "unix/unixutils.h"
        #include <pthread.h>
        typedef pthread_t* Thread;
    #endif

    Thread createThread(void *(*function)(void*), void *arg);
    int joinAndDestroyThread(Thread *thread, void **retvalue);

#ifdef __cplusplus
}
#endif
