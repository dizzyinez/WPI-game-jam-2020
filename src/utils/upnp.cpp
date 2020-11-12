
// #include <miniupnpc.h>
// #include <upnpcommands.h>
// #include "portlistingparse.h"
// #include "upnpreplyparse.h"
// #include <iostream>

// struct upnp
// {
//         inline static bool upnp_initialized = false;
//         inline static int error = 0;
//         inline static UPNPDev *upnp_dev = nullptr;

//         inline static char lan_address[64];
//         inline static struct UPNPUrls upnp_urls;
//         inline static struct IGDdatas upnp_data;
//         inline static char wan_address[64];


//         static bool upnp_setup()
//         {
//                 UPNPDev *upnp_dev = upnpDiscoverAll(
//                         2000,         // time to wait (milliseconds)
//                         nullptr,         // multicast interface (or null defaults to 239.255.255.250)
//                         nullptr,         // path to minissdpd socket (or null defaults to /var/run/minissdpd.sock)
//                         0,         // source port to use (or zero defaults to port 1900)
//                         0,         // 0==IPv4, 1==IPv6
//                         128,         //TTL
//                         &error);         // error condition

//                 if (error != 0)
//                 {
//                         std::cout << "upnp error code: " << error << std::endl;
//                         return false;
//                 }

//                 int status = UPNP_GetValidIGD(upnp_dev, &upnp_urls, &upnp_data, lan_address, sizeof(lan_address));
//                 // look up possible "status" values, the number "1" indicates a valid IGD was found
//                 std::cout << "lan status: " << status << std::endl;

//                 // get the external (WAN) IP address
//                 UPNP_GetExternalIPAddress(upnp_urls.controlURL, upnp_data.first.servicetype, wan_address);
//                 return true;
//         }

//         static bool upnp_open(int port)
//         {
//                 if (!upnp_dev)
//                         if (!upnp_setup())
//                                 return false;


//                 error = UPNP_AddPortMapping(
//                         upnp_urls.controlURL,
//                         upnp_data.first.servicetype,
//                         "port",         // external (WAN) port requested
//                         "port",         // internal (LAN) port to which packets will be redirected
//                         lan_address,         // internal (LAN) address to which packets will be redirected
//                         "Train Game Server",         // text description to indicate why or who is responsible for the port mapping
//                         "UDP",         // protocol must be either TCP or UDP
//                         nullptr,         // remote (peer) host address or nullptr for no restriction
//                         "0");         // port map lease duration (in seconds) or zero for "as long as possible"

//                 if (error != 0)
//                 {
//                         std::cout << "upnp error code: " << error << std::endl;
//                         return false;
//                 }
//                 return true;
//         }

//         static bool upnp_close(int port)
//         {
//                 if (!upnp_dev)
//                         if (!upnp_setup())
//                                 return false;

//                 error = UPNP_DeletePortMapping(
//                         upnp_urls.controlURL,
//                         upnp_data.first.servicetype,
//                         "6743",
//                         "UDP",
//                         nullptr
//                         );
//                 if (error != 0)
//                         std::cout << "upnp error code: " << error << std::endl;
//         }

//         static char* get_wan_address()
//         {
//                 return wan_address;
//         }
// };