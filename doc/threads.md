## Threads library

A thread library for Windows and Unix.

A thread can run a function in parallel to the rest of the program, and every program/process can run multiple threads, and those threads, can have the same or different functions between them.

The limits of the number of threads depends of the OS and the machine running the program, and knowing this library uses Windows native threads and UNIX `pthread` library, you can know more about this kind of limits looking the documentation of your OS threads.


- `Thread:`

	Thread type used by the library.
	
	


- `Thread createThread(void *(*function)(void*), void *arg):`

	Create and start a thread that runs the given function.
	The input function should be able to receive a `void*` value and return a `void*` value, and the `void *arg` value will be passed to that input function.

	Example function that could be used as an argument:
	```c
	
	// Function to run in a thread
	void* myFunction(void* arg) {
		if (NULL == arg) {
			return NULL;
		}
	
		int *oldInt = (int) arg;
		
		int *newInt = (int*) malloc(sizeof(int));
		if (NULL == newInt) {
			return NULL;
		}
	
		*newInt = *oldInt + 1;
	
		return ( (void*) newInt );
	}
	
	```

	Any casted value could be passed as `void *arg` if this value has the same or less size than `void*`, but is recommended to pass pointers instead of values.

	The return value of `createThread(...)` will be NULL in the case of error and a valid `Thread` value if the thread is created.

	Example usage of the function:
	```c
	
	void* myFunction(void* arg);
	int myValue = 123;
	
	Thread th = createThread(&myFunction, &myValue);
	
	if (NULL == th) {
		printf("Error creating thread\n");
	} else {
		printf("Thread created successfully\n");
	}
	
	```




- `int joinAndDestroyThread(Thread *thread, void **retvalue):`

	Waits to `*thread` to end, and writes the returning value to `**retvalue`. This function returns `0` if the thread is joined correctly and return other values otherwise.

	In a successful case, the `*thread` value is set to `NULL` to let the user know there 

	Example usage of the function:
	```c
	
	//...
	
	Thread th = createThread(&myFunction, &myValue);
	
	//...
	
	int returnValue = 0;
	if ( joinAndDestroyThread(&th, (void*) &returnValue) ) {
		printf("Error joining the thread\n");
	} else {
		printf("Value returned from the thread: %d\n", returnValue);
	}
	
	//...
	
	```

