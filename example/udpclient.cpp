#include "app/SimpleApp.h"
#include "sys/AbstractThread.h"
#include "net/UdpSocket.h"
#include "net/Datagram.h"

#include <iostream>

namespace
{
    class UdpClient : public ::sys::AbstractThread
    {
    public:
        explicit UdpClient( const uint16_t port )
            : ::sys::AbstractThread( "UdpClient" )
            , m_socket()
        {
        }
        
    private:        
        virtual void run()
        {
            if ( m_socket.open( 55557 ) )
            {
                m_socket.setTimeouts( 50U, 100U );
                
                ::std::cout << "Port opened" << ::std::endl;
                ::net::Datagram auxiliary( ::net::Address( 55555, "127.0.0.1" ) );
                auxiliary.setContent( "TRACELOG-CLIENT-HELLO" );
                m_socket.send( auxiliary );
                
                while ( !isStopRequested() )
                {
                    if ( m_socket.receive( auxiliary ) )
                    {
                        ::std::string auxStr;
                        auxiliary.toString( auxStr );
                        ::std::cout << auxStr << ::std::endl;
                    }
                }
            }
        }
        
        /* Client UDP Socket */
        ::net::UdpSocket m_socket;
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
        UdpClient client( 55557 );
        client.start();
        
        waitForSignal();
        
        client.requestStop();
        client.join();
        
        return 0;
    }
};


int32_t main( int argc, const char * const * argv )
{
    int32_t result = ::UdpClientApp().run( argc, argv );
    
    return result;
}
