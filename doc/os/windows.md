## Windows specific utility library

This is available in `include/winutils.h` and `src/winutils.c`

- `printWinError(msg)`

	Macro that print to stderr any Windows API related error.

	Returns number of bytes written to stderr or a negative number if there is any kind of error. This is due to the use of `fprintf()`.

	This macro is NOT thread safe.

	&nbsp;


- `char const *get_windows_last_error(void)`

	Get a formated string with the last error using the Windows API. The returned string is a statically declared array or char, so there isn't any need to free that memory

	This function is NOT thread safe.

	&nbsp;


- `void setSecAttribInherit(SECURITY_ATTRIBUTES *sa, BOOL inheritHandle)`

	Set the contents of `*sa` with the given `inheritHandle` value and a null security descriptor.

	&nbsp;

---


## Windows specific FIFO library

This is available in `include/winfifo.h` and `src/winfifo.c`

All local pipes and FIFOs in Windows should start with `\\.\pipe\` to function properly, and some of the functions would check this and format the pipe names if is needed.

For more information about pipes and FIFOs names, check the Microsoft documentation

&nbsp;

### Enumerations

`WinFifoDirection`

Read and write configuration

- `FIFO_OUT_ONLY`: Write-only

- `FIFO_IN_ONLY`:  Read-only

- `FIFO_IN_OUT`:   Read and write

&nbsp;


`WinFifoDataType`

Type of data used

- `FIFO_TYPE_BYTE`: Binary oriented data. To use with known data size reads, like integers or structures.

- `FIFO_TYPE_MESSAGE`: Message oriented data. To use when there could be unknown data size in reads, like variable length strings.

&nbsp;


`WinFifoReadMode`

Read mode

- `FIFO_READ_BYTE`: Read data as bytes.

- `FIFO_READ_MESSAGE`: Read data as a message.

&nbsp;


`WinFifoWaitMode`

When a FIFO connection end waits means that it will block the execution of its process/thread until the other side is done reading or writing, and also waits when there are nothing connected to the other end. This select to use that configuration or not.

Default mode should be `FIFO_WAIT`


- `FIFO_WAIT`: Wait the other side to end their transmission.

- `FIFO_NOWAIT`: Don't wait the other side.

&nbsp;

---

### Functions

```c
bool isValidWinFifoName(char const *name)
```

A valid Windows FIFO name is a pipe path that starts with `\\.\pipe\`, and this function only checks that.

Returns `true` if the path is valid or returns `false` otherwise.

&nbsp;


```c
char *newWinFifoName(char const *name)
```

Check if the path is valid with `isValidWinFifoName()`. If the path is valid, returns that path, if not, then returns a string that starts with `\\.\pipe\` and ends with the contents of `*name`,

Returns a string allocated with `malloc()` in a successfull execution or returns `NULL` on any error.

Those strings returned by this function need to be freed with `free()` after using them.

&nbsp;


```c
HANDLE createWinFifo(char const *name, const enum WinFifoDirection direction,
    const enum WinFifoDataType type, const enum WinFifoReadMode readMode,
    const enum WinFifoWaitMode waitMode)
```

Creates a FIFO with the given arguments. The created FIFO shouldn't be in use previously.

Returns a valid value on a successfull execution or returns `NULL` otherwise.

&nbsp;


```c
void destroyWinFifo(const HANDLE fifo)
```

Close and destroy a previously created FIFO.

This function must be used with FIFOs created by `createWinFifo()`.

&nbsp;


```c
HANDLE openWinFifo(char const *name, const bool readOnly)
```

Opens a previously created FIFO.

That FIFO could be any FIFO created by any local process running in the machine, independently if it was created using this library or not.

Returns a valid value on a successfull execution or returns `NULL` otherwise

&nbsp;


```c
void closeWinFifo(HANDLE fifo)
```

Close a previously opened FIFO.

This function must be used with FIFOs opened by `openWinFifo()`.

&nbsp;


```c
int readWinFifo(const HANDLE fifo, char *buffer, const size_t bufferSize,
    const enum WinFifoDataType dataType)
```

Reads from a FIFO and stores the read content in `*buffer`. If the read content is bigger than `bufferSize`, it will truncate it.

Returns the amount of bytes read if the execution is successfull or returns a negative number otherwise.

&nbsp;


```c
int writeWinFifo(const HANDLE fifo, char const *buffer, const size_t bufferSize)
```

Writes the contents of `*buffer` until they reach the amount of `bufferSize` written bytes.

Returns the number of bytes written if the execution is successfull or returns a negative number otherwise.
