// winfifo.h
// José Miguel Rodríguez Marchena (@josemirm)

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    #include "../utils.h"
    #include <stdbool.h>
    #include <limits.h>

    #ifdef __WIN_PLATFORM__
        #include "winutils.h"
        #include <Windows.h>
        

        #define WINFIFO_BUFFER_SIZE 128
        #define WINFIFO_TIMEOUT_MS 1000


        enum WinFifoDirection {
            FIFO_OUT_ONLY = PIPE_ACCESS_OUTBOUND,
            FIFO_IN_ONLY = PIPE_ACCESS_INBOUND,
            FIFO_IN_OUT = PIPE_ACCESS_DUPLEX
        };

        enum WinFifoDataType {
            FIFO_TYPE_BYTE = PIPE_TYPE_BYTE,
            FIFO_TYPE_MESSAGE = PIPE_TYPE_MESSAGE
        };

        enum WinFifoReadMode {
            FIFO_READ_BYTE = PIPE_READMODE_BYTE,
            FIFO_READ_MESSAGE = PIPE_READMODE_MESSAGE
        };

        enum WinFifoWaitMode {
            FIFO_WAIT = PIPE_WAIT,
            FIFO_NOWAIT = PIPE_NOWAIT
        };


        bool isValidWinFifoName(char const *name);
        char *newWinFifoName(char const *name);

        // In Windows, all pipes should be named like "\\\\.\\pipe\\your_pipe_name".
        HANDLE createWinFifo(char const *name, const enum WinFifoDirection direction,
            const enum WinFifoDataType type, const enum WinFifoReadMode readMode,
            const enum WinFifoWaitMode waitMode);
        void destroyWinFifo(const HANDLE fifo);
        
        HANDLE openWinFifo(char const *name, const bool readOnly);
        void closeWinFifo(HANDLE fifo);


        int readWinFifo(const HANDLE fifo, char *buffer, const size_t bufferSize,
            const enum WinFifoDataType dataType);
        int writeWinFifo(const HANDLE fifo, char const *buffer, const size_t bufferSize);


    #endif

#ifdef __cplusplus
}
#endif
