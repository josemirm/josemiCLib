// mutex.c
// José Miguel Rodríguez Marchena (@josemirm)

#include "mutex.h"

Mutex createMutex() {
	#ifdef __WIN_PLATFORM__
		Mutex mtx = (Mutex) malloc(sizeof(struct Mutex_st));

		if (NULL == mtx) {
			perror("createMutex: Error allocating mutex");
			return NULL;
		}

		mtx->mtx = CreateMutex(NULL, FALSE, NULL);

		if (NULL == (mtx->mtx)) {
			printWinError("createMutex: Error creating mutex");
			free(mtx);
			return NULL;
		}

		// It needed to set the "inheritable" attribute to true to make the mutex accesible from
		// all any thread in the process.
		setSecAttribInherit(&(mtx->attr), TRUE);

		return mtx;
	#elif __UNIX_PLATFORM__

		Mutex ret = malloc(sizeof(struct Mutex_st));
		if (NULL == ret) {
			perror("createMutex: Error allocating mutex");
			return NULL;
		}

		if (pthread_mutexattr_init(&(ret->attr))) {
			perror("createMutex: Error creating mutex attributes");
			pthread_mutexattr_destroy(&(ret->attr));
			free(ret);
			ret = NULL;
		}

		if (pthread_mutex_init(&(ret->mtx), &(ret->attr))) {
			perror("createMutex: Error creating mutex");
			pthread_mutexattr_destroy(&(ret->attr));
			free(ret);
			ret = NULL;
		}

		return ret;
	#endif

	return NULL;
}


void destroyMutex(Mutex* mutex) {
	if (NULL == mutex || NULL == *mutex) {
		return;
	}

	#ifdef __WIN_PLATFORM__
		if (!CloseHandle((*mutex)->mtx)) {
			printWinError("destroyMutex: Error destroying mutex");
		}

		free(*mutex);
		*mutex = NULL;
	#elif __UNIX_PLATFORM__
		int ret = pthread_mutex_destroy(&((*mutex)->mtx));
		if (ret) {
			fprintf(stderr, "destroyMutex: Error destroying mutex: %s\n", strerror(ret));
		}

		ret = pthread_mutexattr_destroy(&((*mutex)->attr));
		if (ret) {
			fprintf(stderr, "destroyMutex: Error destroying mutex attribute: %s\n", strerror(ret));
		}

		free(*mutex);
		*mutex = NULL;
	#endif
}


int lockMutex(Mutex mutex) {
	#ifdef __WIN_PLATFORM__
		if ((DWORD) 0xFFFFFFFF == WaitForSingleObject(mutex->mtx, INFINITE)) {
			printWinError("lockMutex: Error locking mutex");
			return -1;
		}

		return 0;
	#elif __UNIX_PLATFORM__
		int ret = pthread_mutex_lock(&(mutex->mtx));
		if (ret) {
			fprintf(stderr, "lockMutex: Error locking mutex: %s\n", strerror(ret));
			return -1;
		}

		return 0;
	#endif

	return -1;
}


int unlockMutex(Mutex mutex) {
	#ifdef __WIN_PLATFORM__
		if (!ReleaseMutex(mutex->mtx)) {
			printWinError("unlockMutex: Error unlocking mutex");
			return -1;
		}

		return 0;
	#elif __UNIX_PLATFORM__
		int ret = pthread_mutex_unlock(&(mutex->mtx));
		if (ret) {
			fprintf(stderr, "unlockMutex: Error unlocking mutex: %s\n", strerror(ret));
			return -1;
		}

		return 0;
	#endif

	return -1;
}
