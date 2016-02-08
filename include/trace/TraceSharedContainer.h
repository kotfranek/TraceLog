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

#ifndef TRACESHAREDCONTAINER_H
#define TRACESHAREDCONTAINER_H

#include "LogDefines.h"
#include "trace/entry/LogEntry.h"
#include "TraceBuffer.h"

#include <mutex>
#include <condition_variable>

namespace trace
{

class TraceSharedContainer
{
public:
    TraceSharedContainer();
    ~TraceSharedContainer();
    
    
    /**
     * Add entry to buffer
     * Thread safe.
     * @arg entry
     * @result true, if element was added
     */
    bool add( const entry::LogEntry& entry );
    
    
    /**
     * Read all available entries.
     * Thread safe.
     * @arg timeout Max waiting time
     * @arg [out] entry
     * @result number of entries
     */
    size_t waitUntilAvailableAndRead( const uint32_t timeout, entry::LogEntry* entryBuffer );
    
private:    
    TraceSharedContainer(const TraceSharedContainer& other);
    TraceSharedContainer& operator=(const TraceSharedContainer& other);
    
    /* Thread safety access mutex */
    ::std::mutex m_mutex;
    
    /* Data available signalling */
    ::std::condition_variable m_condDataAvail;

    /* Circular buffer */
    TraceBuffer m_buffer;
};

} // 

#endif // TRACESHAREDCONTAINER_H
