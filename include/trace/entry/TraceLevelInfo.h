/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TraceLevelInfo.h
 * Author: kret
 *
 * Created on February 12, 2016, 11:09 PM
 */

#ifndef TRACELEVELHELPER_H
#define TRACELEVELHELPER_H

#include "trace/LogDefines.h"

namespace trace
{
    namespace entry
    {
        class TraceLevelInfo 
        {
        public:
            /* Construct helper from actual level */
            TraceLevelInfo( const ::trace::LogLevel traceLevel );
            
            /* Construct helper from character representation */
            TraceLevelInfo( const char traceLevel );   
            
            /* Get the Character representation */
            inline char charId() const
            {
                return m_char;
            }
            
            /* Get the actual level */
            inline ::trace::LogLevel level() const
            {
                return m_level;
            }
            
            /* Get the name */
            inline const char* name() const;
            
        private:            
            /* Log Level */
            const ::trace::LogLevel m_level;
            
            /* Character representation */
            const char m_char;
        };
    }
};

#endif /* TRACELEVELHELPER_H */

