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

#include "trace/log.h"
#include "trace/Logger.h"
#include "trace/FileBackEnd.h"
#include "trace/ConsoleBackEnd.h"

namespace
{
    /* Logger instance */
    ::trace::Logger S_LOGGER;
    
    /* Console backend instance */
    ::trace::ConsoleBackEnd S_BE_CONSOLE;
        
    /* File backend instance */
    ::trace::FileBackEnd S_BE_FILE;    
}

namespace trace
{
    ILogger& logInstance()
    {
        return S_LOGGER;
    }
    
    
    ILogBackEnd& logBackend( const LogBackend backEnd )
    {
        switch( backEnd )
        {
            case LogBackend_Console:
                return S_BE_CONSOLE;
                
            case LogBackend_File:
                return S_BE_FILE;
                
            default:
                return S_BE_CONSOLE;
        }
        
        return S_BE_CONSOLE;
    }
};