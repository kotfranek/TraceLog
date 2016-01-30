#ifndef LOGDEFINES_H
#define LOGDEFINES_H

#include <cstddef>
#include <stdint.h>

namespace trace
{
    /**
     * Possible Log Levels. 
     * The lower the number the higher priority it has.
     */
    enum LogLevel
    {                     
        LogLevel_Assert,
        LogLevel_Fatal,
        LogLevel_Error,
        LogLevel_Warning,
        LogLevel_Info,
        LogLevel_Debug,
        LogLevel_Developer,          
        LogLevel_Internal
    };
    
    /* Maximum log entry length */
    static const size_t LOG_MESSAGE_SIZE_MAX = 255U; 

    /* Log cache size */
    static const size_t LOG_CACHE_SIZE = 64U; 
    
    
    static const struct TraceVersion
    {
        uint8_t MAJOR = 0;
        uint16_t MINOR = 1;
        uint16_t BUILD = 1;
    } LOG_VERSION_INFO;
};

#endif