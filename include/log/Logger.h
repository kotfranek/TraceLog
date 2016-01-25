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

#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>
#include <mutex>
#include <string>
#include <ostream>
#include "log/LogDefines.h"
#include "log/LogEntry.h"
#include "log/TraceSharedContainer.h"
#include "log/LogPersistThread.h"

namespace log
{       
    class ILogBackEnd;
        
    class Logger
    {        
    public:
        Logger();
        ~Logger();
        
        /**
         * Log a single string
         * @arg level 
         * @arg message
         */
        void log( const LogLevel level, const ::std::string& message );
        
        
        /**
         * Log a formatted C-string
         */
        void logV( const LogLevel level, const char * format, ... );
        
        
        /**
         * Log a C-string
         * @arg level 
         * @arg format Format C-string
         * @arg ... variable arguments
         */
        void log( const LogLevel level, const char* message );
        
        
        /**
         * Execute an assertion and log the provided text
         */
        void assert( const char* fileName, const uint32_t line, const char* message );
              
        
        /**
         * Set the backend implementation 
         * @arg backEnd Valid implementation
         */
        void setBackEnd( ILogBackEnd* backEnd );
        
        
        /**
         * Close the log, clean the resources, notify the BackEnd
         */
        void shutDown();

    private:    

        Logger(const Logger& other);    
        Logger& operator=(const Logger& other);
        
        /* Thread safety access mutex */
        ::std::mutex m_mutex;           
        
        /* Trace Entry buffer */
        TraceSharedContainer m_buffer;  
        
        /* Log data persist thread */
        LogPersistThread m_persistThread;
               
        /* Log backend */
        ILogBackEnd* m_backEnd;
    };

};

#endif // LOGGER_H
