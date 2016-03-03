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

#ifndef ILOGGER_H
#define ILOGGER_H

#include <string>

#include "trace/LogDefines.h"

namespace trace
{           
    namespace backend
    {
        class ILogBackEnd;
    }
        
    class ILogger
    {        
    public:        
        /**
         * Log a single string
         * @arg level 
         * @arg message
         */
        virtual void log( const LogLevel level, const ::std::string& message ) = 0;
        
        
        /**
         * Log a formatted C-string
         */
        virtual void logV( const LogLevel level, const char * format, ... ) = 0;
        
        
        /**
         * Log a C-string
         * @arg level 
         * @arg format Format C-string
         * @arg ... variable arguments
         */
        virtual void log( const LogLevel level, const char* message ) = 0;
        
        
        /**
         * Execute an assertion and log the provided text
         */
        virtual void assert( const char* fileName, const uint32_t line, const char* message ) = 0;
              
        
        /**
         * Set the backend implementation 
         * @arg backEnd Valid implementation
         */
        virtual void setBackEnd( backend::ILogBackEnd* backEnd ) = 0;
        
        
        /**
         * Close the log, clean the resources, notify the BackEnd
         */
        virtual void shutDown() = 0;                

    protected:
        ILogger()
        {
        };
        
        virtual ~ILogger()
        {
        };
        
    };

};

#endif // ILOGGER_H
