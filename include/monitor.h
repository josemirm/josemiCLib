// monitor.h
// José Miguel Rodríguez Marchena (@josemirm)

#pragma once

#include "utils.h"
#include "mutex.h"

#ifdef __cplusplus
extern "C" {
#endif

	#ifdef __WIN_PLATFORM__
		typedef struct {
			CONDITION_VARIABLE  cv;
			CRITICAL_SECTION    cs;
		} Monitor_st;

		typedef Monitor_st* Monitor;
	#endif

	#ifdef __UNIX_PLATFORM__
		#include <errno.h>

		typedef struct {
			pthread_condattr_t attr;
			pthread_cond_t  cv;
			Mutex condMtx;
		} Monitor_st;

		typedef Monitor_st* Monitor;
	#endif

	int createMonitor(Monitor *monPtr);
	void destroyMonitor(Monitor *monPtr);

	int enterMonitor(Monitor mon);
	int waitAndEnterMonitor(Monitor mon, const unsigned timeout);
	int exitMonitor(Monitor mon);
	int wakeMonitor(Monitor mon);
	int wakeAllMonitor(Monitor mon);

#ifdef __cplusplus
}
#endif
