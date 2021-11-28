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
        } Monitor_st;

        typedef Monitor_st* Monitor;
    #endif

    #ifdef __UNIX_PLATFORM__
        typedef struct {
            pthread_cond_t  *cv;
            Mutex mutex;
        } Monitor_st;

        typedef Monitor_st* Monitor;
    #endif

    int createMonitor(Monitor *monPtr);
    void destroyMonitor(Monitor *monPtr);

    void enterMonitor(Monitor mon);
    void exitMonitor(Monitor mon);

    int sleepMonitor(Monitor mon);
    int wakeMonitor(Monitor mon);
    int wakeAllMonitor(Monitor mon);

#ifdef __cplusplus
}
#endif
