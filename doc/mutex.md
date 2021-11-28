## Mutex library

Mutex library for Windows and Unix.

Mutex are syncronization tools used to lock some threads while other are running and accesing a shared space.

A thread wait until there is no other thread using that mutex, then lock the mutex to prevent others doing the same, then have access to the shared space and unlock the mutex at the end to wake the other threads waiting.

---

- `Mutex`:

	Mutex type used by the library.

---

- `Mutex createMutex():`

	Create a mutex.

	Returns a `Mutex` value on a successful execution or return `NULL` when there is any kind of error.


- `void destroyMutex(Mutex *mutex):`

	Destroy the mutex and set `*mutex` value to NULL.



- `int lockMutex(Mutex mutex):`

	Waits until the mutex is unlocked blocking the thread efficiently (using 0% of the CPU). 

	Returns `0` on a successful execution or return non-zero otherwise.


- `int unlockMutex(Mutex mutex):`
	
	Unlock the mutex, letting waiting threads continue their execution

	Returns `0` on a successful execution or return non-zero otherwise.
