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

#include "trace/model/TraceSharedContainer.h"
#include "sys/SysTypes.h"

#include <iostream>

namespace
{
    /* Condition Variable "No timeout" value */
    const auto COND_NO_TIMEOUT = ::std::cv_status::no_timeout; 
}


namespace trace
{
namespace model
{
TraceSharedContainer::TraceSharedContainer()
    : m_mutex()
    , m_condDataAvail()
    , m_buffer()
{

}


bool TraceSharedContainer::add( const entry::LogEntry& entry )
{
    bool result = false;
    {
        ::sys::TLockMutex l( m_mutex );
        result = m_buffer.add( entry );
    }
    if ( result )
    {
        m_condDataAvail.notify_one();
    }
    
    return result;
}


bool TraceSharedContainer::waitForEntries( const uint32_t timeout )
{
    ::sys::TLockUnique l( m_mutex );
    
    return ::COND_NO_TIMEOUT == m_condDataAvail.wait_for( l, ::std::chrono::milliseconds( timeout ) );
}


size_t TraceSharedContainer::getEntries( entry::LogEntry* destination, const size_t maxNumber )
{
    ::sys::TLockMutex l( m_mutex );
    
    return readEntries( destination, maxNumber );
}


size_t TraceSharedContainer::readAllRemaining( entry::LogEntry* entryBuffer )
{
    ::sys::TLockMutex l( m_mutex );
    
    return readEntries( entryBuffer, m_buffer.size() );
}


size_t TraceSharedContainer::readEntries( entry::LogEntry* entryBuffer, const size_t bufferSize )
{
    const size_t result = ::std::min( bufferSize, m_buffer.size() );

    for( size_t i = 0U; i < result; i++ )
    {
        if ( m_buffer.read( entryBuffer[ i ] ) )
        {
            //::std::cout << "B" << ::std::endl;            
        }
    }     
    
    return result;    
}


TraceSharedContainer::~TraceSharedContainer()
{

}
}
};