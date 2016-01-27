/*
 * Copyright (c) 2016, Przemyslaw Podwapinski <p.podwapinski@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Przemyslaw Podwapinski <p.podwapinski@gmail.com> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Przemyslaw Podwapinski <p.podwapinski@gmail.com> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "log/LogPersistThread.h"
#include "log/ILogBackEnd.h"
#include "log/TraceSharedContainer.h"

#include <cstdlib>
#include <iostream>

namespace
{
    /* Max time in ms to wait for the LogEntry */
    const uint32_t NEW_ENTRY_TIMEOUT_MS = 250U;
    
    
    inline ::std::string backendRegisteredText( const ::std::string& backendName )
    {
        ::std::string descr( "BackEnd '" );
        descr.append( backendName );
        descr.append( "' registered" );   
        
        return descr;
    }
};

namespace trace
{
   
LogPersistThread::LogPersistThread ( TraceSharedContainer& traceContainer )
    : ::sys::AbstractThread( "LogPersistence" )
    , m_traceContainer( traceContainer )
    , m_backEnd( NULL )
    , m_index( 0u )
    , m_entries()
{

}


void LogPersistThread::setBackEnd( ILogBackEnd* backEnd )
{
    m_backEnd = backEnd;
}


void LogPersistThread::run()
{   
    m_backEnd->onRegister();
    m_backEnd->add( LogEntry( LogLevel_Internal, ::backendRegisteredText( m_backEnd->getName() ).c_str() ) );   
    
    bool assertion = false;
    
    while ( !isStopRequested() )
    {
        const size_t count = m_traceContainer.waitUntilAvailableAndRead( ::NEW_ENTRY_TIMEOUT_MS, m_entries );
               
        for ( size_t i = 0U; i < count; i++ )
        {
            assertion = m_entries[ i ].isLevel( LogLevel_Assert );
            m_backEnd->add( m_entries[ i ] );
        } 
        
        /* Exit the thread loop in case of ASSERT level entry */
        if ( assertion )
        {
            break;
        }
    }
    
    /* Add the closing entry and shutdown the LogBackEnd */
    m_backEnd->add( LogEntry( LogLevel_Internal, "Shutdown the BackEnd" ) );    
    m_backEnd->onShutdown();
    
    /* Here the LogBackEnd is switched off, all entries are expected to be persisted.
     * It is safe to call the AssertHandler. */    
    if ( assertion )
    {
        abort();
    }
}



LogPersistThread::~LogPersistThread()
{

}

};
