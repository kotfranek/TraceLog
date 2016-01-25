#ifndef LOG_H_INTERFACE
#define LOG_H_INTERFACE

#include "log/Logger.h"

namespace log
{
    enum LogBackend
    {
        LogBackend_Console,
        LogBackend_File,
        LogBackend_Default = LogBackend_Console,    
    };
    
    /**
     * Retrieve the Logger Instance reference
     */
    Logger& logInstance();
    
    /**
     * Get the desired LogBackend instance
     * @arg backEnd Backend Implementation
     * @result LogBackEnd instance reference
     * @remark Return default in case of wrong argument
     */
    ILogBackEnd& logBackend( const LogBackend backEnd );
};

/* Globally available Logger instance */
#define LOGGER_INSTANCE ::log::logInstance()

/* Backend selection. Backend has to be a global object */
#define LOGGER_SET_BACKEND( backend ) LOGGER_INSTANCE.setBackEnd( backend )

#define LOGGER_INIT_BE_CONSOLE LOGGER_SET_BACKEND( &::log::logBackend( ::log::LogBackend_Console ) )

#define LOGGER_INIT_BE_FILE LOGGER_SET_BACKEND( &::log::logBackend( ::log::LogBackend_File ) )

#define LOGGER_INIT_BE_DEFAULT LOGGER_INIT_BE_CONSOLE

/* Neat log close */
#define LOGGER_SHUTDOWN LOGGER_INSTANCE.shutDown

#define LOG_INFO_C( msg, ... ) LOGGER_INSTANCE.logV( ::log::LogLevel_Info, msg, ##__VA_ARGS__ )
#define LOG_DEBUG_C( msg, ... ) LOGGER_INSTANCE.logV( ::log::LogLevel_Debug, msg, ##__VA_ARGS__ )
#define LOG_WARN_C( msg, ... ) LOGGER_INSTANCE.logV( ::log::LogLevel_Warning, msg, ##__VA_ARGS__ )
#define LOG_ERR_C( msg, ... ) LOGGER_INSTANCE.logV( ::log::LogLevel_Error, msg, ##__VA_ARGS__ )
#define LOG_DEV_C( msg, ... ) LOGGER_INSTANCE.logV( ::log::LogLevel_Developer, msg, ##__VA_ARGS__ )

/* in case of a negative condition result log and invoke registered AbortHandler */
#define LOG_ASSERT( condition, msg ) if ( !(condition) ) LOGGER_INSTANCE.assert( __FILE__, __LINE__, msg )

#endif // LOG_H_INTERFACE