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

#include "log/UdpClientMediator.h"
#include "net/UdpSocket.h"
#include "net/Datagram.h"

#include <iostream>

namespace
{
    /* UDP Client identification */
    const ::std::string UDP_CLIENT_HANDSHAKE( "TRACELOG-CLIENT-HELLO" );  
    
    /* UDP Client Disconnect */
    const ::std::string UDP_CLIENT_CLOSE( "TRACELOG-CLIENT-CLOSE\n" );     
    
    /* UDP Client Present */
    const ::std::string UDP_CLIENT_PRESENT( "TRACELOG-CLIENT-PRESENT\n" );     
    
    /* UDP Server identification */
    const ::std::string UDP_SERVER_ID( "TRACELOG-SRV-HELLO" );
    
    /* Maximum time for the Client's presence signal */
    const uint64_t UDP_CLIENT_TIMEOUT_MS = 1000U;
}

namespace trace
{

UdpClientMediator::UdpClientMediator( ::net::UdpSocket& socket )
    : ::sys::AbstractThread( "UdpClientMediator" )
    , m_socket( socket )
    , m_state( Mediator_Disconnected )
{

}


void UdpClientMediator::run()
{
    ::net::Datagram fromClient;
    
    while( !isStopRequested() )
    {
        switch ( m_state )
        {
            case Mediator_Disconnected:
            {
                m_state = Mediator_Waiting;
                break;
            }
            
            case Mediator_Waiting:
            {
                if ( waitForClient( fromClient, ::UDP_CLIENT_HANDSHAKE ) )
                {
                    const ::net::Address& addr = fromClient.getAddress();

                    if ( m_socket.connect( addr ) )
                    {
                        ::std::cout << "CONNECTED..." << ::std::endl;   
                    }
                    
                    ::net::Datagram s( addr );
                    s.setContent( ::UDP_SERVER_ID );                    
                    m_socket.send( s );             
                    
                    m_state = Mediator_Connected;
                } 
                break;                
            }
            
            case Mediator_Connected:
            {
                // TODO: Check the time, that passed from the last Client Present message
                if ( waitForClient( fromClient, ::UDP_CLIENT_PRESENT ) )
                {
                    // Client present message received. Store the timestamp.
                }
                
                if ( waitForClient( fromClient, ::UDP_CLIENT_CLOSE ) )
                {
                    m_state = Mediator_Disconnected;
                }
                break;
            }
            
            default:
                break;
        }   
    }         
}


bool UdpClientMediator::waitForClient( ::net::Datagram& auxiliary, const ::std::string& handshake )
{
    bool result = false;
    ::std::cout << "waitForClient: " << handshake << ::std::endl;     
    if ( m_socket.receive( auxiliary ) )
    {
        ::std::cout << "received: " << handshake << ::std::endl;        
        ::std::string handshake;
        auxiliary.toString( handshake );
        
        result = 0 == handshake.compare( handshake );
    }
    
    return result;
}



bool UdpClientMediator::send( const net::Datagram& datagram )
{
    bool result = false;
    
    if ( m_state == Mediator_Connected )
    {
        result = m_socket.send( datagram );
    }
    
    return result;
}



UdpClientMediator::~UdpClientMediator()
{

}

}; // namespace trace
