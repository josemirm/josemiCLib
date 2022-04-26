// monitor.c
// José Miguel Rodríguez Marchena (@josemirm)

#include "monitor.h"


int createMonitor(Monitor *monitorPtr) {
	if (NULL == monitorPtr) {
		return -1;
	}

	*monitorPtr = (Monitor) malloc(sizeof(Monitor_st));
	if (NULL == *monitorPtr) {
		return -1;
	}

	#ifdef __WIN_PLATFORM__
		// Those functions not return any kind of error.
		InitializeConditionVariable(&((*monitorPtr)->cv));
		InitializeCriticalSection(&((*monitorPtr)->cs));
		return 0;
	#endif

	#ifdef __UNIX_PLATFORM__
		Monitor mon = *monitorPtr;

		mon->condMtx = createMutex();
		if (NULL == mon->condMtx) {
			free(*monitorPtr);
			*monitorPtr = NULL;
			return -1;
		}

		int ret = pthread_condattr_init(&(mon->attr));
		if (0 != ret) {
			destroyMutex(&(mon->condMtx));
			free(*monitorPtr);
			*monitorPtr = NULL;
			return -1;
		}
/*
		// This attribute makes the conditional variable acessible outside this thread. This is
		// necessary to work on BSD as tested.
		pthread_condattr_setpshared(&(mon->attr), PTHREAD_PROCESS_SHARED);
*/
		ret = pthread_cond_init(&(mon->cv), &(mon->attr));
		if (0 != ret) {
			destroyMutex(&(mon->condMtx));
			pthread_condattr_destroy(&(mon->attr));
			/*destroyMutex(&(mon->monitorMtx));*/
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

		int cond_ret = pthread_cond_destroy(&(mon->cv));
		if (0 != cond_ret) {
			printf("destroyMonitor: Error destroying condition variable: %s\n",
					strerror(cond_ret));
			return;
		}

		destroyMutex(&(mon->condMtx));
		/*destroyMutex(&(mon->monitorMtx));*/
	#endif

	free(*monPtr);
	*monPtr = NULL;
}


int enterMonitor(Monitor mon) {
	#ifdef __WIN_PLATFORM__
		// Windows function doesn't return any kind of error.
		EnterCriticalSection(&(mon->cs));
		return 0;
	#endif

	#ifdef __UNIX_PLATFORM__
		return lockMutex(mon->condMtx);
	#endif

	return -1;
}


int exitMonitor(Monitor mon) {
	#ifdef __WIN_PLATFORM__
		// Windows function doesn't return any kind of error.
		LeaveCriticalSection(&(mon->cs));
		return 0;
	#endif

	#ifdef __UNIX_PLATFORM__
		return unlockMutex(mon->condMtx);
	#endif

	return -1;
}


int waitAndEnterMonitor(Monitor mon, const unsigned timeout) {
	#ifdef __WIN_PLATFORM__
		// timeout is in milliseconds.
		DWORD winTimeout;

		if (0 == timeout) {
			winTimeout = INFINITE;
		} else {
			winTimeout = timeout;
		}

		int ret = 0;

		if ( 0 == SleepConditionVariableCS (&(mon->cv), &(mon->cs), winTimeout) &&
			 ERROR_TIMEOUT != GetLastError() ) {
				// If there is an error and isn't a timeout, notifies the error
				// returning -1.
				ret = -1;
		}

		if (0 == ret) {
			// If there is no error, enters the monitor.
			EnterCriticalSection(&(mon->cs));
		}

		return ret;
	#endif

	#ifdef __UNIX_PLATFORM__
		lockMutex(mon->condMtx);
		if (0 == timeout) {
			int waitRet = pthread_cond_wait(&(mon->cv), &(mon->condMtx->mtx));
			if (waitRet) {
				fprintf(stderr,
					"waitAndEnterMonitor: Error in pthread_cond_wait (%s)\n",
					strerror(waitRet)
					);
			}

			return waitRet;
		}

		struct timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);

		ts.tv_sec += timeout / 1000;
		ts.tv_nsec += (timeout % 1000) * 1000000;

		int waitRet = pthread_cond_timedwait(&(mon->cv), &(mon->condMtx->mtx), &ts);

		// If there is a timeout, and the error number is ETIMEDOU, there is no
		// real error and returns 0.
		if (waitRet != 0 && waitRet != ETIMEDOUT) {
			fprintf(stderr,
				"waitAndEnterMonitor: Error in pthread_cond_timedwait (%s)\n",
				strerror(waitRet)
			);
		}

		return waitRet;
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
		return pthread_cond_signal(&(mon->cv));
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
		return pthread_cond_broadcast(&(mon->cv));
	#endif

	return -1;
}
