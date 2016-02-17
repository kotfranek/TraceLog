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


/* 
 * File:   Payload.h
 * Author: kret
 *
 * Created on February 8, 2016, 10:54 PM
 */

#ifndef PAYLOAD_H
#define PAYLOAD_H

#include "trace/LogDefines.h"
#include <istream>

namespace trace
{
    namespace entry
    {
        class Payload 
        {
        public:
            Payload();

            /* Timestamp: ms since epoch */
            uint64_t m_timestamp;
            
            /* Trace level */
            LogLevel m_level;
            
            /* Message content */
            char m_message[ LOG_MESSAGE_SIZE_MAX ]; 
            
            /**
             * Serialize the payload 
             * @arg output buffer
             * @return number of bytes written
             */
            size_t serialize( uint8_t* output ) const;
            
            
            /**
             * Fill the payload from serialized buffer
             * @param input
             */
            void deserialize( uint8_t* input );
            
            
            bool deserialize( ::std::istream& stream );
        };        
    }
}

#endif /* PAYLOAD_H */

