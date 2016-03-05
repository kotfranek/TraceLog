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

- libtracecommon contains code for handling the LogEntry objects (serialize and
	deserialize)
- libtracelog contains the logger implementation, including all standard
BackEnds

## Usage
### Code
Only one header file has to be included. It contains all definitions and macros.
```cpp
#include "trace/log.h"
```
#### Logging a single message
The TraceLog library does not require any particular initialization, if you do
not intend to use any other backend than console.
It can be used immediately by invoking one of the LOG macros. For every log
severity level there is a separate macro:
```cpp
LOG_INFO_C( "Information" );
LOG_DEBUG_C( "Debug: auxiliary development information" );
LOG_WARN_C( "Warning: a handled exception" );
LOG_ERR_C( "Error: unhandled exception" );
```
C printf-like syntax with multiple arguments is supported:
```cpp
LOG_ERR_C( "Error id %d:, reason: %s", errorId, errorReason );
```

#### Selecting a Backend
A backend selection is possible **only** before first message was logged.

There are 3 available Backends: console, file and UDP server. A Backend can be selected using one of the macros:
```cpp
/* Set the UDP Server as a backend */
LOGGER_INIT_BE_UDP;

/* This message will be sent using UDP */
LOG_INFO_C( "Using UDP Backend" );

/* Shutdown the logger. No more messages will be accepted.
 * This call is not obligatory, as the logger will shut down on process exit.
 */
LOGGER_SHUTDOWN;

/* This message will not be logged! */
LOG_WARN_C( "Oops! This one does not work!" );
```

### Linking
Link your binaries with the *tracelog* and *esys* static libraries. For the posix systems you have to link the executable with the *-pthread* flag.
```
g++ example/main.o -llibtracelog -lesys -pthread -o log-demo
```

## Todo
- file i/o-error handling
- custom assertion handler

Copyright &copy; 2016 [Przemysław Podwapiński][98].<br>
Distributed under the [Simplified BSD License][99].

[10]:https://github.com/kotfranek/ESys
[98]:mailto:p.podwapinski@gmail.com
[99]:https://www.freebsd.org/copyright/freebsd-license.html
