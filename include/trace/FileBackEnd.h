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

#ifndef LOGGERBACKEND_H
#define LOGGERBACKEND_H

#include <stdint.h>
#include <string>
#include <fstream>

#include "trace/backend/ILogBackEnd.h"
#include "trace/LogDefines.h"
#include "trace/entry/LogEntry.h"

namespace trace
{

class FileBackEnd : public backend::ILogBackEnd
{
public:
        
    FileBackEnd();
    ~FileBackEnd();
    
   
private:
    FileBackEnd(const FileBackEnd& other);    
    FileBackEnd& operator=(const FileBackEnd& other);
    
    /**
     * @see ILogBackEnd
     */
    virtual bool add( const ::trace::entry::LogEntry& entry );

    /**
     * @see ILogBackEnd
     */
    virtual void onRegister( const ::sys::TPid pid );
    
    /**
     * @see ILogBackEnd
     */    
    virtual void onShutdown();
    
    /**
     * @see ILogBackEnd
     */      
    virtual const ::esys::TString31& getName() const;
       
    /**
     * Persist the entries buffer
     */
    void persistEntries();
    
    /** 
     * Depending on the settings and the Trace Level will print 
     * the content to console 
     * @arg entry Log item
     */
    void printToConsoleIfRequired( const entry::LogEntry& entry ) const;
        
    /* Whether to print the error messages to console */
    bool m_errorToConsole;
                
    /* Log Cache index */
    size_t m_index;    
    
    /* Log Entries */
    entry::LogEntry m_entries[ LOG_CACHE_SIZE ];
    
    /* Output file handle */
    ::std::fstream m_file;
};

};

#endif // LOGGERBACKEND_H
