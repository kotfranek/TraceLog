#include "log/log.h"
#include "log/Logger.h"
#include "log/FileBackEnd.h"
#include "log/ConsoleBackEnd.h"

namespace
{
    /* Logger instance */
    ::log::Logger S_LOGGER;
    
    /* Console backend instance */
    ::log::ConsoleBackEnd S_BE_CONSOLE;
        
    /* File backend instance */
    ::log::FileBackEnd S_BE_FILE;    
}

namespace log
{
    Logger& logInstance()
    {
        return S_LOGGER;
    }
    
    
    ILogBackEnd& logBackend( const LogBackend backEnd )
    {
        switch( backEnd )
        {
            case LogBackend_Console:
                return S_BE_CONSOLE;
                
            case LogBackend_File:
                return S_BE_FILE;
                
            default:
                return S_BE_CONSOLE;
        }
        
        return S_BE_CONSOLE;
    }
};