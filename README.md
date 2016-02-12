# TraceLog
Lightweight Logging/Tracing utility

## Features
- easy integration
- thread-safe implementation
- negligible performance impact
- minimized heap operations
- logging asynchronous to the caller thread
- C printf-like syntax
- milliseconds based timestamping
- output to file, console or via UDP
- ready for custom output backend
- support for the assertion mechanism with file name and line number
- Java UDP log-viewer available soon

## Building project
Requires the [ESys][10] framework pre-built and `ESYS_HOME` environment variable
set to directory containing [ESys][10].

Example:

	$ ESYS_HOME="${HOME}/ESys" make

Two static libraries are created: *libtracelog.a* and *libtracecommon.a*.
Both are necessary to use the Logger in own projects.

- libtracecommon contains code for handling the LogEntry objects (serialize and deserialize)
- libtracelog contains the logger implementation, including all standard BackEnds

## Todo
- finish the UDP logger backend
- file i/o-error handling
- process-id dependent output file name
- custom assertion handler
- improve code documentation

Copyright &copy; 2016 [Przemysław Podwapiński][98].<br>
Distributed under the [Simplified BSD License][99].

[10]:https://github.com/kotfranek/ESys
[98]:mailto:p.podwapinski@gmail.com
[99]:https://www.freebsd.org/copyright/freebsd-license.html
