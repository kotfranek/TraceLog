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

#include "trace/entry/LogEntry.h"
#include "trace/entry/TraceLevelInfo.h"
#include "esys/utils.h"

#include <cstring>
#include <sstream>
#include <chrono>
#include <cstdarg>
#include <cstdio>
#include <ctime>
#include <iomanip>

namespace
{           
    void msecToTime( tm& destTm, uint16_t& destMsec, const uint64_t& timeMsec )
    {
        destMsec = timeMsec % 1000U;
        const time_t epochTime = timeMsec / 1000U;
        
        destTm = *localtime( &epochTime );
    }
    
    
    void msecToTime( ::std::stringstream& dest, const uint64_t& timeMsec )
    {
        static char buffer[ 24 ];
        
        ::std::tm time;
        uint16_t msec = 0U;
    
        ::msecToTime( time, msec, timeMsec );        
        
        const size_t offset = strftime( buffer, sizeof( buffer ), "%T", &time );
        snprintf( buffer + offset, 5, ".%03u", msec );
        
        dest << buffer ;
    }
}

namespace trace
{
namespace entry
{
    
LogEntry::LogEntry()
    : LogEntry( 0U, LogLevel_Debug, "" )
{

}
    
    
LogEntry::LogEntry( const LogLevel level, const char* message )
    : LogEntry( getTimestamp(), level, message )
{
    
}


LogEntry::LogEntry( const uint64_t& timestamp, const LogLevel level, const char* message )
    : m_data()
{
    set( level, message );
    m_data.m_timestamp = timestamp;    
}


void LogEntry::set( const LogLevel level, const char* message )
{
    m_data.m_timestamp = getTimestamp();
    m_data.m_level = level;    
    ::std::strncpy( m_data.m_message, message, LOG_MESSAGE_SIZE_MAX );  
    m_data.m_length = ::strlen( m_data.m_message );
}


void LogEntry::set( const LogLevel level, const char* format, va_list args )
{
    m_data.m_timestamp = getTimestamp();
    m_data.m_level = level;  
    const int32_t length = ::std::vsnprintf( m_data.m_message, LOG_MESSAGE_SIZE_MAX, format, args );
    
    if ( length >= 0 )
    {
        m_data.m_length = length % ( LOG_MESSAGE_SIZE_MAX - 1U );
        m_data.m_message[ LOG_MESSAGE_SIZE_MAX - 1U ] = '\0';
    }
    else
    {
        m_data.m_length = 0U;
    }
}



uint64_t LogEntry::getTimestamp()
{
    using namespace ::std::chrono;
    const milliseconds ms = duration_cast< milliseconds >( system_clock::now().time_since_epoch() );
    return ms.count();
}


std::string LogEntry::toString() const
{
    ::std::stringstream result;
    
    ::msecToTime( result, m_data.m_timestamp );
      
    result << "|" << TraceLevelInfo( m_data.m_level ).charId() << "|" << m_data.m_message;
    
    return result.str();
}

};   
};
