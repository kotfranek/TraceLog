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

## Dependencies
Requires the [ESys][10] framework.<br>
Set the ESYS_HOME environment variable before calling the make.

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
