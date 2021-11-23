#include "mutex.h"

Mutex createMutex() {
    #ifdef __WIN_PLATFORM__
        HANDLE mutex = CreateMutex(NULL, FALSE, NULL);

        if (NULL == mutex) {
            printWinError("Error creating mutex");
        }

        return mutex;
    #elif __UNIX_PLATFORM__
        pthread_mutex_t *mtx = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
        if (NULL == mtx) {
            perror("Error creating mutex");
            return NULL;
        }

        if (pthread_mutex_init(mtx, NULL)) {
            perror("Error creating mutex");
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
        CloseHandle(*mutex);
    #elif __UNIX_PLATFORM__
        if (pthread_mutex_destroy(*mutex)) {
            perror("Error destroying the thread");
            return;
        }
    #endif

    *mutex = NULL;
}


int lockMutex(Mutex mutex) {
    #ifdef __WIN_PLATFORM__
        if ((DWORD) 0xFFFFFFFF == WaitForSingleObject(mutex, INFINITE)) {
            return -1;
        }
    #elif __UNIX_PLATFORM__
        if (pthread_mutex_lock(mutex)) {
            return -1;
        }
    #endif

    return 0;
}


void unlockMutex(Mutex mutex) {
    #ifdef __WIN_PLATFORM__
        ReleaseMutex(mutex);
    #elif __UNIX_PLATFORM__
        if (pthread_mutex_unlock(mutex)) {
            perror("Error unlocking the thread");
        }
    #endif
}
