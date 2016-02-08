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

#include "trace/TraceSharedContainer.h"
#include "sys/ESysDefs.h"

#include <iostream>


namespace trace
{

TraceSharedContainer::TraceSharedContainer()
    : m_mutex()
    , m_condDataAvail()
    , m_buffer()
{

}


bool TraceSharedContainer::add( const LogEntry& entry )
{
    ::sys::TLockMutex l( m_mutex );

    const bool result = m_buffer.add( entry );
    
    m_condDataAvail.notify_all();
    
    return result;
}


size_t TraceSharedContainer::waitUntilAvailableAndRead( const uint32_t timeout, LogEntry* entryBuffer )
{
    ::sys::TLockUnique l( m_mutex );
    
    size_t result = 0U;
    
    if ( std::cv_status::no_timeout == m_condDataAvail.wait_for( l, ::std::chrono::milliseconds( timeout ) ) )
    {
        result = m_buffer.size();        
        
        for( size_t i = 0U; i < result; i++ )
        {
            if ( m_buffer.read( entryBuffer[ i ] ) )
            {
                //::std::cout << "B" << ::std::endl;            
            }
        }        
    }
    
    return result;
}



TraceSharedContainer::~TraceSharedContainer()
{

}

};