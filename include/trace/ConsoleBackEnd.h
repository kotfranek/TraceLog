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

#ifndef CONSOLEBACKEND_H
#define CONSOLEBACKEND_H

#include "trace/backend/ILogBackEnd.h"

namespace trace
{

    /**
     * Default LogBackend implementation.
     * Output goes to the System Console.
     * Uses the cout and cerr streams depending on the LogEntry level.
     *  
     */
    class ConsoleBackEnd : public backend::ILogBackEnd
    {
    public:
        ConsoleBackEnd();
        ~ConsoleBackEnd()
        {
        };
        
    private:
        
        /**
        * @see ILogBackEnd
        */
        virtual bool add( const entry::LogEntry& entry );

        /**
        * @see ILogBackEnd
        */
        virtual void onRegister( const ::sys::TPid pid );
        
        /**
        * @see ILogBackEnd
        */    
        virtual void onShutdown();
        
        /**
        * @see ILogBackEnd
        */      
        virtual const ::esys::TString31& getName() const;    
    };

};

#endif // CONSOLEBACKEND_H
