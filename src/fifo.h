// José Miguel Rodríguez Marchena (@josemirm)

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "utils.h"

    #ifdef __WIN_PLATFORM__
        #include "windows/winfifo.h"
    #endif

    #ifdef __UNIX_PLATFORM__
        #include "unix/unixfifo.h"
    #endif

    enum FifoDataType {
        FIFO_BYTE_DATA = 0,
        FIFO_MSG_DATA = 1
    };

    struct Fifo_st {
        FifoHandle handle;
        char* name;
        enum FifoDataType dataType;
        bool created;
    };

    typedef struct Fifo_st* Fifo;

    Fifo createFifo(char const* fifoName, const enum FifoDataType dataType, const bool nonBlocking);
    Fifo openFifo(char const* fifoName, const bool readOnly);
    void destroyFifo(const Fifo fifo);

    int readFifo(const Fifo fifo, char *buffer, const size_t bufferSize);
    int writeFifo(const Fifo fifo, char const *data, const size_t size);

#ifdef __cplusplus
}
#endif
