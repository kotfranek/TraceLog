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

#include "trace/UdpClientMediator.h"
#include "trace/backend/udp/UdpBackEndControl.h"
#include "trace/entry/LogEntry.h"
#include "sys/SysTypes.h"
#include "sys/StopWatch.h"
#include "net/UdpSocket.h"
#include "net/Datagram.h"

#include <iostream>
#include <cstring>

namespace
{    
    /* Server identifier */
    const ::std::string UDP_SERVER_NAME( "UDP-TraceLog-Server-0.2.0" );
}

namespace trace
{

UdpClientMediator::UdpClientMediator( ::net::UdpSocket& socket )
    : ::sys::AbstractThread( "UdpClientMediator" )
    , m_socket( socket )
    , m_sendPayload()
    , m_state( Mediator_Disconnected )
    , m_clientId()
    , m_udpMutex()
{

}


UdpClientMediator::MediatorState UdpClientMediator::getState()
{
    return m_state;
}


void  UdpClientMediator::setState( MediatorState state )
{
    m_state = state;    
}


bool UdpClientMediator::isState( const MediatorState state )
{
    return state == getState();
}


void UdpClientMediator::run()
{
    ::net::Datagram fromClient;
    ::sys::StopWatch heartBeatTimer;
    
    /* Thread does stay idle for this time, not to block normal send operations */
    uint32_t sleepTime = 50U;
       
    while( !isStopRequested() )
    {   
        /* Don't let it block too often */
        sleep( sleepTime );
        
        ::sys::TLockMutex l( m_udpMutex );
                
        switch ( getState() )
        {
            case Mediator_Disconnected:
            {
                sleepTime = 50U;
                
                if ( waitForClient( fromClient, backend::udp::UDP_CLIENT_HANDSHAKE ) )
                {
                    const ::net::Address& addr = fromClient.getAddress();                    
                    
                    if ( m_socket.connect( addr ) )
                    {                       
                        if ( sendStringToClient( backend::udp::UDP_SERVER_HANDSHAKE + ::UDP_SERVER_NAME ) )
                        { 
                            setState( Mediator_WaitForClientId );
                        }
                    }                                        
                } 
                break;                  
            }
            
            case Mediator_WaitForClientId:
            {
                ::std::string clientId;
                
                if ( receiveStringFromClient( fromClient, clientId ) )
                {
                    if ( 0 == clientId.find( backend::udp::UDP_CLIENT_ID ) )
                    { 
                        m_clientId = clientId.substr( ::std::strlen( ::trace::backend::udp::UDP_CLIENT_ID ) );
                                                
                        if ( sendStringToClient( backend::udp::UDP_SERVER_HEARTBEAT ) )
                        {
                            sleepTime = 200U;
                            heartBeatTimer.start();
                            setState( Mediator_Connected );
                        }  
                        else
                        {
                            setState( Mediator_Disconnected );
                        }
                    }
                }
                break;
            }
            
            case Mediator_Connected:
            {
                if ( heartBeatTimer.elapsed( backend::udp::UDP_SERVER_HEARTBEAT_PERIOD ) )
                {                    
                    if ( sendStringToClient( backend::udp::UDP_SERVER_HEARTBEAT ) )
                    {
                        heartBeatTimer.reStart();
                    }
                    else
                    {
                        heartBeatTimer.stop();
                        setState( Mediator_Disconnected );
                    }
                }
                break;
            }
            
            default:
                break;
        }   
    }  
    
    /* Thread requested to stop. Notify the Client, if necessary */
    ::sys::TLockMutex l( m_udpMutex );
    if ( isState( Mediator_Connected ) )
    {
        sendStringToClient( backend::udp::UDP_SERVER_CLOSE );
    }
}


bool UdpClientMediator::sendStringToClient( const ::std::string& content )
{
    m_sendPayload.setContent( content );                    
    
    return m_socket.send( m_sendPayload ); 
}


bool UdpClientMediator::receiveStringFromClient( ::net::Datagram& auxiliary, ::std::string& content )
{
    bool result = false;
    
    if ( m_socket.receive( auxiliary ) )
    {      
        auxiliary.toString( content );
        result = true;        
    }    
    
    return result;
}


bool UdpClientMediator::waitForClient( ::net::Datagram& auxiliary, const ::std::string& content )
{
    bool result = false;
    ::std::string fromClient;
  
    if ( receiveStringFromClient( auxiliary, fromClient ) )
    {              
        result = 0 == content.compare( fromClient );
    }
    
    return result;
}



bool UdpClientMediator::send( const ::trace::entry::LogEntry& entry )
{
    bool result = false;
 
    ::sys::TLockMutex l( m_udpMutex );
    
    if ( isState( Mediator_Connected ) )
    {
        m_sendPayload.setContent( entry.toString() );
        
        if ( m_socket.send( m_sendPayload ) )
        {
            result = true;
        }
        else
        {
            setState( Mediator_Disconnected );
        }
    }
    
    return result;
}



UdpClientMediator::~UdpClientMediator()
{

}

}; // namespace trace
