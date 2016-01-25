/*
 * Copyright 2016 Przemyslaw Podwapinski <p.podwapinski@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef ILOGBACKEND_H
#define ILOGBACKEND_H

#include <string>

namespace log
{
    class LogEntry;
    
    
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
        virtual bool add( const ::log::LogEntry& entry ) = 0;
        
        
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
        virtual const ::std::string& getName() const = 0;
        
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
