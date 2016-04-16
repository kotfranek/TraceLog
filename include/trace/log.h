/*
 * Copyright (c) 2016, Przemysław Podwapiński <p.podwapinski@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY Przemysław Podwapiński ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Przemysław Podwapiński BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef LOG_H_INTERFACE
#define LOG_H_INTERFACE

#include "trace/ILogger.h"
#include "trace/config/IConfig.h"

namespace trace
{   
    /**
     * Retrieve the Logger Instance reference
     */
    ILogger& logInstance();
    
    
    /**
     * Get the Logger configuration
     * @return 
     */
    config::IConfig& logConfig();
    
    /**
     * Get the desired LogBackend instance
     * @arg backEnd Backend Implementation
     * @result LogBackEnd instance reference
     * @remark Return default in case of wrong argument
     */
    backend::ILogBackEnd& logBackend( const LogBackend backEnd );
};

#if ( defined ( TRACE_ENABLED ) || defined ( LOG_ENABLED ) )

/* Globally available Logger instance */
#define LOGGER_INSTANCE ::trace::logInstance()

/* Globally available Logger Configuration */
#define LOGGER_CONFIGURATION ::trace::logConfig()

/* Backend selection. Backend has to be a global object */
#define LOGGER_SET_BACKEND( backend ) LOGGER_INSTANCE.setBackEnd( backend )

/* Configuration: Set the UDP Backend port */
#define LOGGER_SET_UDP_PORT( port ) LOGGER_CONFIGURATION.setUdpPort( port )

/* Neat log close */
#define LOGGER_SHUTDOWN() LOGGER_INSTANCE.shutDown()

/* Basic log message macro. Preferably use the level-named macros. */
#define LOG_MSG_C( lvl, msg, ... ) LOGGER_INSTANCE.logV( lvl, msg, ##__VA_ARGS__ )

/* in case of a negative condition result log and invoke registered AbortHandler */
#define LOG_ASSERT( condition, msg ) if ( !(condition) ) LOGGER_INSTANCE.assert( __FILE__, __LINE__, msg )

#else

/* Backend selection. Backend has to be a global object */
#define LOGGER_SET_BACKEND( backend ) (void)0

/* Configuration: Set the UDP Backend port */
#define LOGGER_SET_UDP_PORT( port ) (void)0

/* Neat log close */
#define LOGGER_SHUTDOWN() (void)0

/* Basic log message macro. Preferably use the level-named macros. */
#define LOG_MSG_C( lvl, msg, ... ) (void)0

/* in case of a negative condition result log and invoke registered AbortHandler */
#define LOG_ASSERT( condition, msg ) (void)0

#endif

/**
 * Here is the list of macros, that can be defined using the actual TraceLog calls
 * or the '(void)0'.
 */

/* Enable the Console Backend */
#define LOGGER_INIT_BE_CONSOLE LOGGER_SET_BACKEND( &::trace::logBackend( ::trace::LogBackend_Console ) )

/* Enable the File Backend */
#define LOGGER_INIT_BE_FILE LOGGER_SET_BACKEND( &::trace::logBackend( ::trace::LogBackend_File ) )

/* Enable the UDP Server Backend */
#define LOGGER_INIT_BE_UDP LOGGER_SET_BACKEND( &::trace::logBackend( ::trace::LogBackend_Udp ) )

/* Enable the Default Backend */
#define LOGGER_INIT_BE_DEFAULT LOGGER_INIT_BE_CONSOLE

/* Log with INFO level */
#define LOG_INFO_C( msg, ... ) LOG_MSG_C( ::trace::LogLevel_Info, msg, ##__VA_ARGS__ )

/* Log with DEBUG level */
#define LOG_DEBUG_C( msg, ... ) LOG_MSG_C( ::trace::LogLevel_Debug, msg, ##__VA_ARGS__ )

/* Log with WARNING level */
#define LOG_WARN_C( msg, ... ) LOG_MSG_C( ::trace::LogLevel_Warning, msg, ##__VA_ARGS__ )

/* Log with ERROR level */
#define LOG_ERR_C( msg, ... ) LOG_MSG_C( ::trace::LogLevel_Error, msg, ##__VA_ARGS__ )

/* Log with DEVELOPER level */
#define LOG_DEV_C( msg, ... ) LOG_MSG_C( ::trace::LogLevel_Developer, msg, ##__VA_ARGS__ )


#endif // LOG_H_INTERFACE