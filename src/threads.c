// threads.c
// José Miguel Rodríguez Marchena (@josemirm)

#include "threads.h"


#ifdef __cplusplus
extern "C" {
#endif

Thread createThread(void *(*function)(void*), void *arg) {
	if (NULL == function) {
		return NULL;
	}

	#ifdef __WIN_PLATFORM__
		// CreateThread returns NULL on error
		return CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)function, arg, 0, NULL);
	#endif

	#ifdef __UNIX_PLATFORM__
		pthread_t *thread = (pthread_t *) calloc(1, sizeof(pthread_t));

		if (NULL == thread) {
			printError("Error allocating memory\n");
			return NULL;
		}

		pthread_attr_t attr;

		if (pthread_attr_init(&attr) != 0) {
			perror("Error in pthread_attr_init");
			free(thread);
			return NULL;
		}

		pthread_create(thread, &attr, function, arg);

		return thread;
	#endif

	return NULL;
}


int joinAndDestroyThread(Thread *thread, void **retvalue) {
	if (NULL == *thread) {
		return -1;
	}

	#ifdef __WIN_PLATFORM__
		if (WaitForSingleObject(*thread, INFINITE) != WAIT_OBJECT_0) {
			printWinError("Error waiting for thread to finish");
			return -1;
		}

		CloseHandle(*thread);
	#endif

	#ifdef __UNIX_PLATFORM__
		if (pthread_join(**thread, retvalue)) {
			perror("Error waiting for thread to finish");
			return -1;
		}

		free(*thread);
	#endif

	*thread = NULL;

	return 0;
}


#ifdef __cplusplus
}
#endif
