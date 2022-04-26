// utils.c
// José Miguel Rodríguez Marchena (@josemirm)

#include "utils.h"

char* newString(char const *value, const size_t size) {
	if (size < 1 || value == NULL) {
		return NULL;
	}

	char *ret = (char*) malloc(sizeof(char)*(size + 1));
	if (ret == NULL) {
		return NULL;
	}

	memcpy(ret, value, size);
	ret[size] = '\0';
	return ret;
}


// It could have used "strdup", but it's deprecated in Windows (even if it's correct
// in POSIX OSes), so this prevent a future dependency problem.
char *duplicateString(char const *str) {
	if (NULL == str) {
		return NULL;
	}

	size_t size = strlen(str) + 1;
	char* ret = (char*) malloc(size);
	if (NULL == ret) {
		return NULL;
	}

	memcpy(ret, str, size);
	return ret;
}


int equalString(char const *str1, char const *str2) {
	if (NULL == str1 || NULL == str2) {
		return 0;
	}

	size_t len1 = strlen(str1);
	size_t len2 = strlen(str2);

	if (len1 != len2) {
		return 0;
	}

	for (size_t i=0; i<len1; ++i) {
		if (str1[i] != str2[i]) {
			return 0;
		}
	}

	return 1;
}

int equalString_ci(char const *str1, char const *str2) {
	if (NULL == str1 || NULL == str2) {
		return 0;
	}

	size_t len1 = strlen(str1);
	size_t len2 = strlen(str2);

	if (len1 != len2) {
		return 0;
	}

	for (size_t i=0; i<len1; ++i) {
		if (tolower(str1[i]) != tolower(str2[i])) {
			return 0;
		}
	}

	return 1;
}


int startsWith(char const *str, char const *prefix) {
	if (NULL == str || NULL == prefix) {
		return 0;
	}

	size_t len1 = strlen(str);
	size_t len2 = strlen(prefix);

	if (len1 < len2) {
		return 0;
	}

	for (size_t i=0; i<len2; ++i) {
		if (str[i] != prefix[i]) {
			return 0;
		}
	}

	return 1;
}

// Case insensitive version
int startsWith_ci(char const *str, char const *prefix) {
	if (NULL == str || NULL == prefix) {
		return 0;
	}

	size_t len1 = strlen(str);
	size_t len2 = strlen(prefix);

	if (len1 < len2) {
		return 0;
	}

	for (size_t i=0; i<len2; ++i) {
		if (tolower(str[i]) != tolower(prefix[i])) {
			return 0;
		}
	}

	return 1;
}
