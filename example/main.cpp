#include "app/SimpleApp.h"
#include "sys/AbstractThread.h"
#include "sys/StopWatch.h"
#include "trace/log.h"
#include "trace/UdpBackEnd.h"

//#define PERFORMANCE_TEST 1

namespace
{
    class Th1 : public ::sys::AbstractThread
    {
    public:
        explicit Th1( const uint32_t id )
            : ::sys::AbstractThread( ::std::to_string( id ).c_str() )
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
                sleep( 5 + ( m_char * 2U ) );
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
        ::printf( "TraceLog Demo. Press ctrl+c to exit.\n" );
#ifndef PERFORMANCE_TEST
        const size_t NUM_TH = 26U;
                
        Th1* threads[ NUM_TH ];
        
        for ( size_t i = 0U; i < NUM_TH; i++ )
        {
            threads[ i ] = new Th1( i );
            threads[ i ]->start();      
        }        
        
        if ( waitForExit( 15000U ) )
        {
            LOG_INFO_C( "Example running until end" );
            ::printf( "\nReceived BREAK signal...\n" );            
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
#else        
        /* Run the benchmark */
        LOG_INFO_C( "Start The Benchmark" );
        ::sys::StopWatch sw( true );

        for ( uint32_t i = 0; i < 100000; i++ )
        {
            LOG_INFO_C("TEST: and some text here :-)");                   
        }
        
        LOG_INFO_C( "Took: %u ms", sw.stop() / 1000U );        
#endif // PERFORMANCE_TEST  
        
        ::printf( "Exit\n" );
        
        return 0;
    }
};


int32_t main( int argc, const char * const * argv )
{
    LOGGER_INIT_BE_UDP();
    
    LOG_INFO_C( "App Started: %c", 'a' );
    int32_t result = ::DemoApp().run( argc, argv );
    LOG_INFO_C( "App Exited: %d" , result );
    LOG_INFO_C( "- bye! -" );
    
    LOGGER_SHUTDOWN();
    
    return result;
}
