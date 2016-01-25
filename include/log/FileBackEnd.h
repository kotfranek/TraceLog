/*
 * Copyright 2016 Przemyslaw Podwapinski <p.podwapinski@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef LOGGERBACKEND_H
#define LOGGERBACKEND_H

#include <stdint.h>
#include <string>
#include <fstream>
#include <array>

#include "log/ILogBackEnd.h"
#include "log/LogDefines.h"
#include "log/LogEntry.h"

namespace log
{

class FileBackEnd : public ILogBackEnd
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
    virtual bool add( const LogEntry& entry );

    /**
     * @see ILogBackEnd
     */
    virtual void onRegister();
    
    /**
     * @see ILogBackEnd
     */    
    virtual void onShutdown();
    
    /**
     * @see ILogBackEnd
     */      
    virtual const ::std::string& getName() const;
       
    /**
     * Persist the entries buffer
     */
    void persistEntries();
    
    /** 
     * Depending on the settings and the Trace Level will print 
     * the content to console 
     * @arg entry Log item
     */
    void printToConsoleIfRequired( const LogEntry& entry ) const;
        
    /* Whether to print the error messages to console */
    bool m_errorToConsole;
                
    /* Log Cache index */
    size_t m_index;    
    
    /* Log Entries */
    LogEntry m_entries[ LOG_CACHE_SIZE ];
    
    /* Output file handle */
    ::std::fstream m_file;
};

};

#endif // LOGGERBACKEND_H
