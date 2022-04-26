// fifo.h
// José Miguel Rodríguez Marchena (@josemirm)

#pragma once

#include "utils.h"


#ifdef __cplusplus
extern "C" {
#endif

	#ifdef __WIN_PLATFORM__
		#include "windows/winfifo.h"
		typedef HANDLE FifoHandle;
	#endif

	#ifdef __UNIX_PLATFORM__
		#include "unix/unixfifo.h"
		typedef int FifoHandle;
	#endif

	enum FifoIOMode {
		FIFO_READ = 0,
		FIFO_WRITE = 1,
		FIFO_RW = 2
	};

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

	Fifo createFifo(char const* fifoName, const enum FifoDataType dataType,
					const enum FifoIOMode, const bool nonBlocking);
	Fifo openFifo(char const* fifoName, const enum FifoDataType dataType,
				  const enum FifoIOMode mode);
	void destroyFifo(Fifo* fifo);

	int readFifo(const Fifo fifo, char *buffer, const size_t bufferSize);
	int writeFifo(const Fifo fifo, char const *data, const size_t size);

#ifdef __cplusplus
}
#endif
