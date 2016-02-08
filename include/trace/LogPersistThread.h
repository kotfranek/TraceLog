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

#ifndef LOGPERSISTTHREAD_H
#define LOGPERSISTTHREAD_H

#include <mutex>
#include <condition_variable>

#include "trace/LogDefines.h"
#include "trace/LogEntry.h"
#include "sys/AbstractThread.h"

namespace trace
{
class ILogBackEnd;
class TraceSharedContainer;

class LogPersistThread : public ::sys::AbstractThread
{
public:
    explicit LogPersistThread( TraceSharedContainer& traceContainer );
    ~LogPersistThread();
    
    /**
     * Set the LogBackEnd
     * Call this only before the start!
     * @arg backEnd LogBackEnd implementation
     */
    void setBackEnd( ILogBackEnd* backEnd );
    
private:
    
    /**
     * @see ::sys::AbstractThread
     */
    virtual void run();
    
    /* Prohibit copying */
    LogPersistThread(const LogPersistThread& other);
    
    /* Prohibit copying */
    LogPersistThread& operator=(const LogPersistThread& other);  
    
    /* Trace Container instance */
    TraceSharedContainer& m_traceContainer;
    
    /* LogBackend implementation */
    ILogBackEnd* m_backEnd;    
        
    /* Log Cache index */
    size_t m_index;    
    
    /* Log Entries */
    LogEntry m_entries[ LOG_CACHE_SIZE ];    
};

}; // 

#endif // LOGPERSISTTHREAD_H
