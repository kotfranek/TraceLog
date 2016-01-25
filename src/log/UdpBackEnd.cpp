/*
 * Copyright (c) 2016, <copyright holder> <email>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY <copyright holder> <email> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> <email> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "log/UdpBackEnd.h"
#include "log/LogEntry.h"

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <iostream>

namespace
{
    /* Backend name */
    const ::std::string LOG_BACKEND_NAME( "UDP" ); 
    
    /* Invalid Socket Id */
    const int32_t SOCKET_INVALID = -1;
}

namespace log
{

UdpBackEnd::UdpBackEnd()
    : ILogBackEnd()
    , m_socketId( SOCKET_INVALID )
    , m_info( NULL )
{

}


void UdpBackEnd::onRegister()
{
    const char* hostname=0; /* localhost */
    const char* portname="6871";
    struct addrinfo hints;
    memset(&hints,0,sizeof(hints));
    hints.ai_family=0;
    hints.ai_socktype=SOCK_DGRAM;
    hints.ai_protocol=0;
    hints.ai_flags=AI_PASSIVE | AI_ADDRCONFIG;

    int err=getaddrinfo(hostname,portname,&hints,&m_info);
    if (err!=0) {
        ::std::cout << "ERROR: " << err << ::std::endl;
    }
    else
    {
        int fd=socket(m_info->ai_family,m_info->ai_socktype,m_info->ai_protocol);
        if (fd==-1) 
        {
           ::std::cout << "ERROR: " << strerror(errno) << ::std::endl;
        }  
        else
        {
            m_socketId = fd;
            send( "onRegister" );
        }
    }        
}


bool UdpBackEnd::add( const LogEntry& entry )
{    
    return send( entry.toString() );
}


bool UdpBackEnd::send( const std::string& text )
{
    bool result = false;
    
    if ( ::SOCKET_INVALID != m_socketId )
    {        
        if (sendto(m_socketId,text.c_str(),text.length(),0,
            m_info->ai_addr,m_info->ai_addrlen)==-1) 
        {
            ::std::cout << "ERROR: " << strerror(errno) << ::std::endl;
        }     
        else
        {
            result = true;
        }        
    }
    
    return result;
}



void UdpBackEnd::onShutdown()
{
    if ( ::SOCKET_INVALID != m_socketId )
    {
        send( "onShutdown" );
        ::close( m_socketId );
    }
}


const ::std::string& UdpBackEnd::getName() const
{
    return ::LOG_BACKEND_NAME;
}


UdpBackEnd::~UdpBackEnd()
{

}

}; // namespace log
