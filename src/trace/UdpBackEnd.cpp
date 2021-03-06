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

#include "trace/UdpBackEnd.h"
#include "trace/entry/LogEntry.h"
#include "net/NetTypes.h"

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
    const ::esys::TString31 LOG_BACKEND_NAME( "UDP" ); 
    
    /* UDP Wait-For-Receive Timeout in ms */
    const uint32_t UDP_RECIEVE_TIMEOUT_MS = 1U;
    
    /* UDP Wait-For-Send Timeout in ms */
    const uint32_t UDP_SEND_TIMEOUT_MS = 5U;    
    
    /* Server Receive Port */
    const ::net::TPort UDP_LISTEN_PORT = 55555U;
}

namespace trace
{

UdpBackEnd::UdpBackEnd()
    : ILogBackEnd()
    , m_socket()
    , m_mediator( m_socket )
{

}


void UdpBackEnd::onRegister( const ::sys::TPid pid )
{         
    if ( m_socket.open( UDP_LISTEN_PORT ) )
    {        
        m_socket.setTimeouts( UDP_SEND_TIMEOUT_MS, UDP_RECIEVE_TIMEOUT_MS );       
        m_mediator.start();                      
    }       
}


bool UdpBackEnd::add( const entry::LogEntry& entry )
{    
    return m_mediator.send( entry );
}


void UdpBackEnd::onShutdown()
{   
    m_mediator.requestStop();
    m_mediator.join();
    
    m_socket.close();
}


const ::esys::TString31& UdpBackEnd::getName() const
{
    return ::LOG_BACKEND_NAME;
}


UdpBackEnd::~UdpBackEnd()
{
    
}

}; // namespace trace
