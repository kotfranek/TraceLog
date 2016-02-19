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


/* 
 * File:   Payload.cpp
 * Author: kret
 * 
 * Created on February 8, 2016, 10:54 PM
 */

#include "trace/entry/Payload.h"
#include "trace/entry/TraceLevelInfo.h"
#include "esys/utils.h"

#include <cstring>

namespace
{
    template<typename T>bool readAndDeserialize( char* buffer, T& value, ::std::istream& stream )
    {
        bool result = false;
        
        stream.read( buffer, sizeof( T ) );
        
        if ( sizeof( T ) == stream.gcount() )
        {
            ::esys::deserialize( buffer, value );
            result = true;            
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


Payload& Payload::operator =( const Payload& other )
{
    m_timestamp = other.m_timestamp;
    m_level = other.m_level;
    m_length = other.m_length;
    ::memcpy( m_message, other.m_message, other.m_length );
    m_message[ m_length ] = '\0';
    return *this;
}


size_t Payload::serialize( uint8_t* output ) const
{
    size_t offset = 0U;
    const uint16_t messageLength = strlen( m_message );
    
    offset += ::esys::serialize( output + offset, m_timestamp );
    
    *( output + offset ) = TraceLevelInfo( m_level ).charId();
    ++offset;
    
    offset += ::esys::serialize( output + offset, messageLength );
    
    ::std::memcpy( output + offset, m_message, messageLength );
    offset += messageLength;
    
    return offset;
}


void Payload::deserialize( uint8_t* input )
{
    input += ::esys::deserialize( input, m_timestamp );
    
    m_level = TraceLevelInfo( char( *input ) ).level();
    input += sizeof( char );
    
    uint16_t messageLength = 0U;
    input += ::esys::deserialize( input, messageLength );
    
    if ( messageLength <= LOG_MESSAGE_SIZE_MAX )
    {    
        ::std::memcpy( m_message, input, messageLength );
        m_message[ messageLength ] = '\0';
    }
}


bool Payload::deserialize( ::std::istream& stream )
{
    bool result = false;
    
    if ( ::readAndDeserialize( m_message, m_timestamp, stream ) )
    {
        stream.read( m_message, sizeof( char ) );
        
        if ( sizeof( char ) ==  stream.gcount() )
        {
            m_level = TraceLevelInfo( m_message[ 0 ] ).level();
            
            uint16_t messageLength = 0U;

            if ( ::readAndDeserialize( m_message, messageLength, stream ) )
            {    
                if ( messageLength <= LOG_MESSAGE_SIZE_MAX )
                {
                    stream.read( m_message, messageLength );
                    m_message[ messageLength ] = '\0';
                    result = messageLength == stream.gcount();
                }
            }            
        }          
    }     
    
    return result;
}

}
}
