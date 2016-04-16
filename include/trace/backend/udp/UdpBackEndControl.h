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
            const char* UDP_CLIENT_HANDSHAKE = "TL_UDP_CL_HS";  

            /* UDP Client Disconnect */
            const char* UDP_CLIENT_CLOSE = "TL_UDP_CL_CLOSE";

            /* UDP Client Disconnect */
            const char* UDP_CLIENT_ID = "TL_UDP_CL_ID";      
            
            /* UDP Client Present (CURRENTLY NOT USED) */
            const char* UDP_CLIENT_PRESENT = "TL_UDP_CL_PR";
            
            /* UDP Server Log Entry prefix */
            const char* UDP_SERVER_LOG_ENTRY = "TL_LE";

            /* UDP Server identification */
            const char* UDP_SERVER_HANDSHAKE = "TL_UDP_SRV_HS";    

            /* UDP Server Heartbeat signal */
            const char* UDP_SERVER_HEARTBEAT = "TL_UDP_SRV_HB";

            /* UDP Client Disconnect */
            const char* UDP_SERVER_CLOSE = "TL_UDP_SRV_CLOSE";                
        }
    }
};



#endif /* UDPBACKENDNETINTERFACE_H */

