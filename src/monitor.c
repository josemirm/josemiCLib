// José Miguel Rodríguez Marchena (@josemirm)

#include "monitor.h"


int createMonitor(Monitor *monitorPtr) {
    printError("createMonitor: Monitors aren't tested yet. Use at your own risk.\n");

    if (NULL == monitorPtr) {
        return -1;
    }

    *monitorPtr = (Monitor) malloc(sizeof(Monitor_st));
    if (NULL == *monitorPtr) {
        return -1;
    }

    Monitor mon = *monitorPtr;

    #ifdef __WIN_PLATFORM__
        // Those functions not return any kind of error.
        InitializeConditionVariable(&(mon->cv));
        InitializeCriticalSection(&(mon->cs));
        return 0;
    #endif

    #ifdef __UNIX_PLATFORM__
        mon->cv = (pthread_cond_t*) malloc(sizeof(pthread_cond_t));
        if (NULL == mon->cv) {
            return -1;
        }

        mon->mutex = createMutex();
        if (NULL == mon->mutex) {
            free(mon->cv);
            free(*monitorPtr);
            *monitorPtr = NULL;
            return -1;
        }

        int ret = pthread_cond_init(mon->cv, NULL);
        if (0 != ret) {
            free(mon->cv);
            free(mon->mutex);
            free(*monitorPtr);
            *monitorPtr = NULL;
            return -1;
        }

        return 0;
    #endif

    return -1;
}


void destroyMonitor(Monitor *monPtr) {
    if (NULL == monPtr || NULL == *monPtr) {
        return;
    }

    #ifdef __WIN_PLATFORM__
        // No need to do anything special in Windows.
    #endif

    #ifdef __UNIX_PLATFORM__
        Monitor mon = *monPtr;
        pthread_cond_destroy(mon->cv);
        destroyMutex(mon->mutex);
    #endif

    free(*monPtr);
    *monPtr = NULL;
}


void enterMonitor(Monitor mon) {
    #ifdef __WIN_PLATFORM__
        EnterCriticalSection(&(mon->cs));
    #endif

    #ifdef __UNIX_PLATFORM__
        lockMutex(mon->mutex);
    #endif
}


void exitMonitor(Monitor mon) {
    #ifdef __WIN_PLATFORM__
        LeaveCriticalSection(&(mon->cs));
    #endif

    #ifdef __UNIX_PLATFORM__
        unlockMutex(mon->mutex);
    #endif
}


int sleepMonitor(Monitor mon) {
    #ifdef __WIN_PLATFORM__
        if (0 == SleepConditionVariableCS (&(mon->cv), &(mon->cs), INFINITE)) {
            // Error happended
            return -1;
        } else {
            return 0;
        }
    #endif

    #ifdef __UNIX_PLATFORM__
        return pthread_cond_wait(mon->cv, mon->mutex);
    #endif

    return -1;
}


int wakeMonitor(Monitor mon) {
    #ifdef __WIN_PLATFORM__
        WakeConditionVariable (&(mon->cv));
        // Return 0 because there is no error checking in Windows.
        return 0;
    #endif

    #ifdef __UNIX_PLATFORM__
        return pthread_cond_signal(mon);
    #endif

    return -1;
}


int wakeAllMonitor(Monitor mon) {
    #ifdef __WIN_PLATFORM__
        WakeAllConditionVariable(&(mon->cv));
        // Return 0 because there is no error checking in Windows.
        return 0;
    #endif

    #ifdef __UNIX_PLATFORM__
        return pthread_cond_broadcast(mon);
    #endif

    return -1;
}

