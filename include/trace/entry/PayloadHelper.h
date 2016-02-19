/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PayloadHelper.h
 * Author: kret
 *
 * Created on February 17, 2016, 8:28 PM
 */

#ifndef PAYLOADHELPER_H
#define PAYLOADHELPER_H

#include <cstddef>
#include <cstdint>

namespace trace
{
    namespace entry
    {
        class Payload;
        
        class PayloadHelper 
        {
        public:
            PayloadHelper( const Payload& payload );
            
            /**
             * Serialize the Payload header into the buffer
             * @param buffer
             * @return Following message text length
             */
            size_t serializeHeader( uint8_t* buffer ) const;
            
            /**
             * Get the text message length
             * @return 
             */
            inline size_t messageLength() const;
            
            
            /**
             * Expose the text message
             * @return 
             */
            const char* message() const;
            
            
            static size_t getHeaderLength();
            
        private:
            const Payload& m_payload;
        };        
    }
}

#endif /* PAYLOADHELPER_H */

