/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
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
            void deSerialize( uint8_t* input );
            
            /**
             * Convert the TraceLevel to character
             * @param level
             * @return character representation
             */
            static char traceLevelToChar( const ::trace::LogLevel level );
        };        
    }
}

#endif /* PAYLOAD_H */

