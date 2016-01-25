/*
 * Copyright 2016 Przemyslaw Podwapinski <p.podwapinski@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "log/Logger.h"
#include "log/ILogBackEnd.h"

#include <cstdarg>
#include <cstdio>
#include <sstream>
#include <iostream>

namespace log 
{
typedef ::std::lock_guard<std::mutex> TLockMutex;
        
Logger::Logger()
    : m_mutex()
    , m_buffer()
    , m_persistThread( m_buffer )
    , m_backEnd( NULL )
{

}


void Logger::setBackEnd( ILogBackEnd* backEnd )
{
    TLockMutex l( m_mutex );
    
    if ( NULL == m_backEnd )
    {
        m_backEnd = backEnd;
        m_persistThread.setBackEnd( backEnd );
        m_persistThread.start();
    }
}


void Logger::log( const LogLevel level, const std::string& message)
{
    log( level, message.c_str() );
}


void Logger::logV( const LogLevel level, const char* format, ... )
{    
    va_list args;
    va_start ( args, format );
    
    LogEntry entry;
    entry.set( level, format, args );
    
    if ( m_buffer.add( entry ) )
    {
        
    }     
    
    va_end ( args );    
}



void Logger::log( const LogLevel level, const char* message )
{
    logV( level, "%s", message );
}


void Logger::shutDown()
{
    TLockMutex l( m_mutex );
    
    if ( NULL != m_backEnd )
    {
        m_persistThread.requestStop();
        m_persistThread.join();
        
        m_backEnd = NULL;
    }
}


void Logger::assert( const char* fileName, const uint32_t line, const char* message )
{
    logV( LogLevel_Assert, "%s:%u-> %s", fileName, line, message );
}




Logger::~Logger()
{

}

};