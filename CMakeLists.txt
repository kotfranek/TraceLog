# CMake minimum version dependency
cmake_minimum_required( VERSION 3.0.0 FATAL_ERROR )

# Project name
project( "TraceLog" )

# Compiler and linker flags
set( CPP_COMPILE_FLAGS "-g -Wall -std=c++11" )
set( CPP_LINK_FLAGS "-pthread" )

# Header files locations
include_directories(
    include
    "/usr/local/include/ESys"
)


add_library( tl-entry OBJECT
    src/trace/entry/Payload.cpp
    src/trace/entry/LogEntry.cpp
    src/trace/entry/TraceLevelInfo.cpp
    src/trace/entry/PayloadHelper.cpp
)

add_library( tlcommon STATIC
    $<TARGET_OBJECTS:tl-entry>
)


# Main project library
add_library( tl STATIC
    $<TARGET_OBJECTS:tl-entry>
    src/trace/model/LogPersistThread.cpp
    src/trace/model/LogBuffer.cpp
    src/trace/model/LogSharedContainer.cpp
    src/trace/Logger.cpp
    src/trace/log.cpp
    src/trace/ConsoleBackEnd.cpp
    src/trace/UdpBackEnd.cpp
    src/trace/FileBackEnd.cpp
    src/trace/UdpClientMediator.cpp
    src/trace/config/TraceConfig.cpp
)

# Demo application: sources
add_executable( tl-demo
    example/main.cpp
)

# Demo application: compiler definitions
target_compile_definitions( tl-demo
    PUBLIC TRACELOG_ENABLED
)

# Demo application: required libraries
target_link_libraries( tl-demo
    tl
    esys
)

# Set the compiler and linker flags for particular targets
set_target_properties( tl PROPERTIES COMPILE_FLAGS ${CPP_COMPILE_FLAGS} )
set_target_properties( tl-entry PROPERTIES COMPILE_FLAGS ${CPP_COMPILE_FLAGS} )
set_target_properties( tl-demo PROPERTIES COMPILE_FLAGS ${CPP_COMPILE_FLAGS} )
set_target_properties( tl-demo PROPERTIES LINK_FLAGS ${CPP_LINK_FLAGS} )

# Target Install
install( TARGETS tl-demo DESTINATION bin )
install( TARGETS tl DESTINATION lib )
install( TARGETS tlcommon DESTINATION lib )
install( DIRECTORY 
    include/ DESTINATION include/${CMAKE_PROJECT_NAME}
    FILES_MATCHING PATTERN "*.h" 
)

