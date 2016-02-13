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

#ifndef UDPCLIENTMEDIATOR_H
#define UDPCLIENTMEDIATOR_H

#include <mutex>
#include "sys/AbstractThread.h"

namespace net
{
    class UdpSocket;
    class Datagram;
}

namespace trace
{

    class UdpClientMediator : public ::sys::AbstractThread
    {
    public:
        enum MediatorState
        {
            Mediator_WaitForClientId,
            Mediator_Connected,
            Mediator_Disconnected,
        };
        
        explicit UdpClientMediator( ::net::UdpSocket& socket );
        virtual ~UdpClientMediator();
        
        /**
         * Send if possible 
         * @arg datagram
         */
        bool send( const ::net::Datagram& datagram );
        
    private:
        /* Prohibit copying */
        UdpClientMediator& operator=(const UdpClientMediator& other);        
        UdpClientMediator(const UdpClientMediator& other);
        
        /**
        * @see ::sys::AbstractThread
        */
        virtual void run();
        
        /** 
         * Try to read a single UDP datagram and check if it originates 
         * from the Trace Client Application 
         * @result true on success
         */
        bool waitForClient( ::net::Datagram& auxiliary, const ::std::string& content );
        
        
        /**
         * Receive text from the client
         * @param auxiliary
         * @param content
         * @return 
         */
        bool receiveStringFromClient( ::net::Datagram& auxiliary, ::std::string& content );
        
        
        /**
         * Send the given text content to the connected client
         * @param auxiliary
         * @param content
         */
        bool sendStringToClient( const ::std::string& content );
        
        
        /**
         * Set the new state
         * @param state
         */
        void setState( MediatorState state );
        
        /**
         * Get the current state
         * @return 
         */
        MediatorState getState();
        
        
        /**
         * Check the current state
         * @param state
         * @return 
         */
        bool isState( const MediatorState state );
                
        /* Socket instance */
        ::net::UdpSocket& m_socket;
        
        /* Internal state */
        MediatorState m_state;
        
        /* Connected Client Id */
        ::std::string m_clientId;
        
        /* Socket access mutex */
        ::std::mutex m_udpMutex;        
    };

}; // namespace trace

#endif // UDPCLIENTMEDIATOR_H
