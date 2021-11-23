// José Miguel Rodríguez Marchena (@josemirm)

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    #include "utils.h"
    #include "mutex.h"

    #ifdef __WIN_PLATFORM__
        typedef struct {
            CONDITION_VARIABLE  cv;
            CRITICAL_SECTION    cs;
        } CondVar_st;

        typedef CondVar_st* CondVar;
    #endif

    #ifdef __UNIX_PLATFORM__
        typedef struct {
            pthread_cond_t  *cv;
            Mutex mutex;
        } CondVar_st;

        typedef CondVar_st* CondVar;
    #endif

    int createCondVar(CondVar *condVarPtr);
    void destroyCondVar(CondVar *condVarPtr);

    void enterCondVar(CondVar condVar);
    void exitCondVar(CondVar condVar);

    int sleepCondVar(CondVar condVar, Mutex mutex);
    int wakeCondVar(CondVar condVar);
    int wakeAllCondVar(CondVar condVar);

#ifdef __cplusplus
}
#endif
