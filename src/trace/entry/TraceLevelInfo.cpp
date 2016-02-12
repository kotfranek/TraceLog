/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TraceLevelInfo.cpp
 * Author: kret
 * 
 * Created on February 12, 2016, 11:09 PM
 */

#include "trace/entry/TraceLevelInfo.h"
#include "esys/utils.h"

namespace
{
    /* Level Descriptor */
    struct TraceLevelDescriptor
    {
        ::trace::LogLevel m_logLevel;
        char m_char;
        const char* m_name;
    };
    
    /* All levels mapping */
    const TraceLevelDescriptor TRACE_LEVELS[] = 
    {
        { ::trace::LogLevel_Assert, 'A', "Assertion" },
        { ::trace::LogLevel_Fatal, 'F', "Fatal" },
        { ::trace::LogLevel_Error, 'E', "Error" },
        { ::trace::LogLevel_Warning, 'W', "Warning" },
        { ::trace::LogLevel_Info, 'I', "Info" },
        { ::trace::LogLevel_Debug, 'D', "Debug" },
        { ::trace::LogLevel_Developer, 'P', "Developer" },
        { ::trace::LogLevel_Internal, 'L', "Internal" },
    };
    
    /* Calculate the size */
    const size_t TRACE_LEVELS_SIZE = ESYS_ARRAY_SIZE( TRACE_LEVELS );
    
    /**
     * Log Level to Character
     * @param level
     * @return 
     */
    char traceLevelToChar( const ::trace::LogLevel level )
    {
        char result = 'x';
        
        for ( size_t i = 0; i < TRACE_LEVELS_SIZE; i++ )
        {
            const TraceLevelDescriptor& descr = TRACE_LEVELS[ i ];
            
            if( level == descr.m_logLevel )
            {
                result = descr.m_char;
                break;
            }
        }
        
        return result;
    }
    
    
    ::trace::LogLevel charToTraceLevel( const char character )
    {
        ::trace::LogLevel result = ::trace::LogLevel_Unknown;
        
        for ( size_t i = 0; i < TRACE_LEVELS_SIZE; i++ )
        {
            const TraceLevelDescriptor& descr = TRACE_LEVELS[ i ];
            
            if( character == descr.m_char )
            {
                result = descr.m_logLevel;
                break;
            }
        }
        
        return result;        
    }
    
    
    const char* traceLevelToName( const ::trace::LogLevel level )
    {
        const char* result = "Unknown";
        
        for ( size_t i = 0; i < TRACE_LEVELS_SIZE; i++ )
        {
            const TraceLevelDescriptor& descr = TRACE_LEVELS[ i ];
            
            if( level == descr.m_logLevel )
            {
                result = descr.m_name;
                break;
            }
        }
        
        return result;
    }    
    
}

namespace trace
{
namespace entry 
{
TraceLevelInfo::TraceLevelInfo( const ::trace::LogLevel traceLevel )
    : m_level( traceLevel )
    , m_char( ::traceLevelToChar( traceLevel ) )
{   
}


TraceLevelInfo::TraceLevelInfo( const char traceLevel )
    : m_level( ::charToTraceLevel( traceLevel ) )
    , m_char( traceLevel )
{    
}


const char* TraceLevelInfo::name() const
{
    return ::traceLevelToName( m_level );
}
    
}
};

