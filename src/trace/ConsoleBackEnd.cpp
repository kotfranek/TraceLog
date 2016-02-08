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

#include "trace/ConsoleBackEnd.h"
#include "trace/LogDefines.h"
#include "trace/entry/LogEntry.h"

#include <iostream>

namespace
{
    /* Backend name */
    const ::std::string LOG_BACKEND_NAME( "Console" );    
}

namespace trace
{

ConsoleBackEnd::ConsoleBackEnd()
    : ILogBackEnd()
{
}


void ConsoleBackEnd::onRegister()
{

}


void ConsoleBackEnd::onShutdown()
{

}



bool ConsoleBackEnd::add( const LogEntry& entry )
{
    ::std::ostream& outStream = ( entry.exposeData().m_level > LogLevel_Error ) ? ::std::cout : ::std::cerr;

    outStream << entry.toString() << ::std::endl;
    
    return true;
}



const ::std::string& ConsoleBackEnd::getName() const
{
    return ::LOG_BACKEND_NAME;
}

};
