// mutexTest.c
// José Miguel Rodríguez Marchena (@josemirm)

#include <stdio.h>

#include "../include/mutex.h"
#include "../include/threads.h"
#include "../include/utils.h"



//-----------------------------------------------------------------------------
// Data declaration and global definitions
//-----------------------------------------------------------------------------

#define THREAD_COUNT 20

struct ThreadData_st {
	Mutex mtx;
	int* number;
	int id;
};

typedef struct ThreadData_st ThreadData;



//-----------------------------------------------------------------------------
// Thread function
//-----------------------------------------------------------------------------

void* threadFunction(void* args) {
	ThreadData* data = (ThreadData*) args;

	for (int i=0; i<THREAD_COUNT; ++i) {
		if (lockMutex(data->mtx)) {
			fprintf(stderr, "Thread %i: Error locking mutex\n", data->id);
			break;
		}

		++(*(data->number));

		if (unlockMutex(data->mtx)){
			fprintf(stderr, "Thread %i: Error unlocking mutex\n", data->id);
			break;
		}
	}

	return NULL;
}


//-----------------------------------------------------------------------------
// Multithread test function
//-----------------------------------------------------------------------------

int multithreadUse(void) {
	puts("Entering mutex multithread test");
	int number = 0;
	int thCreated = 0;
	Mutex mtx = createMutex();
	if (!mtx) {
		return 1;
	}

	Thread th[THREAD_COUNT];
	ThreadData data[THREAD_COUNT];
	for (int i=0; i<THREAD_COUNT; ++i) {
		data[i].id = i;
		data[i].number = &number;
		data[i].mtx = mtx;

		th[i] = createThread(threadFunction, &(data[i]));
		if (!th[i]) {
			fprintf(stderr, "multiThreadUse: Error creating thread %i\n\n", i);
			break;
		}
		++thCreated;
	}

	for (int i=0; i<thCreated; ++i) {
		void* ret;
		joinAndDestroyThread(&(th[i]), &ret);
	}

	destroyMutex(&mtx);

	printf("Final count: %i/%i\n", (number), THREAD_COUNT*THREAD_COUNT);
	puts("Exiting mutex multithread test");

	if (thCreated < THREAD_COUNT) {
		return -1;
	}

	return 0;
}



//-----------------------------------------------------------------------------
// Single thread test function
//-----------------------------------------------------------------------------

int basicUse(void) {
	puts("Entering mutex basic test");
	Mutex mtx = createMutex();
	if (!mtx || lockMutex(mtx) || unlockMutex(mtx)) {
		return 1;
	};

	destroyMutex(&mtx);
	puts("Exiting mutex basic test\n");
	return 0;
}



//-----------------------------------------------------------------------------
// Main function
//-----------------------------------------------------------------------------

int main(void) {
	if (basicUse() || multithreadUse()) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}