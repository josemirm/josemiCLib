// José Miguel Rodríguez Marchena (@josemirm)

#include "winfifo.h"

#ifdef __WIN_PLATFORM__

	static char const WIN_FIFO_PREFIX[] = "\\\\.\\pipe\\";

	bool isValidWinFifoName(char const *name) {
		if (NULL == name) {
			return false;
		}

		if (!startsWith(name, WIN_FIFO_PREFIX)) {
			return false;
		}

		return true;
	}


	char* newWinFifoName(char const *name) {
		if (NULL == name) {
			printErrorFileLine("newWinFifoName: name is NULL");
			return NULL;
		}

		char *ret;
		size_t len;

		if (!isValidWinFifoName(name)) {
			size_t prefLen = strlen(WIN_FIFO_PREFIX);
			len = prefLen + strlen(name) + 1;
			ret = malloc(len);
			if (NULL == ret) {
				printError("newWinFifoName: Error allocating memory\n");
				return NULL;
			}

			memcpy(ret, WIN_FIFO_PREFIX, strlen(WIN_FIFO_PREFIX));
			memcpy(ret + prefLen, name, strlen(name));
		} else {
			len = strlen(name) + 1;
			ret = malloc(len);
			if (NULL == ret) {
				printError("newWinFifoName: Error allocating memory\n");
				return NULL;
			}

			memcpy(ret, name, len-1);
		}

		ret[len-1] = '\0';

		return ret;
	}


    // In Windows, all pipes should be named like "\\\\.\\pipe\\your_pipe_name".
    HANDLE createWinFifo(char const *name, const enum WinFifoDirection direction,
			const enum WinFifoDataType type, const enum WinFifoReadMode readMode,
			const enum WinFifoWaitMode waitMode) {
        if (NULL == name) {
			printErrorFileLine("createWinFifo: name is NULL");
			return NULL;
		}
		
		HANDLE ret = CreateNamedPipeA(
			name,
			direction | FILE_FLAG_FIRST_PIPE_INSTANCE,
			type | readMode | waitMode,
			PIPE_UNLIMITED_INSTANCES,
			FIFO_BUFFER_SIZE,
			FIFO_BUFFER_SIZE,
			FIFO_TIMEOUT_MS,
			NULL
		);

		if (INVALID_HANDLE_VALUE == ret) {
			printWinError("createWinFifo: CreateNamedPipeA failed");
			return NULL;
		}

		return ret;
    }


	void destroyWinFifo(const HANDLE fifo) {
		if (NULL == fifo) {
			printErrorFileLine("destroyWinFifo: fifo is NULL");
			return;
		}

		if (INVALID_HANDLE_VALUE != fifo) {
			// Wait clients to end to read the pipe
			if (0 == FlushFileBuffers(fifo)) {
				printError("destroyWinFifo: Error flushing the FIFO\n");
			}

			// Disconects clients from pipe
			if (0 == DisconnectNamedPipe(fifo)) {
				printError("destroyWinFifo: Error disconnecting clients from the FIFO\n");
			}

			// Closes the pipe
			if (0 == CloseHandle(fifo)) {
				printError("destroyWinFifo: Error closing the FIFO\n");
			}
		}
	}

	void closeWinFifo(const HANDLE fifo) {
		if (NULL == fifo) {
			printErrorFileLine("closeWinFifo: fifo is NULL");
			return;
		}

		if (INVALID_HANDLE_VALUE != fifo) {
			// Closes the pipe
			if (0 == CloseHandle(fifo)) {
				printError("closeWinFifo: Error closing the FIFO\n");
			}
		}
	}

	HANDLE openWinFifo(char const *name, const bool readOnly) {
		if (NULL == name) {
			printErrorFileLine("openWinFifo: name is NULL");
			return NULL;
		}

		// Waits to other process to create the pipe
		if (0 == WaitNamedPipeA(name, FIFO_TIMEOUT_MS)) {
			printWinError("openWinFifo: Error waiting the named pipe");
			return NULL;
		}
		
		DWORD access = GENERIC_READ;
		DWORD shareMode = FILE_SHARE_READ;

		if (!readOnly) {
			access |= GENERIC_WRITE;
			shareMode |= FILE_SHARE_WRITE;
		}

		// Open the pipe
		HANDLE ret = CreateFileA(name, access, shareMode, NULL, OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL, NULL);

		// Checks if there is any error
		if (INVALID_HANDLE_VALUE == ret) {
			printWinError("openWinFifo: Error opening the named pipe");
			return NULL;
		}

		return ret;
	}


	int readWinFifo(const HANDLE fifo, char *buffer, const size_t bufferSize,
			const enum WinFifoDataType dataType) {
		if (NULL == fifo) {
			printError("readWinFifo: fifo is NULL");
			return -1;
		}
		
		if (NULL == buffer) {
			printError("readWinFifo: buffer is NULL");
			return -1;
		}	

		DWORD bytesRead = 0;
		BOOL ret = FALSE;

		do {
			ret = ReadFile(fifo, buffer, (DWORD) bufferSize, &bytesRead, NULL);
		} while (FALSE == ret && FIFO_TYPE_BYTE == dataType && ERROR_MORE_DATA == GetLastError());
		

		if (0 == ret && !(ERROR_MORE_DATA == GetLastError())) {
			printWinError("readWinFifo: Error reading the FIFO");
			return -1;
		}

		if (INT_MAX < bytesRead) {
			bytesRead = INT_MAX;
		}

		return ((int) bytesRead);
	}

	int writeWinFifo(const HANDLE fifo, char const *buffer, const size_t bufferSize) {
		if (NULL == fifo) {
			printError("writeWinFifo: fifo is NULL");
			return -1;
		}
		
		if (NULL == buffer) {
			printError("writeWinFifo: buffer is NULL");
			return -1;
		}	

		DWORD bytesWritten = 0;

		while (bytesWritten < bufferSize) {
			DWORD loopBytesWritten = 0;
			DWORD ret = WriteFile(fifo, buffer+bytesWritten, (DWORD) bufferSize-bytesWritten, &loopBytesWritten, NULL);

			bytesWritten += loopBytesWritten;

			if (0 == ret && !(ERROR_IO_PENDING == GetLastError())) {
				printWinError("writeWinFifo: Error writing the FIFO");
				return -1;
			}
		}

		if (INT_MAX < bytesWritten) {
			bytesWritten = INT_MAX;
		}

		return ((int) bytesWritten);
	}

#endif
