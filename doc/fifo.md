## FIFO library

A local FIFO library for Windows and Unix.

A FIFO is an interprocess communication tool that let different threads and processes write to and read from a common place. The intended use of this implementation is local.

When a FIFO is created, any other thread or process can open a connection to that FIFO and communicate with the rest.

FIFO name style in Unix are '/folder_1/folder_2/fifoName', and are created like a regular file in the folder indicated. On the other hand, FIFOs in Windows have a "network style" paths that start like '\\.\pipe\fifoName' in local FIFO or pipe connections, and '\\serverName\pipe\fifoName' in remote FIFO or pipe connections.

In this library, if you use only a name (recommended way), it will automatically format the path name, but if you indicate a Unix or Windows valid path (in their respective target OS), it will accept it.

---

### Types

- `Fifo`
	
	FIFO type used by the library.


- `FifoDataType`
	
	Enumeration used to set the type of data used in a new FIFO.
	The values of this enumeration are:

	- `FIFO_BYTE_DATA`: Binary-oriented data. Recommended in transmission of known sized values. For
		example, integer values.

	- `FIFO_MSG_DATA`: Message or text oriented data. Recommended in transmission of abitrary sized
		values (for example, text strings).

---

### Functions

- `Fifo createFifo(char const* fifoName, const enum FifoDataType dataType, const bool nonBlocking)`

	Creates a FIFO with the name `fifoName` and with the given data type. When the `nonBlocking` argument is `true` there won't be any kind of waiting when reading or writing in this FIFO, it will just return zero bytes read or written, and this affects this connection and any other connection to this FIFO.

	Returns a `Fifo` value when the execution is successful or returns `NULL` otherwise. 

	&nbsp;

- `Fifo openFifo(char const* fifoName, const bool readOnly)`

	Open an already created FIFO.

	If `readOnly` is `true`, that FIFO will be read only from the returning FIFO connection of this function, the other connections to that FIFO won't change.

	Returns a `Fifo` value when the execution is successful or returns `NULL` otherwise.  

	&nbsp;

- `void destroyFifo(Fifo* fifo):`

	Destroys the given `Fifo` value. If the FIFO was created, it closes the connection to the clients and destroys the FIFO, if the FIFO was just a open connection, closes the connection.

	`fifo` value is set to NULL at the end of the execution.

	&nbsp;

- `int readFifo(const Fifo fifo, char *buffer, const size_t bufferSize)`

	Read from `fifo` a maximum of `bufferSize` bytes. If the read is byte oriented, it would retry
	to read if the amount of bytes read are less that `bufferSize'.

	Returns the number of bytes read in successful executions and returns `-1` when there is any
	error.


	&nbsp;

- `int writeFifo(const Fifo fifo, char const *data, const size_t size)`

	Writes to `fifo` a `bufferSize` bytes.

	Returns the number of bytes read in successful executions and returns `-1` when there is any
	error.
