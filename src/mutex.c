#include "mutex.h"

Mutex createMutex() {
    #ifdef __WIN_PLATFORM__
        HANDLE mutex = CreateMutex(NULL, FALSE, NULL);

        if (NULL == mutex) {
            printWinError("createMutex: Error creating mutex");
        }

        return mutex;
    #elif __UNIX_PLATFORM__
        pthread_mutex_t *mtx = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
        if (NULL == mtx) {
            perror("createMutex: Error creating mutex");
            return NULL;
        }

        if (pthread_mutex_init(mtx, NULL)) {
            perror("createMutex: Error creating mutex");
            mtx = NULL;
        }

        return mtx;
    #endif

    return NULL;
}


void destroyMutex(Mutex *mutex) {
    if (NULL == mutex || NULL == *mutex) {
        return;
    }

    #ifdef __WIN_PLATFORM__
        if (!CloseHandle(*mutex)) {
            printWinError("destroyMutex: Error destroying mutex");
            return;
        }
    #elif __UNIX_PLATFORM__
        if (pthread_mutex_destroy(*mutex)) {
            perror("destroyMutex: Error destroying mutex");
            return;
        }
    #endif

    *mutex = NULL;
}


int lockMutex(Mutex mutex) {
    #ifdef __WIN_PLATFORM__
        if ((DWORD) 0xFFFFFFFF == WaitForSingleObject(mutex, INFINITE)) {
            printWinError("lockMutex: Error locking mutex");
            return -1;
        }

        return 0;
    #elif __UNIX_PLATFORM__
        if (pthread_mutex_lock(mutex)) {
            perror("lockMutex: Error locking mutex");
            return -1;
        }

        return 0;
    #endif

    return -1;
}


int unlockMutex(Mutex mutex) {
    #ifdef __WIN_PLATFORM__
        if (!ReleaseMutex(mutex)) {
            printWinError("unlockMutex: Error unlocking mutex");
            return -1;
        }

        return 0;
    #elif __UNIX_PLATFORM__
        if (pthread_mutex_unlock(mutex)) {
            perror("unlockMutex: Error unlocking mutex");
            return -1;
        }

        return 0;
    #endif

    return -1;
}
