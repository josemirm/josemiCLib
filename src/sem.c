// sem.c
// José Miguel Rodríguez Marchena (@josemirm)

#include "sem.h"

int createUnnamedSemaphore(Semaphore *sem, const int maxValue) {
	if (NULL == sem) {
		return -1;
	}

	#ifdef __WIN_PLATFORM__
		*sem = CreateSemaphore(NULL, maxValue, maxValue, NULL);
		if (NULL == *sem) {
			printWinError("Error creating unnamed semaphore");
			return -1;
		} else {
			return 0;
		}
	#endif

	#ifdef __UNIX_PLATFORM__
		int ret = sem_init(*sem, 0, maxValue);
		if (0 != ret) {
			perror("(Error creating unnamed semaphore");
		}
		return ret;
	#endif

	return -1;
}


void destroySemaphore(Semaphore *sem) {
	if (NULL == sem || NULL == *sem) {
		return;
	}

	#ifdef __WIN_PLATFORM__
		if (0 == CloseHandle(*sem)) {
			printWinError("Error destroying semaphore");
		} else {
			*sem = NULL;
		}

	#endif

	#ifdef __UNIX_PLATFORM__
		if (0 != sem_destroy(*sem)) {
			perror("(Error destroying semaphore");
		} else {
			*sem = NULL;
		}
	#endif
}


int incrementSemaphore(Semaphore sem) {
	#ifdef __WIN_PLATFORM__
		if (FALSE == ReleaseSemaphore(sem, 1, NULL)) {
			printWinError("Error incrementing semaphore");
			return -1;
		} else {
			return 0;
		}
	#endif

	#ifdef __UNIX_PLATFORM__
		int ret = sem_post(sem);
		if (0 != ret) {
			perror("(Error incrementing semaphore");
		}
		return ret;
	#endif

	return -1;
}


int waitSemaphore(Semaphore sem) {
	#ifdef __WIN_PLATFORM__
		if (WAIT_FAILED == WaitForSingleObject(sem, INFINITE)) {
			printWinError("Error waiting semaphore");
			return -1;
		} else {
			return 0;
		}
	#endif

	#ifdef __UNIX_PLATFORM__
		int ret = sem_wait(sem);
		if (0 != ret) {
			perror("(Error waiting semaphore");
		}
		return ret;
	#endif

	return -1;
}



int tryWaitSemaphore(Semaphore sem) {
	#ifdef __WIN_PLATFORM__
		if (WAIT_FAILED == WaitForSingleObject(sem, 0)) {
			printWinError("Error waiting semaphore");
			return -1;
		} else {
			// After decrementing, increments again to let it unchanged
			if (FALSE == ReleaseSemaphore(sem, 1, NULL)) {
				printWinError("Error incrementing semaphore");
				return -1;
			}

			return 0;
		}
	#endif

	#ifdef __UNIX_PLATFORM__
		return sem_trywait(sem);
	#endif

	return -1;
}
