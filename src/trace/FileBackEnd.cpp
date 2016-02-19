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
#include "trace/entry/PayloadHelper.h"
#include "sys/StopWatch.h"
#include "esys/AutoString.h"


namespace
{    
    /* Open Mode: Binary Write, Truncate if exists */
    const char* LOG_FILE_OPEN_MODE = "wb";
        
    /* Maximal Single log Entry binary representation length */
    const size_t MAX_ENTRY_SIZE = sizeof( ::trace::entry::Payload );
    
    /* Static buffer, accessed always with mutex, so this is safe */
    //uint8_t ENTRY_BUFFER[ MAX_LOG_ENTRY_STREAM_SIZE ] = {};
    
    /* Standard File Header */
    const uint8_t LOG_FILE_HEADER[] = { 'l', 'o', 'g', '1' };
    
    /* Backend name */
    const ::esys::TString31 LOG_BACKEND_NAME( "File" );
    
    /* Console backend used for Error tracing */
    ::trace::ConsoleBackEnd S_BE_CONSOLE;
    
    
    /**
     * Check if the given file is valid/opened
     * @param file
     * @return 
     */
    inline bool isOpened( const ::std::FILE* file )
    {
        return NULL != file;
    }
};

namespace trace
{

FileBackEnd::FileBackEnd()
    : ILogBackEnd()
    , m_errorToConsole( true )
    , m_cachePtr( 0U )
    , m_cache()
    , m_file( NULL )
{

}


void FileBackEnd::onRegister( const ::sys::TPid pid )
{    
    ::esys::TString63 fileName;    
    fileName.c_format( "tracelog_%d.log", pid );    
    
    m_file = ::std::fopen( fileName.c_str() , ::LOG_FILE_OPEN_MODE );
    
    if ( ::isOpened( m_file ) )
    {
        //::std::setvbuf ( m_file, (char*) m_entries, _IOFBF, LOG_FILE_BACKEND_CACHE_SIZE_BYTES );
        //::std::setvbuf ( m_file, NULL, _IONBF, 0 );
        ::std::fwrite( LOG_FILE_HEADER, sizeof( LOG_FILE_HEADER ), 1U,  m_file );
        ::std::fflush( m_file );
    }
}


void FileBackEnd::onShutdown()
{    
    if ( ::isOpened( m_file ) )
    {
        persistEntries();
        ::std::fclose( m_file );
    }
}


bool FileBackEnd::add( const entry::LogEntry& entry )
{     
    if ( m_cachePtr >= LOG_FILE_BACKEND_CACHE_SIZE_BYTES - MAX_ENTRY_SIZE )
    {
        persistEntries();
    }
    
    size_t msgLength = 0U;
    entry::PayloadHelper helper( entry.exposeData() );

    msgLength = helper.serializeHeader( m_cache + m_cachePtr );
    m_cachePtr += entry::PayloadHelper::getHeaderLength();

    ::memcpy( m_cache + m_cachePtr, helper.message(), msgLength );
    m_cachePtr +=msgLength;
    
    
    printToConsoleIfRequired( entry );
    
    return true;
}


const ::esys::TString31& FileBackEnd::getName() const
{
    return LOG_BACKEND_NAME;
}


void FileBackEnd::persistEntries()
{
    if ( ::isOpened( m_file ) && 0U != m_cachePtr )
    {
        ::std::fwrite( m_cache, m_cachePtr, 1U, m_file );
        //::std::fflush( m_file );
        m_cachePtr = 0U;
    }
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
