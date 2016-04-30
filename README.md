# TraceLog
Lightweight Logging/Tracing utility with various output Backends

## Features
- easy integration
- various output Backends:
 - file
 - console
 - UDP Server
- thread-safe implementation
- C printf-like syntax
- no dynamic memory allocation
- negligible performance impact
- asynchronous to the caller thread
- milliseconds based timestamping
- single macro to disable all TraceLog calls in production
- ready for custom output Backend
- assertion mechanism with file name and line number
- buffering in case of slow Backend or high load
- command-line UDP client and file viewer available, check: [TraceLogViewer][11]
- Java-based graphical interface UDP log-viewer available soon

## Building project
### Dependencies
- Requires the [ESys][10] framework installed.
For details refer to ESys instructions.
- Requires the [CMake][20] installed. Minimum version required is 3.0.0.

### Building

In the project root directory execute the shell script configure.sh with root
privileges, if you intend to install the binary output and headers:
```
sudo ./configure.sh
```
This script will create a new output directory, named *build*,
to enable an _out-of-place_ build.

Two static libraries are created: *libtl.a* and *libtlcommon.a*.

- **libtlcommon** contains code for handling the LogEntry objects (serialize and
	deserialize)
- **libtl** contains the logger implementation, including all standard
BackEnds

If you choose to install the project, then the binaries, headers and example code will
be copied to default directory, which, on Linux systems is normally _/usr/local/_.

|Location|Content|
|---|---|
|/usr/local/bin|demo executables|
|/usr/local/lib|libraries|
|/usr/local/include/TraceLog|headers|

## Usage
### Integration into user code
All Tracelog operations are done using the macro statements.

#### Enabling and disabling the logger
The TraceLog is, by default, disabled. The statements produce no code.

To enable the logger a special macro has to be defined. It is preferred to use
the global macro definitions, i.e. using compiler directives (-D).
```
-DTRACELOG_ENABLE
```
Disabling the 'TRACELOG_ENABLE' macro will cause _no logger-call code_ being
compiled and all macros will have no effect.

#### Required headers
Only one header file has to be included. It contains all required TraceLog
definitions and macros.
```cpp
#include "trace/log.h"
```
#### Selecting a Backend
Before TraceLog can be used a Backend has to be selected. Backend is responsible
for storing or printing of the messages.

A Backend selection is possible **only** before first message was logged.

There are 3 available Backends: console, file and UDP server. A Backend can be
selected using one of the macros:

|Backend|Macro|Description|
|---|---|---|
|Console|LOGGER_INIT_BE_CONSOLE|Standard output, all messages are written to the console|
|File|LOGGER_INIT_BE_FILE|Output messages to a file|
|UDP|LOGGER_INIT_BE_UDP|Output messages using lightweight UDP Server|
|Default|LOGGER_INIT_BE_DEFAULT|Default Backend: same as console|

```cpp
/* Set the UDP Server as a backend */
LOGGER_INIT_BE_UDP();

/* This message will be sent using UDP */
LOG_INFO_C( "Using UDP Backend" );

/* Shutdown the logger. No more messages will be accepted.
 * This call is not obligatory, as the logger will shut down on process exit.
 */
LOGGER_SHUTDOWN();

/* This message will not be logged! */
LOG_WARN_C( "Oops! This one does not work!" );
```

#### Logging a message
<!--The TraceLog library does not require any particular initialization, if you do
not intend to use any other backend than console.
It can be used immediately by invoking one of the LOG macros.--> For every log
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
Beside the standard messages the TraceLog supports also Assertions.
#### Assertions
Assertions are statements, that are always expected to be true.

TraceLog provides a LOG_ASSERT macro to log a failed statement and execute an
assertion handler. <BR/>
By default the assertion handler will stop the program execution.
```cpp
// This shall never return a 0!
const int32_t div = calculateDivisor();
LOG_ASSERT( div != 0, "Divisor calculation" );
const int32_t result = 50/div;
```

### Linking
Link your binaries with the *tracelog* and *esys* static libraries.

**Remark**: For the posix builds you have to link the executable with the *-pthread* flag.
```
g++ example/main.o -llibtl -lesys -pthread -o log-demo
```

## Todo
- file i/o-error handling
- custom assertion handler

Copyright &copy; 2016 [Przemysław Podwapiński][98].<br>
Distributed under the [Simplified BSD License][99].

[10]:https://github.com/kotfranek/ESys
[11]:https://github.com/kotfranek/TraceLogViewer
[20]:https://cmake.org/
[98]:mailto:p.podwapinski@gmail.com
[99]:https://www.freebsd.org/copyright/freebsd-license.html
