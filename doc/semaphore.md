## Semaphore library

An unnamed semaphore library for Windows and Unix.

A semaphore is a synchronization tool that saves a positive integer value. When the value is zero and the threads are waiting to be non-zero, the threads will be waiting efficiently (will be using 0% of the CPU) until the semaphore change value.

---

### Types

- `Semaphore`

	Semaphore type used by the library.
	
---

### Examples

Example of `createUnnamedSemaphore()`:
```c
	Semaphore mySem;

	if ( createUnnamedSemaphore(&mySem, 3) ) {
		printf("Error creating semaphore\n");
	}
```

&nbsp;


Example of `destroySemaphore()`:
```c
destroySemaphore(&mySem);
```

---

### Functions

- `int createUnnamedSemaphore(Semaphore *sem, const int maxValue)`

	Creates an unnamed semaphore, with a maximum value and a initial value of `maxValue`, and saves it in `*sem`.

	Returns `0` if is executed successfully, returns non-zero otherwise.

	&nbsp;


- `void destroySemaphore(Semaphore *sem)`

	Destroys a semaphore and set `*sem` to NULL.

	&nbsp;


- `int incrementSemaphore(Semaphore sem)`

	Increment by one the internal value of the semaphore.

	Returns `0` if is done successfully and non-zero otherwise.
	
	&nbsp;


- `int waitSemaphore(Semaphore sem)`

	Waits blocking the thread efficiently until the semaphore has a non-zero value. When the semaphore has a value greater than zero, it decrement it and continues the execution.

	Returns `0` if is done successfully and non-zero otherwise.
	
	&nbsp;


- `int tryWaitSemaphore(Semaphore sem)`

	Test if the semaphore value is greater than zero.

	Returns `0` if the semaphore greater than zero and returns non-zero otherwise. It may seems contradictory, but it keeps the "returns zero if execution is good, returns non-zero if execution had problems" logic.

