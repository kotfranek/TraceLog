#ifndef LOGDEFINES_H
#define LOGDEFINES_H

#include <cstddef>
#include <stdint.h>

/**
 * Macro Definition TRACE_LOG_OP_SYNC
 * Enable/Disable Syncronous Logging
 * 
 * Setting this macro to 1 enables the simplified Syncronous Operation.
 * This means, that each time the Log is being added it will be directly written 
 * to the backend. The Log message addition time can't be guaranteed as it 
 * depends on the backend performance and it's internal buffering scheme.
 * 
 * Normally Log messages are put into a message queue and later processed by a 
 * Persist Thread, guaranteeing constant execution time while adding new entries.
 */
#define TRACE_LOG_OP_SYNC 0

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
    
    /* Persisting thread auxiliary cache */
    static const size_t LOG_PERSIST_THREAD_CACHE = 48U;
    
    /* Log FileBackend cache size */
    static const size_t LOG_FILE_BACKEND_CACHE_SIZE_BYTES = 4096U;         
};

#endif