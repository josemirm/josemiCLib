// José Miguel Rodríguez Marchena (@josemirm)

#pragma once

#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

    #ifdef __WIN_PLATFORM__
        #include "windows/winutils.h"
        typedef HANDLE Semaphore;
    #endif

    #ifdef __UNIX_PLATFORM__
        #include "unix/unixutils.h"
        #include <fcntl.h>
        #include <sys/stat.h>
        #include <semaphore.h>

        typedef sem_t* Semaphore;
    #endif


    // NOTE: Semaphore initial value is the max value configured
    int createUnnamedSemaphore(Semaphore *sem, const int maxValue);
    void destroySemaphore(Semaphore *sem);

    int incrementSemaphore(Semaphore sem);
    int waitSemaphore(Semaphore sem);
    int tryWaitSemaphore(Semaphore sem);

#ifdef __cplusplus
}
#endif
