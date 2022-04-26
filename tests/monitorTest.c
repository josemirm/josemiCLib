// monitorTest.c
// José Miguel Rodríguez Marchena (@josemirm)

#include <stdio.h>

#include "../include/monitor.h"
#include "../include/threads.h"
#include "../include/utils.h"


//-----------------------------------------------------------------------------
// Global variables and definitions
//-----------------------------------------------------------------------------

#define NUM_THREADS 5

Monitor mon;
int running;
int threadError = 0;



//-----------------------------------------------------------------------------
// Thread function
//-----------------------------------------------------------------------------

void* threadFunction(void *arg) {
	int i = *(int*)arg;

	while (running) {
		if (waitAndEnterMonitor(mon, 0)) {
			fprintf(stderr, "Error: waitAndEnterMonitor() failed.\n");
			threadError = 1;
			break;
		}

		if (threadError) {
			break;
		}

		printf("Thread %i awake\n", i);

		if (exitMonitor(mon)) {
			fprintf(stderr, "[!] Thread %d: Error exiting the monitor\n", i);
			threadError = 1;
			break;
		}
	}

	return NULL;
}



//-----------------------------------------------------------------------------
// Simple monitor test
//-----------------------------------------------------------------------------

int simpleMonitorTest(void) {
	puts("Entering simple monitor test");

	int thId = 0;

	// Let the thread run
	running = 1;

	// Create the thread
	Thread th = createThread(threadFunction, (void*) &thId);

	if (NULL == th) {
		printError("[!] Error creating thread\n");
		return EXIT_FAILURE;
	}

	sleepSeconds(1);

	// Stop the thread
	running = 0;
	if (wakeMonitor(mon)) {
		printError("[!] Error waking the monitor\n");
		return EXIT_FAILURE;
	}

	// Join the thread
	if (joinAndDestroyThread(&th, NULL)) {
		printError("[!] Error joining and destroying thread\n");
		return EXIT_FAILURE;
	}

	if (threadError) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}



//-----------------------------------------------------------------------------
// Complex monitor test
//-----------------------------------------------------------------------------

int complexMonitorTest(void) {
	puts("Entering complex monitor test");

	Thread th[NUM_THREADS];
	int thId[NUM_THREADS];
	int threadsCreated = 0;

	// Let threads run
	running = 1;

	// Create the threads
	for (int i=0; i<NUM_THREADS; i++) {
		thId[i] = i;
		th[i] = createThread(threadFunction, (void*) &(thId[i]));
		if (NULL == th[i]) {
			printf("[!] Error creating thread %d\n", i);
			goto stopThreads;
		}

		++threadsCreated;
	}


	// Wakes a thread 100 times
	puts("Waking a (probably) different and arbitrary thread 100 times");
	for (int i=0; i<100; i++) {
		if (wakeMonitor(mon)) {
			puts("[!] Error waking the monitor");
			break;
		}
	}

	// Wakes all threads at the same time four times
	puts("Waking all threads 4 times");
	for (int i=0; i<4; ++i) {
		if (wakeAllMonitor(mon)) {
			puts("[!] Error waking all the monitor");
			break;
		}
	}

	// Wait a second to let the threads finish
	sleepSeconds(1);

	// Stop the threads
	stopThreads:
	running = 0;
	puts ("Stopping thread running...");

	// Join the threads
	for (int i=0; i<threadsCreated; i++) {
		// Wakes all threads at the same time again to stop them
		if (wakeAllMonitor(mon)) {
			puts("[!] Error waking all the monitor");
		}

		void *voidptr;
		if (joinAndDestroyThread(&th[i], &voidptr)) {
			printf("[!] Error joining thread %d\n", i);
			return EXIT_FAILURE;
		}
	}

	if (threadError) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}


//-----------------------------------------------------------------------------
// Main function
//-----------------------------------------------------------------------------

int main(void) {
	if (createMonitor(&mon)) {
		printf("[!] Error creating the monitor\n");
		return EXIT_FAILURE;
	}

	const char* failed = "FAILED";
	const char* passed = "PASSED";

	int result = simpleMonitorTest();
	printf("Result: %s\n\n", (EXIT_SUCCESS == result) ? passed : failed);

	if (EXIT_SUCCESS == result) {
		result = complexMonitorTest();
		printf("Result: %s\n\n", (EXIT_SUCCESS == result && threadError == 0) ? passed : failed);
	}

	destroyMonitor(&mon);
	return result | threadError;
}