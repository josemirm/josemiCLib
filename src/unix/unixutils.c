#include "unixutils.h"

#ifdef __UNIX_PLATFORM__

ssize_t write_n(const int fd, const void* buffer, const size_t count) {
	const char* cbuffer = (char*)buffer;
	size_t totalWritten = 0;

	do {
		ssize_t written = write(fd, (cbuffer + totalWritten), (count - totalWritten));

		if (-1 < written) {
			// if written >= 0
			totalWritten += written;
		}
		else {
			// if Written == -1 some error happened 

			if (errno == EINTR) {
				// If the process was interrupted in the middle of the writting, ir should continue
				written = 0;
				continue;
			}
			else {
				return -1;
			} // if (errno == EINTR)
		} // if (-1 < written)

	} while (count > totalWritten);

	return totalWritten;
}

ssize_t read_n(const int fd, void* buffer, const size_t count) {
	char* cbuffer = (char*)buffer;
	size_t totalRead = 0;

	do {
		ssize_t readBytes = read(fd, (cbuffer + totalRead), (count - totalRead));

		if (0 == readBytes) {
			// if there isn't any byte to read, just stops reading
			cbuffer[totalRead] = '\0';
			return 0;
		}

		if (-1 < readBytes) {
			// if read >= 0
			totalRead += readBytes;
		}
		else {
			// if read == -1 some error happened 

			if (errno == EINTR) {
				// If the process was interrupted in the middle of the reading, ir should continue
				readBytes = 0;
				continue;
			}
			else {
				return -1;
			} // if (errno == EINTR)
		} // if (-1 < read)

	} while (count > totalRead);

	return totalRead;
}

#endif