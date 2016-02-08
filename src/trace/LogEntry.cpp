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

#include "trace/LogEntry.h"
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
    char traceLevelToChar( const ::trace::LogLevel level )
    {
        char result = 'x';
        
        switch( level )
        {
            case ::trace::LogLevel_Info:
                result = 'I';
                break;
                
            case ::trace::LogLevel_Debug:
                result = 'D';
                break;
                
            case ::trace::LogLevel_Warning:
                result = 'W';
                break;
                
            case ::trace::LogLevel_Error:
                result = 'E';
                break;
                
            case ::trace::LogLevel_Fatal:
                result = 'F';
                break;
                
            case ::trace::LogLevel_Assert:
                result = 'A';
                break;                
                                
            case ::trace::LogLevel_Internal:
                result = 'L';
                break;
                                
            case ::trace::LogLevel_Developer:
                result = 'T';
                break;                
            
            default:
                break;
                
        }
        return result;
    }
    
    
    template<typename T> size_t serializeVariable( uint8_t* dest, const T& variable )
    {
        for( size_t i = 0U; i < sizeof( T ); i++ )
        {
            *( dest + i ) = static_cast<uint8_t>( ( variable >> ( 8U * i ) ) & 0xFF );
        }
        
        return sizeof( T );
    }
    
       
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
    
size_t LogEntry::Data::serialize( uint8_t* output ) const
{
    size_t offset = 0U;
    const uint16_t messageLength = strlen( m_message );
    
    offset += ::esys::serialize( output + offset, m_timestamp );
    
    *( output + offset ) = traceLevelToChar( m_level );
    ++offset;
    
    offset += ::esys::serialize( output + offset, messageLength );
    
    memcpy( output + offset, m_message, messageLength );
    offset += messageLength;
    
    return offset;
}
    
    
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
    m_data.m_timestamp = timestamp;
    m_data.m_level = level;
    ::std::strncpy( m_data.m_message, message, LOG_MESSAGE_SIZE_MAX );
}


void LogEntry::set( const LogLevel level, const char* message )
{
    m_data.m_timestamp = getTimestamp();
    m_data.m_level = level;    
    ::std::strncpy( m_data.m_message, message, LOG_MESSAGE_SIZE_MAX );   
}


void LogEntry::set( const LogLevel level, const char* format, va_list args )
{
    m_data.m_timestamp = getTimestamp();
    m_data.m_level = level;  
    ::std::vsnprintf( m_data.m_message, LOG_MESSAGE_SIZE_MAX, format, args );
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
      
    result << "|" << ::traceLevelToChar( m_data.m_level ) << "|" << m_data.m_message;
    
    return result.str();
}
    
};
