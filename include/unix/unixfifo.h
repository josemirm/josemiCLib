// José Miguel Rodríguez Marchena (@josemirm)

#pragma once

#include "../utils.h"


#ifdef __cplusplus
extern "C" {
#endif


#ifdef __UNIX_PLATFORM__
	#include "unixutils.h"

	#include <stdbool.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <fcntl.h>
	#include <signal.h>

    int createUnixFifo(char const *fname);
	int openUnixFifo(char const *fname, const bool readonly);
	void closeUnixFifo(const int fd);
	void destroyUnixFifo(char const* fname, const int fd);

	int readUnixFifo(const int fd, char *buf, const unsigned int size, const bool isByteOriented);
	int writeUnixFifo(const int fd, char const* data, const unsigned int size);

    typedef int FifoHandle;
#endif


#ifdef __cplusplus
}
#endif