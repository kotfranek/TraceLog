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

#include "trace/TraceBuffer.h"
#include <cstdlib>

namespace
{
    /**
     * Range-Safe Increment routine
     * @arg value to be Incremented
     */
    inline size_t safeIncrement( const size_t& value )
    {
        return ( value + 1U ) % ::trace::LOG_CACHE_SIZE;        
    }
}

namespace trace
{

TraceBuffer::TraceBuffer()
    : m_entries()
    , m_writePtr( 0U )
    , m_readPtr( 0U )
{

}


bool TraceBuffer::add( const entry::LogEntry& entry )
{
    const size_t next = ::safeIncrement( m_writePtr );
    bool writePossible = next != m_readPtr;
    
    if ( writePossible )
    {
        m_entries[ m_writePtr ] = entry; 
        m_writePtr = next;
    }    
    
    return writePossible;
}


bool TraceBuffer::read( entry::LogEntry& entry )
{
    bool readPossible = m_readPtr != m_writePtr;
    
    if ( readPossible )
    {
        entry = m_entries[ m_readPtr ];
        m_readPtr = ::safeIncrement( m_readPtr );;
    }    
    
    return readPossible;
}


size_t TraceBuffer::size() const
{
    ssize_t result = m_writePtr - m_readPtr;
    
    if ( result < 0 )
    {
        result += LOG_CACHE_SIZE;
    }
    return result;
}



TraceBuffer::~TraceBuffer()
{

}


}; // 
