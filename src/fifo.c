// José Miguel Rodríguez Marchena (@josemirm)

#include "fifo.h"

Fifo createFifo(char const* fifoName, enum FifoDataType dataType, bool nonBlocking) {
    Fifo fifo = (Fifo) malloc(sizeof(struct Fifo_st));

    if (NULL == fifo) {
        printErrorFileLine("createFifo: Error allocating memory\n");
        return NULL;
    }
    
    #ifdef __WIN_PLATFORM__
    
        fifo->name = newWinFifoName(fifoName);
        if (NULL == fifo->name) {
            printErrorFileLine("createFifo: Error allocating memory\n");
            free(fifo);
            return NULL;
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

        fifo->handle = createWinFifo(fifo->name, FIFO_IN_OUT, wfdt, wfrm, wfwm);
        if (INVALID_HANDLE_VALUE == fifo->handle) {
            free(fifo->name);
            free(fifo);
            return NULL;
        }

    #endif

    #ifdef __LINUX_PLATFORM__
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


Fifo openFifo(char const* fifoName, bool readOnly) {
    Fifo fifo = (Fifo) malloc(sizeof(struct Fifo_st));

    if (NULL == fifo) {
        printErrorFileLine("openFifo: Error allocating memory\n");
        return NULL;
    }

    #ifdef __WIN_PLATFORM__

        fifo->name = newWinFifoName(fifoName);
        if (NULL == fifo->name) {
            free(fifo);
            return NULL;
        }

        fifo->handle = openWinFifo(fifo->name, readOnly);
        if (INVALID_HANDLE_VALUE == fifo->handle) {
            free(fifo->name);
            free(fifo);
            return NULL;
        }

    #endif

    #ifdef __LINUX_PLATFORM__

        fifo->name = newString(fifoName, strlen(fifoName));
        if (NULL == fifo->name) {
            printErrorFileLine("openFifo: Error allocating memory\n");
            free(fifo);
            return NULL;
        }

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


void destroyFifo(const Fifo fifo) {
    if (NULL == fifo) {
        return;
    }

    #ifdef __WIN_PLATFORM__
        if (fifo->created) {
            destroyWinFifo(fifo->handle);
        } else {
            closeWinFifo(fifo->handle);
        }
    #endif

    #ifdef __LINUX_PLATFORM__
        if (fifo->created) {
            destroyUnixFifo(fifo->name, fifo->handle);
        } else {
            closeUnixFifo(fifo->handle);
        }
    #endif

    free(fifo->name);
    free(fifo);
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

    #ifdef __LINUX_PLATFORM__
        bool byteOriented = false;

        if (FIFO_BYTE_DATA == fifo->dataType) {
            byteOriented = true;
        }

        return readUnixFifo(fifo->handle, buffer, bufferSize, byteOriented);
    #endif
}

int writeFifo(const Fifo fifo, char const *data, const size_t size) {
    #ifdef __WIN_PLATFORM__
        return writeWinFifo(fifo->handle, data, size);
    #endif

    #ifdef __LINUX_PLATFORM__
        return writeUnixFifo(fifo->handle, data, size);
    #endif
}
