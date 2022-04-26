## Mutex library

Mutex library for Windows and Unix.

Mutex are syncronization tools used to lock some threads while other are running and accesing a shared space.

A thread wait until there is no other thread using that mutex, then lock the mutex to prevent others doing the same, then have access to the shared space and unlock the mutex at the end to wake the other threads waiting.

---

### Types

- `Mutex`

	Mutex type used by the library.

---

### Examples

Example of `createMutex()` and `destroyMutex()`:

```c
	Mutex mtx = createMutex();

	if (NULL != mtx) {
		printf("Mutex created successfully");
	} else {
		printf("Error creating mutex");
		abort();
	}

	destroyMutex(&mtx);
```


Example of mutex usage with `lockMutex()` and `unlockMutex()` with a thread function:
```c
void* threadFunction(void* args) {
	// Global mutex created previously
	if (lockMutex(mtx)) {
		fprintf(stderr, "Error locking mutex\n");
		return NULL;
	}

	// If there are multiple threads running this function, the process done up
	// next would be execute only in one thread, stopping the rest of the
	// threads from accessing the same shared space.
	doSomeProcess(&someSharedSpace);

	if (unlockMutex(mtx)) {
		fprintf(stderr, "Error unlocking mutex\n");
	}

	return NULL;
}
```

---

### Functions

- `Mutex createMutex()`

	Create a mutex.

	Returns a `Mutex` value on a successful execution or return `NULL` when there is any kind of error.

	&nbsp;


- `void destroyMutex(Mutex *mutex)`

	Destroy the mutex and set `*mutex` value to NULL.

	&nbsp;


- `int lockMutex(Mutex mutex)`

	Waits until the mutex is unlocked blocking the thread efficiently (using 0% of the CPU). 

	Returns `0` on a successful execution or return non-zero otherwise.

	&nbsp;


- `int unlockMutex(Mutex mutex)`
	
	Unlock the mutex, letting waiting threads continue their execution

	Returns `0` on a successful execution or return non-zero otherwise.
