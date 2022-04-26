## Monitor library

A monitor library for Windows and Unix

Monitors are a synchronization tool used to lock threads until some kind of condition is made true. These tools are composed by a mutex and a condition variable.

When a thread use a monitor, this are used to block the thread efficiently if a condition is not achieved. After some change in the part monitored, the thread wakes to check if the condition is achieved and then can continue its execution. Using a monitor requieres at least a thread that enter that waits and other that wakes the blocked one.

---

### Example usage

The same monitor must be used between all the threads using the same shared resource. It should be
created like this:

```c
Monitor mon;

if (createMonitor(&mon)) {
    printf("Error creating monitor\n");
    // do something more
}
```

And should be destroyed simply like this:

```c
destroyMonitor(&mon);
```


When there is any usage of the shared resources and a waiting condition, it must be used `exitMonitor()` after executing `waitAndEnterMonitor()` and any necessary process done in any shared value.

Wake functions (`wakeMonitor()` and `wakeAllMonitor()`) should be executed after changing the shared resource, and that means, after the execution of `exitMonitor()`.


&nbsp; 

- Conditionally blocked thread:

```c
// Monitor mon is created previously

// Check if the condition is achieved, and it isn't, it sleeps until some
// changes are made and the monitor "mon" is signaled to wake the thread.

// After every wake, it will check the condition and it will only continue its
// execution when its condition is made.

// This wait won't have any timeout.
while (someTriggerValue < sharedResource.value) {
    waitAndEnterMonitor(mon, 0);
}

doSomething(sharedResource.Value);

exitMonitor(mon)
```

&nbsp; 

- Waiting thread:
```c
// The monitor blocks the thread until other thread wakes it. If it's not
// awakeded in 5000 milliseconds, it will wake itself after that time.
waitAndEnterMonitor(mon, 5000);

doSomething();

exitMonitor(mon);

```

---

### Types

- `Monitor`

    Monitor type used by the library.

---

### Functions

- `int createMonitor(Monitor *monPtr)`

    Creates a monitor and set the value of `*monPtr` to this new monitor.
    
    Returns `0` in a successful execution or return non-zero otherwise.

    &nbsp;

- `void destroyMonitor(Monitor* monPtr)`

    Destroy the monitor and set the value that `monPtr` points to `NULL`.

    &nbsp;

- `void enterMonitor(Monitor mon)`

    Enters a critical section of the code when the shared resources are accessed.

    &nbsp;


- `void exitMonitor(Monitor mon)`

    Exits a critical section of the code when the shared resources are accessed.

    &nbsp;


- `int waitAndEnterMonitor(Monitor mon)`

    Blocks the thread and waits until other thread wake it using the same monitor.
    It isn't necessary to execute `enterMonitor()` before this function.

    Returns `0` when executes successfully or returns non-zero otherwise.

    &nbsp;


- `int wakeMonitor(Monitor mon)`

    Wake a single thread waiting for changes in the shared resource.

    Returns `0` when executes successfully or returns non-zero otherwise.

    &nbsp;


- `int wakeAllMonitor(Monitor mon)`
    
    Wake a all the threads waiting for changes in the shared resource.

    Returns `0` when executes successfully or returns non-zero otherwise.
