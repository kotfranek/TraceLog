#include "app/SimpleApp.h"
#include "sys/AbstractThread.h"
#include "sys/StopWatch.h"
#include "net/UdpSocket.h"
#include "net/Datagram.h"

#include <iostream>

namespace
{   
    /* Trace Server's port */
    const uint16_t TRACE_SRV_PORT = 55555;
    
    /* Maximum time between the Presence Signal (us) */
    const int64_t TRACE_SRV_PRESENCE_SIGNAL_TIME = 500000;
    
    class UdpClient : public ::sys::AbstractThread
    {
    public:
        explicit UdpClient( const ::std::string& ip )
            : ::sys::AbstractThread( "UdpClient" )
            , m_srvAddress( TRACE_SRV_PORT, ip )
            , m_socket()
            , m_timeStamp( 0 )
        {
        }
        
    private:        
        virtual void run()
        {
            if ( m_socket.open( 55557 ) )
            {
                m_socket.setTimeouts( 50U, 100U );
                
                ::std::cout << "Port opened" << ::std::endl;
                ::net::Datagram outMsg( m_srvAddress );
                ::net::Datagram inMsg;
                
                outMsg.setContent( "TRACELOG-CLIENT-HELLO" );
                m_socket.send( outMsg );
                
                ::sys::StopWatch sw( true );
                
                while ( !isStopRequested() )
                {
                    if ( m_socket.receive( inMsg ) )
                    {
                        ::std::string auxStr;
                        inMsg.toString( auxStr );
                        ::std::cout << auxStr << ::std::endl;
                    }
                    
                    const int64_t current = sw.elapsed();
                    
                    if ( ( current - m_timeStamp ) > TRACE_SRV_PRESENCE_SIGNAL_TIME )
                    {
                        m_timeStamp = current;
                        outMsg.setContent( "TRACELOG-CLIENT-PRESENT" );
                        m_socket.send( outMsg );
                    }
                }
            }
        }
        
        /* Trace Server Address */
        ::net::Address m_srvAddress;
        
        /* Client UDP Socket */
        ::net::UdpSocket m_socket;
        
        /* Client-Present signal timestamp */
        int64_t m_timeStamp;
    };
}



class UdpClientApp : public ::app::SimpleApp
{
public:
    UdpClientApp()
        : ::app::SimpleApp( "UdpClientApp" )
    {

    }
    
private:    
    virtual int32_t onRun( const TStringVector& args )
    {       
        if ( args.size() == 2U )
        {
            UdpClient client( args[ 1 ] );
            client.start();
            
            waitForSignal();
            
            client.requestStop();
            client.join();
        }
        else
        {
            ::std::cerr << "Missing Trace Server IP Address, eg. 127.0.0.1" << ::std::endl;
        }        
        
        return 0;
    }
};


int32_t main( int argc, const char * const * argv )
{
    int32_t result = ::UdpClientApp().run( argc, argv );
    
    return result;
}
