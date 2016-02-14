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

#ifndef ILOGBACKEND_H
#define ILOGBACKEND_H

#include "esys/AutoString.h"

namespace trace
{       
    namespace entry
    {
        class LogEntry;
    }    
    /**
    * Log BackEnd Interface 
    * 
    * Implement this to process and store the single LogEntry.
    * It is guaranteed for the class to be called synchronously.
    * 
    * Unless the internal implementation requires it, there is no need for 
    * access synchronisation.
    */
    class ILogBackEnd
    {
    public:
        /**
        * Add entry. Thread safe
        * @arg entry
        * @result true, on successful entry inhibit
        */
        virtual bool add( const entry::LogEntry& entry ) = 0;
        
        
        /**
         * Called on BackEnd register 
         */
        virtual void onRegister() = 0;
        
        
        /**
         * Called on Logger shutdown/destruction
         */
        virtual void onShutdown() = 0;
        
        
        /**
         * Get the BackEnd's name
         * @result Name-String reference
         */
        virtual const ::esys::TString31& getName() const = 0;
        
    protected:
            
        ILogBackEnd()
        {
        };
        
        virtual ~ILogBackEnd()
        {
        };        
    };

};

#endif // ILOGBACKEND_H
