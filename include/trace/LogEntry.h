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
#ifndef LOGENTRY_H
#define LOGENTRY_H

#include <string>
#include <cstdarg>

#include "LogDefines.h"

namespace trace
{
    class LogEntry
    {
    public:
        
        struct Data
        {
            /* Timestamp: ms since epoch */
            uint64_t m_timestamp;
            
            /* Trace level */
            LogLevel m_level;
            
            /* Message content */
            char m_message[ LOG_MESSAGE_SIZE_MAX ]; 
            
            /**
             * Serilize the buffer content. 
             * @arg output buffer
             * @return number of bytes written
             */
            size_t serialize( uint8_t* output ) const;
        };
               
        LogEntry();
        LogEntry( const LogLevel level, const char* message );
        
        /* Set the entry values */
        void set( const LogLevel level, const char* message );
        
        /* Set the entry values with a format string */
        void set( const LogLevel level, const char* format, va_list args );
        
        /* Expose complete log entry R/O */
        const Data& exposeData() const
        {
            return m_data;
        }
        
        /* Thest the LogLevel */
        bool isLevel( const LogLevel level ) const
        {
            return level == m_data.m_level;
        }
        
        
        /* Convert to text */
        ::std::string toString() const;
        
    private:
        LogEntry( const uint64_t& timestamp, const LogLevel level, const char* message );
        
        /* Get actual timestamp in ms */
        static uint64_t getTimestamp();
        
        /* Log content */
        Data m_data;
    };
}

#endif // LOGENTRY_H
