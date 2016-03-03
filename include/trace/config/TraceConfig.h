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
 * File:   TraceConfig.h
 * Author: kret
 *
 * Created on March 1, 2016, 10:27 PM
 */

#ifndef TRACECONFIG_H
#define TRACECONFIG_H

#include "trace/config/IConfig.h"
#include "trace/config/IConfigProvider.h"

namespace trace
{    
    namespace config
    {
        class TraceConfig : public IConfig
        {
            class Provider : public IConfigProvider
            {
            public:
                Provider();
                
                /**
                 * @see IConfigProvider 
                 */
                virtual ::trace::backend::ILogBackEnd* getBackend()
                {
                    return m_backend;
                }
                
                /**
                 * @see IConfigProvider 
                 */
                virtual ::net::TPort getUdpPort() const
                {
                    return m_port;
                }
                
                
                /* Backend instance */
                ::trace::backend::ILogBackEnd* m_backend;                
                
                /* Server UDP Port */
                ::net::TPort m_port;
            };
            
        public:
            TraceConfig();
            
            /**
             * @see IConfig
             */
            virtual void setBackend()
            {
                m_provider.m_backend = NULL;
            }

            
            /**
             * @see IConfig
             */
            virtual void setUdpPort(const ::net::TPort port)
            {
                m_provider.m_port = port;
            }
            
            
            /**
             * Expose the configuration data
             * @return 
             */
            IConfigProvider& provider()
            {
                return m_provider;
            }
            
        private:
            /* Configuration data container */
            Provider m_provider;
        };
    } // namespace config
}; // namespace trace

#endif /* TRACECONFIG_H */

