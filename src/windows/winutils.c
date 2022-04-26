// winutils.c
// José Miguel Rodríguez Marchena (@josemirm)

#include "winutils.h"

#ifdef __WIN_PLATFORM__

// Warning: This function is not thread safe.
char const *get_windows_last_error() {
	static char buffer[JOSEMI_WIN_UTILS_ERROR_MSG_BUF_LEN] = {0};
	DWORD error_code = GetLastError();

	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				buffer, JOSEMI_WIN_UTILS_ERROR_MSG_BUF_LEN, NULL);

	return buffer;
}

#endif


void setSecAttribInherit(SECURITY_ATTRIBUTES *sa, BOOL inheritHandle) {
	sa->nLength = sizeof(SECURITY_ATTRIBUTES);
	sa->bInheritHandle = inheritHandle;
	sa->lpSecurityDescriptor = NULL;
}
