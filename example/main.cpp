#include "app/SimpleApp.h"
#include "sys/AbstractThread.h"
#include "sys/StopWatch.h"
#include "trace/log.h"
#include "trace/UdpBackEnd.h"

namespace
{
    class Th1 : public ::sys::AbstractThread
    {
    public:
        explicit Th1( const uint32_t id )
            : ::sys::AbstractThread( ::std::to_string( id ) )
            , m_char( 'A' + id )
        {
        }
        
    private:        
        virtual void run()
        {
            uint32_t i = 200;
            while ( i != 0U )
            {
                if ( isStopRequested() )
                {
                    LOG_DEV_C( "Thread '%c' requested to stop on i = %u", m_char, i );
                    break;
                }
                
                LOG_INFO_C( "TH: %c, i = %u", m_char, i );
                sleep( 50 + ( m_char * 2U ) );
                --i;
            }
        }
        
        const char m_char;
    };
}



class DemoApp : public ::app::SimpleApp
{
public:
    DemoApp()
        : ::app::SimpleApp( "TraceLogDemo" )
    {

    }
    
private:    
    virtual int32_t onRun( const TStringVector& args )
    {       
        const size_t NUM_TH = 26U;
        
        ::sys::StopWatch sw( true );
        
        Th1* threads[ NUM_TH ];
        
        for ( size_t i = 0U; i < NUM_TH; i++ )
        {
            threads[ i ] = new Th1( i );
            threads[ i ]->start();      
        }
        
        //LOG_ASSERT( false, "poop!" );
        
        if ( waitForExit( 15000U ) )
        {
            LOG_INFO_C( "Example running until end" );
        }
        else
        {
            LOG_WARN_C( "Example Timeout" );
        }
        
        for ( size_t i = 0U; i < NUM_TH; i++ )
        {
            threads[ i ]->requestStop();            
            threads[ i ]->join();
            
            delete threads[ i ];
        }
        
        LOG_INFO_C( "Took: %u", sw.stop() );
        
        return 0;
    }
};


int32_t main( int argc, const char * const * argv )
{
    LOGGER_INIT_BE_FILE;
    
    LOG_INFO_C( "App Started: %c", 'a' );
    int32_t result = ::DemoApp().run( argc, argv );
    LOG_INFO_C( "App Exited: %d" , result );
    LOG_INFO_C( "- bye! -" );
    
    LOGGER_SHUTDOWN();
    
    return result;
}
