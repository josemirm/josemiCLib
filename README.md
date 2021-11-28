# JosemiCLib

A basic C library with very used common functions and wrappers to use common OS functionalities.
The wrappers are OS agnostic between Unix and Windows, and they use OS specific system calls or APIs to achieve that.

### Usage

This library can be used including the `include` and `src` folders in your project and compiling all together.

It could also link a precompiled static library file after using CMake, and a `CMakeLists.txt` file is provided to make this task easier.

### Documentation

All the documentation for the library is available in Markdown format in the doc folder, and those files refers to the .c and .h files in the `src` and `include` folders. For easier access, click the available links below.

- [fifo.md: FIFO interprocess communication library](doc/fifo.md)
- [monitor.md: Monitor library](doc/monitor.md)
- [mutex.md: Mutex librery](doc/mutex.md)
- [semaphore.md: Semaphore library](doc/semaphore.md)
- [threads.md: Thread library](doc/threads.md)
- [util.md: Common utility functions](doc/util.md)


### License

MIT License

Copyright (c) 2021 Josemi Rodríguez

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
