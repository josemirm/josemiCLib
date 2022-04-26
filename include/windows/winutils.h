// winutils.h
// José Miguel Rodríguez Marchena (@josemirm)

#pragma once

#include "../utils.h"


#ifdef __cplusplus
extern "C" {
#endif


    #ifdef __WIN_PLATFORM__

    #include <stdio.h>
    #include <windows.h>

    #define printWinError(msg) fprintf(stderr, "%s: %s\n", msg, get_windows_last_error())

    #define JOSEMI_WIN_UTILS_ERROR_MSG_BUF_LEN 256
    char const *get_windows_last_error(void);
    void setSecAttribInherit(SECURITY_ATTRIBUTES *sa, BOOL inheritHandle);

    #endif


#ifdef __cplusplus
}
#endif