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

#include "log/FileBackEnd.h"
#include "log/ConsoleBackEnd.h"
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
    const size_t MAX_LOG_ENTRY_STREAM_SIZE = sizeof( ::log::LogEntry::Data );
    
    /* Static buffer, accessed always with mutex, so this is safe */
    uint8_t ENTRY_BUFFER[ MAX_LOG_ENTRY_STREAM_SIZE ] = {};
    
    /* Standard File Header */
    const uint8_t LOG_FILE_HEADER[] = { 'l', 'o', 'g', '1' };
    
    /* Backend name */
    const ::std::string LOG_BACKEND_NAME( "File" );
    
    /* Console backend used for Error tracing */
    ::log::ConsoleBackEnd S_BE_CONSOLE;
};

namespace log
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


bool FileBackEnd::add( const LogEntry& entry )
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
    //::sys::StopWatch stWatch( true );

    if ( m_file.is_open() && 0U != m_index )
    {
        size_t bytesToWrite = 0U;
        
        for ( size_t i = 0U ; i < m_index; i++ )
        {            
             bytesToWrite = m_entries[ i ].exposeData().serialize( ENTRY_BUFFER );
             m_file.write( reinterpret_cast<const char*>( ENTRY_BUFFER ), bytesToWrite );
        }
    }
       
    //::std::cout << "persistEntries("<< m_index <<")-> took [" << sw.stop() << "] us" << std::endl;
}


void FileBackEnd::printToConsoleIfRequired( const LogEntry& entry ) const
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
