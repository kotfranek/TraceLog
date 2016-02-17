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
        LogLevel_Internal,
        LogLevel_Unknown
    };
    
    /* Maximum log entry length */
    static const size_t LOG_MESSAGE_SIZE_MAX = 255U; 

    /* Log cache size */
    static const size_t LOG_CACHE_SIZE = 512U; 
    
    /* Log FileBackend cache size */
    static const size_t LOG_FILE_BACKEND_CACHE_SIZE = 96U;         
};

#endif