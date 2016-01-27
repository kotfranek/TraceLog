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
#include "net/Datagram.h"

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
}

namespace trace
{

UdpBackEnd::UdpBackEnd()
    : ILogBackEnd()
    , m_socket()
{

}


void UdpBackEnd::onRegister()
{
    ::std::cout << "SOCK..." << ::std::endl;
    
    if ( m_socket.open( 6871 ) )
    {
        m_socket.setTimeouts( 5000U, 5000U );
        ::net::Datagram d;
        if ( m_socket.receive( d ) )
        {
            ::std::cout << "From: " << d.getAddress().getIp() << ":" << d.getAddress().getPort() << " " << d.getData()[ 0 ] << ::std::endl;
        }
        
        ::net::Datagram s( ::net::Address( 6872, "127.0.0.1" ) );
        m_socket.send( s );
    }       
}


bool UdpBackEnd::add( const LogEntry& entry )
{    
    return send( entry.toString() );
}


bool UdpBackEnd::send( const std::string& text )
{
    bool result = false;
    
    return result;
}



void UdpBackEnd::onShutdown()
{
    send( "onShutdown" );
}


const ::std::string& UdpBackEnd::getName() const
{
    return ::LOG_BACKEND_NAME;
}


UdpBackEnd::~UdpBackEnd()
{

}

}; // namespace trace
