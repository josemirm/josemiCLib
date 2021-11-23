// José Miguel Rodríguez Marchena (@josemirm)

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <ctype.h>

    #if defined(_WIN64) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        #define __WIN_PLATFORM__ 1
    #elif defined(__linux__) || defined(__APPLE__) || defined(__unix__)
        #define __UNIX_PLATFORM__ 1
    #endif

    #define unimplemented() \
        fprintf(stderr, "Unimplemented function in file '%s' at line %i\n", __FILE__, __LINE__); \
        abort()

    #define printError(msg) fprintf(stderr, msg)

    #define printErrorFileLine(msg) \
        fprintf(stderr, "File '%s' at line %i: %s\n", __FILE__, __LINE__, msg)

    char* newString(char const *value, const size_t size);
    char* duplicateString(char const *str);
    int equalString(char const *str1, char const *str2);
    int startsWith(char const *str, char const *prefix);

    // Case insensitive version
    int equalString_ci(char const *str1, char const *str2);
    int startsWith_ci(char const *str, char const *prefix);

#ifdef __cplusplus
}
#endif
