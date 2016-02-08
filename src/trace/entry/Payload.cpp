/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Payload.cpp
 * Author: kret
 * 
 * Created on February 8, 2016, 10:54 PM
 */

#include "trace/entry/Payload.h"
#include "esys/utils.h"
#include <cstring>

namespace
{
    char traceLevelToChar( const ::trace::LogLevel level )
    {
        char result = 'x';
        
        switch( level )
        {
            case ::trace::LogLevel_Info:
                result = 'I';
                break;
                
            case ::trace::LogLevel_Debug:
                result = 'D';
                break;
                
            case ::trace::LogLevel_Warning:
                result = 'W';
                break;
                
            case ::trace::LogLevel_Error:
                result = 'E';
                break;
                
            case ::trace::LogLevel_Fatal:
                result = 'F';
                break;
                
            case ::trace::LogLevel_Assert:
                result = 'A';
                break;                
                                
            case ::trace::LogLevel_Internal:
                result = 'L';
                break;
                                
            case ::trace::LogLevel_Developer:
                result = 'T';
                break;                
            
            default:
                break;
                
        }
        return result;
    }    
}

namespace trace
{
namespace entry
{
    
Payload::Payload() 
    : m_timestamp( 0U )
    , m_level( ::trace::LogLevel_Internal )
    , m_message()
{
}


size_t Payload::serialize( uint8_t* output ) const
{
    size_t offset = 0U;
    const uint16_t messageLength = strlen( m_message );
    
    offset += ::esys::serialize( output + offset, m_timestamp );
    
    *( output + offset ) = traceLevelToChar( m_level );
    ++offset;
    
    offset += ::esys::serialize( output + offset, messageLength );
    
    memcpy( output + offset, m_message, messageLength );
    offset += messageLength;
    
    return offset;
}


void Payload::deSerialize( uint8_t* input )
{
    
}


char Payload::traceLevelToChar( const ::trace::LogLevel level )
{
    return ::traceLevelToChar( level );
}

}
}
