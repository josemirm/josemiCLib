// José Miguel Rodríguez Marchena (@josemirm)

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    #include <stdio.h>
    #include "utils.h"

    #ifdef __WIN_PLATFORM__
        #include "windows/winutils.h"
        #include <synchapi.h>
        #include <errhandlingapi.h>
        typedef HANDLE Mutex;
    #endif
    
    #ifdef __UNIX_PLATFORM__
        #include <pthread.h>
        typedef pthread_mutex_t* Mutex;
    #endif


    // Mutex handling functions
    Mutex createMutex();
    void destroyMutex(Mutex *mutex);
    int lockMutex(Mutex mutex);
    int unlockMutex(Mutex mutex);

#ifdef __cplusplus
}
#endif
