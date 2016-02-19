/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
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
