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

#include "trace/FileBackEnd.h"
#include "trace/ConsoleBackEnd.h"
#include "trace/entry/LogEntry.h"
#include "sys/StopWatch.h"
#include <iostream>


namespace
{
    /* Default file name */
    const char* LOG_FILE_NAME = "log_trace.log";
    
    /* Open Mode: Binary Write, Truncate if exists */
    const ::std::ios_base::openmode LOG_FILE_OPEN_MODE = 
        ::std::ios_base::in | ::std::ios_base::out | ::std::ios_base::binary | ::std::ios_base::trunc;        
        
    /* Maximal Single log Entry binary representation length */
    const size_t MAX_LOG_ENTRY_STREAM_SIZE = sizeof( ::trace::entry::Payload );
    
    /* Static buffer, accessed always with mutex, so this is safe */
    uint8_t ENTRY_BUFFER[ MAX_LOG_ENTRY_STREAM_SIZE ] = {};
    
    /* Standard File Header */
    const uint8_t LOG_FILE_HEADER[] = { 'l', 'o', 'g', '1' };
    
    /* Backend name */
    const ::std::string LOG_BACKEND_NAME( "File" );
    
    /* Console backend used for Error tracing */
    ::trace::ConsoleBackEnd S_BE_CONSOLE;
};

namespace trace
{

FileBackEnd::FileBackEnd()
    : ILogBackEnd()
    , m_errorToConsole( true )
    , m_index( 0U )
    , m_entries()
    , m_file()
{

}


void FileBackEnd::onRegister()
{    
    m_file.open( LOG_FILE_NAME, LOG_FILE_OPEN_MODE );
    
    if ( m_file.is_open() )
    {
        m_file.write( reinterpret_cast<const char*>( LOG_FILE_HEADER ), sizeof( LOG_FILE_HEADER ) );
    }
}


void FileBackEnd::onShutdown()
{    
    if ( m_file.is_open() )
    {
        persistEntries();
        m_file.close();
    }
}


bool FileBackEnd::add( const entry::LogEntry& entry )
{
    if ( LOG_CACHE_SIZE <= m_index )
    {
        persistEntries();
        m_index = 0U;
    }
    
    m_entries[ m_index ] = entry;
    ++m_index; 
    
    printToConsoleIfRequired( entry );
    
    return true;
}


const ::std::string& FileBackEnd::getName() const
{
    return LOG_BACKEND_NAME;
}


void FileBackEnd::persistEntries()
{
    ::sys::StopWatch stWatch( true );

    if ( m_file.is_open() && 0U != m_index )
    {
        size_t bytesToWrite = 0U;
        
        for ( size_t i = 0U ; i < m_index; i++ )
        {            
             bytesToWrite = m_entries[ i ].exposeData().serialize( ENTRY_BUFFER );
             m_file.write( reinterpret_cast<const char*>( ENTRY_BUFFER ), bytesToWrite );
        }
    }
       
    ::std::cout << "persistEntries("<< m_index <<")-> took [" << stWatch.stop() << "] us" << std::endl;
}


void FileBackEnd::printToConsoleIfRequired( const entry::LogEntry& entry ) const
{
    if ( m_errorToConsole && entry.exposeData().m_level <= LogLevel_Error)
    {
        static_cast<ILogBackEnd*>( &::S_BE_CONSOLE )->add( entry );
    }
}


FileBackEnd::~FileBackEnd()
{

}


};
