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
#include <cstdlib>
#include <iostream>
#include <unistd.h>

#include "sys/SystemInfo.h"

#include "trace/model/LogPersistThread.h"
#include "trace/model/TraceSharedContainer.h"
#include "trace/backend/ILogBackEnd.h"

namespace
{
    /* Max time in ms to wait for the LogEntry */
    const uint32_t NEW_ENTRY_TIMEOUT_MS = 100U;
    
    /* Let the thread sleep a bit while reading */
    const uint32_t GET_ENTRIES_LOOP_SLEEP_US = 25U;
};

namespace trace
{
namespace model
{   
LogPersistThread::LogPersistThread ( model::TraceSharedContainer& traceContainer )
    : ::sys::AbstractThread( "LogPersistence" )
    , m_traceContainer( traceContainer )
    , m_backEnd( NULL )
    , m_entries()
{

}


void LogPersistThread::setBackEnd( backend::ILogBackEnd* backEnd )
{
    m_backEnd = backEnd;
}


void LogPersistThread::run()
{   
    ::esys::TString63 backendRegistered;
    backendRegistered.c_format( "Backend '%s' registered", m_backEnd->getName().c_str() );
    
    m_backEnd->onRegister( ::sys::SystemInfo::getOwnProcessId() );
    m_backEnd->add( entry::LogEntry( LogLevel_Internal, backendRegistered.c_str() ) );   
    
    bool assertion = false;
    size_t count = 0U;
    
    do
    {
        if ( m_traceContainer.waitForEntries( ::NEW_ENTRY_TIMEOUT_MS ) )
        {
            do
            {
                count = m_traceContainer.getEntries( m_entries, LOG_PERSIST_THREAD_CACHE );

                for ( size_t i = 0U; i < count; i++ )
                {
                    assertion = m_entries[ i ].isLevel( LogLevel_Assert );
                    m_backEnd->add( m_entries[ i ] );
                }
                
                /* Let the feeding thread in */
                sleepUs( ::GET_ENTRIES_LOOP_SLEEP_US );
            }
            while( count != 0U );
        }
        
        /* Exit the thread loop in case of ASSERT level entry */
        if ( assertion )
        {
            break;
        }
    }
    while ( !isStopRequested() );    
       
    /* Add the closing entry and shutdown the LogBackEnd */
    m_backEnd->add( entry::LogEntry( LogLevel_Internal, "Shutdown the BackEnd" ) );    
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
};
