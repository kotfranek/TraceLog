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

#ifndef UDPBACKEND_H
#define UDPBACKEND_H

#include "ILogBackEnd.h"
#include "net/UdpSocket.h"

namespace trace
{

    class UdpBackEnd : public ILogBackEnd
    {
    public:
        UdpBackEnd();
        ~UdpBackEnd();
        
        /**
        * @see ILogBackEnd
        */
        virtual bool add( const LogEntry& entry );

        /**
        * @see ILogBackEnd
        */
        virtual void onRegister();
        
        /**
        * @see ILogBackEnd
        */    
        virtual void onShutdown();
        
        /**
        * @see ILogBackEnd
        */      
        virtual const ::std::string& getName() const;        
        
    private:
        UdpBackEnd& operator=(const UdpBackEnd& other); 
        UdpBackEnd(const UdpBackEnd& other);
        
        /**
         * Send text via the UDP Socket
         */
        bool send( const ::std::string& text );
        
        /* UDP Socket */
        ::net::UdpSocket m_socket;        
    };

}; // namespace trace

#endif // UDPBACKEND_H
