// fifo.c
// José Miguel Rodríguez Marchena (@josemirm)

#include "fifo.h"

Fifo createFifo(char const* fifoName, const enum FifoDataType dataType,
				const enum FifoIOMode mode, const bool nonBlocking) {
	Fifo fifo = (Fifo) malloc(sizeof(struct Fifo_st));

	if (NULL == fifo) {
		printErrorFileLine("createFifo: Error allocating memory\n");
		return NULL;
	}

	fifo->dataType = dataType;

	#ifdef __WIN_PLATFORM__

		fifo->name = newWinFifoName(fifoName);
		if (NULL == fifo->name) {
			printErrorFileLine("createFifo: Error allocating memory\n");
			free(fifo);
			return NULL;
		}

		// In case of read only...
		enum WinFifoDirection wfd = FIFO_IN_ONLY;

		// Otherwise...
		if (FIFO_WRITE == mode) {
			wfd = FIFO_OUT_ONLY;
		} else if (FIFO_RW == mode) {
			wfd = FIFO_IN_OUT;
		}

		enum WinFifoDataType wfdt;
		enum WinFifoReadMode wfrm;
		enum WinFifoWaitMode wfwm;

		if (FIFO_BYTE_DATA == dataType) {
			wfdt = FIFO_TYPE_BYTE;
			wfrm = FIFO_READ_BYTE;
		} else {
			wfdt = FIFO_TYPE_MESSAGE;
			wfrm = FIFO_READ_MESSAGE;
		}

		if (nonBlocking) {
			wfwm = FIFO_NOWAIT;
		} else {
			wfwm = FIFO_WAIT;
		}

		fifo->handle = createWinFifo(fifo->name, wfd, wfdt, wfrm, wfwm);
		if (NULL == fifo->handle) {
			free(fifo->name);
			free(fifo);
			return NULL;
		}

	#endif

	#ifdef __UNIX_PLATFORM__
		fifo->name = newString(fifoName, strlen(fifoName));
		if (NULL == fifo->name) {
			printErrorFileLine("createFifo: Error allocating memory\n");
			free(fifo);
			return NULL;
		}

		fifo->handle = createUnixFifo(fifo->name);

		if (fifo->handle < 0) {
			free(fifo->name);
			free(fifo);
			return NULL;
		}
	#endif

	fifo->created = true;

	return fifo;
}


Fifo openFifo(char const* fifoName, const enum FifoDataType dataType,
			  const enum FifoIOMode mode) {
	Fifo fifo = (Fifo) malloc(sizeof(struct Fifo_st));

	if (NULL == fifo) {
		printErrorFileLine("openFifo: Error allocating memory\n");
		return NULL;
	}

	fifo->dataType = dataType;

	#ifdef __WIN_PLATFORM__

		fifo->name = newWinFifoName(fifoName);
		if (NULL == fifo->name) {
			free(fifo);
			return NULL;
		}

		// In case of read only...
		enum WinFifoDirection wfd = FIFO_IN_ONLY;

		// Otherwise...
		if (FIFO_WRITE == mode) {
			wfd = FIFO_OUT_ONLY;
		} else if (FIFO_RW == mode) {
			wfd = FIFO_IN_OUT;
		}

		fifo->handle = openWinFifo(fifo->name, wfd);
		if (NULL == fifo->handle) {
			free(fifo->name);
			free(fifo);
			return NULL;
		}

	#endif

	#ifdef __UNIX_PLATFORM__

		fifo->name = newString(fifoName, strlen(fifoName));
		if (NULL == fifo->name) {
			printErrorFileLine("openFifo: Error allocating memory\n");
			free(fifo);
			return NULL;
		}

		bool readOnly = (mode == FIFO_READ);
		fifo->handle = openUnixFifo(fifo->name, readOnly);
		if (fifo->handle < 0) {
			free(fifo->name);
			free(fifo);
			return NULL;
		}

	#endif

	fifo->created = false;

	return fifo;
}


void destroyFifo(Fifo *fifo) {
	if (NULL == *fifo || NULL == *fifo) {
		return;
	}

	#ifdef __WIN_PLATFORM__
		if ((*fifo)->created) {
			destroyWinFifo((*fifo)->handle);
		} else {
			closeWinFifo((*fifo)->handle);
		}
	#endif

	#ifdef __UNIX_PLATFORM__
		if ((*fifo)->created) {
			destroyUnixFifo((*fifo)->name, (*fifo)->handle);
		} else {
			closeUnixFifo((*fifo)->handle);
		}
	#endif

	free((*fifo)->name);
	free(*fifo);
	*fifo = NULL;
}


int readFifo(const Fifo fifo, char *buffer, const size_t bufferSize) {
	#ifdef __WIN_PLATFORM__
		enum WinFifoDataType wfdt;

		if (FIFO_BYTE_DATA == fifo->dataType) {
			wfdt = FIFO_TYPE_BYTE;
		} else {
			wfdt = FIFO_TYPE_MESSAGE;
		}

		return readWinFifo(fifo->handle, buffer, bufferSize, wfdt);
	#endif

	#ifdef __UNIX_PLATFORM__
		bool byteOriented = false;

		if (FIFO_BYTE_DATA == fifo->dataType) {
			byteOriented = true;
		}

		return readUnixFifo(fifo->handle, buffer, bufferSize, byteOriented);
	#endif

	return -1;
}

int writeFifo(const Fifo fifo, char const *data, const size_t size) {
	#ifdef __WIN_PLATFORM__
		return writeWinFifo(fifo->handle, data, size);
	#endif

	#ifdef __UNIX_PLATFORM__
		return writeUnixFifo(fifo->handle, data, size);
	#endif

	return -1;
}
