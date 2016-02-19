/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UdpBackEndNetInterface.h
 * Author: kret
 *
 * Created on February 19, 2016, 3:17 PM
 */

#ifndef UDPBACKENDNETINTERFACE_H
#define UDPBACKENDNETINTERFACE_H

#include <stdint.h>
#include <string>

namespace trace
{
    namespace backend
    {
        namespace udp
        {
            /* Presence signal period (us) */
            const int64_t UDP_SERVER_HEARTBEAT_PERIOD = 500U * 1000U;
            
            /* Maximum time between the Presence Signal (us) */
            const int64_t UDP_SERVER_HEARTBEAT_TIMEOUT = 1000U * 1000U; 
                                
            /* UDP Client identification */
            const ::std::string UDP_CLIENT_HANDSHAKE( "TRACELOG_UDP_CLIENT_HS" );  

            /* UDP Client Disconnect */
            const ::std::string UDP_CLIENT_CLOSE( "TRACELOG_UDP_CLIENT_CLOSE" );

            /* UDP Client Disconnect */
            const ::std::string UDP_CLIENT_ID( "TRACELOG_UDP_CLIENT_ID" );      
            
            /* UDP Client Present (CURRENTLY NOT USED) */
            const ::std::string UDP_CLIENT_PRESENT( "TRACELOG-CLIENT-PRESENT" );                         

            /* UDP Server identification */
            const ::std::string UDP_SERVER_HANDSHAKE( "TRACELOG_UDP_SRV_HS" );    

            /* UDP Server Heartbeat signal */
            const ::std::string UDP_SERVER_HEARTBEAT( "TRACELOG_UDP_SRV_HB" );

            /* UDP Client Disconnect */
            const ::std::string UDP_SERVER_CLOSE( "TRACELOG_UDP_SRV_CLOSE" );                
        }
    }
};



#endif /* UDPBACKENDNETINTERFACE_H */

