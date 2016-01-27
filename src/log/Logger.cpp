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

#include "log/Logger.h"
#include "log/ILogBackEnd.h"

#include <cstdarg>
#include <cstdio>
#include <sstream>
#include <iostream>

namespace trace 
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