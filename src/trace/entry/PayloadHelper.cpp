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
 * File:   PayloadHelper.cpp
 * Author: kret
 * 
 * Created on February 17, 2016, 8:28 PM
 */

#include "trace/entry/PayloadHelper.h"
#include "trace/entry/Payload.h"
#include "trace/entry/TraceLevelInfo.h"
#include "esys/utils.h"

namespace trace
{
namespace entry
{

PayloadHelper::PayloadHelper( const Payload& payload ) 
    : m_payload( payload )
{
}


size_t PayloadHelper::serializeHeader( uint8_t* buffer ) const
{
    uint16_t msgLength = messageLength();
    size_t offset = ::esys::serialize( buffer, m_payload.m_timestamp );
    
    *( buffer + offset ) = TraceLevelInfo( m_payload.m_level ).charId();
    ++offset;
    
    offset += ::esys::serialize( buffer + offset, msgLength );
    return msgLength;
}


size_t PayloadHelper::messageLength() const
{
    return m_payload.m_length;
}


const char* PayloadHelper::message() const
{
    return m_payload.m_message;
}


size_t PayloadHelper::getHeaderLength()
{
    return sizeof( ::trace::entry::Payload::m_timestamp ) + sizeof( char ) + sizeof( uint16_t );
}

}
}
