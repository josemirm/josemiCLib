#include "unixfifo.h"


#ifdef __cplusplus
extern "C" {
#endif


    #ifdef __UNIX_PLATFORM__

    int createUnixFifo(const char *fname) {
        if (NULL == fname) {
            printError("createUnixFifo: fname is NULL");
            return -1;
        }
        
        const int fileMask = 0660;
        int ret = mkfifo(fname, fileMask);

        if (-1 == ret) {
            if (EEXIST == errno) {
                fprintf(stderr, "File '%s' already exists\n", fname);
            } else {
                perror("Error creating the FIFO");
            }
        }

        return ret;
    }


    int openUnixFifo(const char *fname, const bool readOnly) {
        if (NULL == fname) {
            printErrorFileLine("openUnixFifo: fname is NULL");
            return -1;
        }
        
        int ret = -1;
        int mode = O_RDWR;

        if (readOnly) {
            mode = O_RDONLY;
        }

        ret = open(fname, mode);

        if (-1 == ret) {
            perror("Error opening the FIFO");
        }

        return ret;
    }


    void closeUnixFifo(const int fd) {
        if (0 > fd) {
            printError("closeUnixFifo: Invalid file descriptor");
            return;
        }

        close(fd);
    }


    void destroyUnixFifo(char const* fname, const int fd) {
        if (0 > fd) {
            printError("destroyUnixFifo: Invalid file descriptor");
            return;
        }

        if (NULL == fname) {
            printError("destroyUnixFifo: Invalid file name");
            return;
        }

        if (-1 == close(fd)) {
            perror("destroyUnixFifo: Error closing the FIFO");
        }

        if (-1 == unlink(fname)) {
            perror("destroyUnixFifo: Error unlinking the FIFO");
        }
    }


    int readUnixFifo(const int fd, char *buf, const size_t size, const bool isByteOriented) {
        if (0 > fd) {
            printError("readUnixFifo: fd is invalid");
            return -1;
        }

        if (NULL == data) {
            printError("readUnixFifo: data is NULL");
            return -1;
        }
        
        unsigned int bytesRead = 0;
        if (isByteOriented) {
            bytesRead = read_n(fd, buf, size);
        } else {
            bytesRead = read(fd, buf, size);

            if (bytesRead < 0) {
                perror("Error reading from the FIFO");
                return -1;
            }
        }

        return bytesRead;
    }


    int writeUnixFifo(const int fd, char const *data, const size_t size) {
        if (0 > fd) {
            printError("writeUnixFifo: fd is invalid");
            return -1;
        }

        if (NULL == data) {
            printError("writeUnixFifo: data is NULL");
            return -1;
        }
        
        unsigned int bytesWritten = 0;
        bytesWritten = write_n(fd, data, size);

        if (bytesWritten < 0) {
            perror("Error writing to the FIFO");
        }

        return bytesWritten;
    }


    #endif


#ifdef __cplusplus
}
#endif
