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

#ifndef TRACEBUFFER_H
#define TRACEBUFFER_H

#include "trace/LogDefines.h"
#include "trace/entry/LogEntry.h"

namespace trace
{

/**
 * Trace Entry circular buffer implementation
 * 
 */
class TraceBuffer
{
public:
    TraceBuffer();
    ~TraceBuffer();
    
    /**
     * Add entry to buffer
     * @arg entry
     * @result true, if element was added
     */
    bool add( const LogEntry& entry );
    
    
    /**
     * Read an entry
     * @arg [out] entry
     * @result true, if element was available
     */
    bool read( LogEntry& entry );     
    
    /**
     * Get number of stored elements
     */
    size_t size() const;

private:    
    TraceBuffer(const TraceBuffer& other);
    TraceBuffer& operator=(const TraceBuffer& other);
    
    
    /* Log Entries */
    LogEntry m_entries[ LOG_CACHE_SIZE ]; 
    
    /* Write pointer */
    size_t m_writePtr;
    
    /* Read pointer */
    size_t m_readPtr;
};

}; // 

#endif // TRACEBUFFER_H
