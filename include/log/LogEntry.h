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
