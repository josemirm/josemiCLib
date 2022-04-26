// mutex.h
// José Miguel Rodríguez Marchena (@josemirm)

#pragma once

#include <stdio.h>
#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

	#ifdef __WIN_PLATFORM__
		#include "windows/winutils.h"
		#include <synchapi.h>
		#include <errhandlingapi.h>
		struct Mutex_st {
			HANDLE mtx;
			SECURITY_ATTRIBUTES attr;
		};

		typedef struct Mutex_st* Mutex;
	#endif

	#ifdef __UNIX_PLATFORM__
		#include <pthread.h>
		struct Mutex_st {
			pthread_mutex_t mtx;
			pthread_mutexattr_t attr;
		};
		typedef struct Mutex_st* Mutex;
	#endif

	// Mutex handling functions
	Mutex createMutex();
	void destroyMutex(Mutex *mutex);
	int lockMutex(Mutex mutex);
	int unlockMutex(Mutex mutex);

#ifdef __cplusplus
}
#endif
