// José Miguel Rodríguez Marchena (@josemirm)

#include "condVar.h"


int createCondVar(CondVar *condVarPtr) {
    if (NULL == condVarPtr) {
        return -1;
    }

    *condVarPtr = (CondVar) malloc(sizeof(CondVar_st));
    if (NULL == *condVarPtr) {
        return -1;
    }

    CondVar condVar = *condVarPtr;

    #ifdef __WIN_PLATFORM__
        InitializeConditionVariable(&(condVar->cv));
        InitializeCriticalSection(&(condVar->cs));
        return 0;
    #endif

    #ifdef __UNIX_PLATFORM__
        condVar->cv = (pthread_cond_t*) malloc(sizeof(pthread_cond_t));
        if (NULL == condVar->cv) {
            return -1;
        }

        condVar->mutex = createMutex();
        if (NULL == condVar->mutex) {
            free(condVar->cv);
            free(*condVarPtr);
            *condVarPtr = NULL;
            return -1;
        }

        int ret = pthread_cond_init(condVar->cv, NULL);
        if (0 != ret) {
            free(condVar->cv);
            free(condVar->mutex);
            free(*condVarPtr);
            *condVarPtr = NULL;
            return -1;
        }

        return 0;
    #endif

    return -1;
}


void destroyCondVar(CondVar *condVarPtr) {
    if (NULL == condVarPtr || NULL == *condVarPtr) {
        return;
    }

    #ifdef __WIN_PLATFORM__
        // No need to do anything special
    #endif

    #ifdef __UNIX_PLATFORM__
        CondVar condVar = *condVarPtr;
        pthread_cond_destroy(condVar->cv);
        destroyMutex(condVar->mutex);
    #endif

    free(*condVarPtr);
    *condVarPtr = NULL;
}


void enterCondVar(CondVar condVar) {
    #ifdef __WIN_PLATFORM__
        EnterCriticalSection(&(condVar->cs));
    #endif

    #ifdef __UNIX_PLATFORM__
        lockMutex(condVar->mutex);
    #endif
}


void exitCondVar(CondVar condVar) {
    #ifdef __WIN_PLATFORM__
        LeaveCriticalSection(&(condVar->cs));
    #endif

    #ifdef __UNIX_PLATFORM__
        unlockMutex(condVar->mutex);
    #endif
}


int sleepCondVar(CondVar condVar, Mutex mutex) {
    #ifdef __WIN_PLATFORM__
        if (0 == SleepConditionVariableCS (&(condVar->cv), &(condVar->cs), INFINITE)) {
            // Error happended
            return -1;
        } else {
            return 0;
        }
    #endif

    #ifdef __UNIX_PLATFORM__
        return pthread_cond_wait(condVar, mutex);
    #endif

    return -1;
}


int wakeCondVar(CondVar condVar) {
    #ifdef __WIN_PLATFORM__
        WakeConditionVariable (&(condVar->cv));
        // Return 0 because there is no error
        return 0;
    #endif

    #ifdef __UNIX_PLATFORM__
        return pthread_cond_signal(condVar);
    #endif

    return -1;
}


int wakeAllCondVar(CondVar condVar) {
    #ifdef __WIN_PLATFORM__
        WakeAllConditionVariable(&(condVar->cv));
        // Return 0 because there is no error
        return 0;
    #endif

    #ifdef __UNIX_PLATFORM__
        return pthread_cond_broadcast(condVar);
    #endif

    return -1;
}

