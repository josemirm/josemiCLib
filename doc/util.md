## Common utilities

Basic C functions that can be used in multiple kind of projects.

---

### Unsorted functions


- `unimplemented():`

	A macro function to be used in work-in-progress parts of code. When this is executed, it prints where it was executed and abort the program.

	&nbsp;


- `printError(msg):`

	Prints `msg` to `stderr`. Useful to print info/debug/warning/error messages.
	The `msg` input needs to be a C null-terminated `char*` string to function properly.

	&nbsp;


- `printErrorFileLine(msg) :`

	Same as `printError(msg)`, but also print the file name and line where it was executed.


---

### String functions


- `char* newString(char const *value, const size_t size)`:

	Allocates a new string with the given size, copies the content of `value` to it, make it end with a null terminator (`\0`) if it isn't, and return that value.

	The size of `value` and the `size` argument can be different in any way

	The returned value is allocated with `malloc(..)` and should be freed with `free(...)` by the user. If input values are invalid or there is any error in the process, then returns null.

	&nbsp;


- `char duplicateString(char const *str)`:

	Allocate a new string with the same size and the same content than the input `str`. This is a workaround to `strdup(...)` intended to be use in Windows (due to the deprecation of `strdup()` and possible deletion in some future compiler versions), but it should work with any C compiler in any OS that allow the use dynamic memory functions like `malloc(..)` and `free(...)`.

	The returned value is allocated with `malloc(..)` and should be freed with `free(...)` by the user. If there is any kind of error or the input is null, the returned value is null.
  
	&nbsp;


- `int equalString(char const *str1, char const *str2)`:

	Returns `0` when `*str1` and `*str2` have a different content, and any other value if their content is different. If some of the inputs are null, the returned value is `0`.

	This function doesn't distinguish when `str1` and `str2` are the same pointers or not.

	Example:

	```c
	int a = equalString("ABCD", "ABCD");
	// int a = 1

	int b = equalString("ABCD", "abcd");
	// int b = 0

	int c = equalString("asdf", "qwer");
	// int c = 0
	```

	&nbsp;


- `int startsWith(char const *str, char const *prefix)`:

	Returns `1` when the content of `str` starts with the total content of `prefix` and returns `0` otherwise. This function is case sensitive.

	Examples:
	```c
	int a = startWith("Mark Twain", "Mark T")
	// a = 1

	int b = startWith("Mark Twain", "MARK T")
	// b = 0
	
	int c = startWith("Mark Twain", "Mark Twain was")
	// c = 0
	```

	&nbsp;


- `int equalString_ci(char const *str1, char const *str2)`:

	Case insensitive version of `equalString(...)`.

	Returns `0` when `*str1` and `*str2` have a different content, and any other value if their content is different. If some of the inputs are null, the returned value is `0`.

	This function doesn't distinguish when `str1` and `str2` are the same pointers or not.

	Examples:
	```c
	int a = equalString_ci("QWERTY", "QWERTY")
	// n = 1

	int b = equalString_ci("QWERTY", "QwErTy")
	// b = 1

	int c = equalString_ci("asdf", "qwer")
	// c = 0
	```

	&nbsp;


- `int startsWith_ci(char const *str, char const *prefix)`:

	Case insensitive version of `startWith(...)`

	Returns `1` when the content of `str` starts with the total content of `prefix` and returns `0` otherwise.

	Examples:
	```c
	int a = startWith_ci("Mark Twain", "Mark T")
	// a = 1

	int b = startWith_ci("Mark Twain", "MARK T")
	// b = 1

	int c = startWith_ci("Mark Twain", "Mark Twain was")
	// c = 0
	```
