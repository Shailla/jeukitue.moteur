#ifndef __JKT__UDPCONTROLLER_H
#define __JKT__UDPCONTROLLER_H

#include <string>
using namespace std;

#include "SDL_net.h"

class UdpConnector;

class UdpController
{
    UdpConnector* m_udpConnector;
public:
    static int CODE_Signalement;
    static int CODE_PingRequest;
    static int CODE_PingAck;
    static int CODE_List;

    UdpController(void);
    ~UdpController(void);

    void connect(char* userName, int portLocal, char* ipCentralisateur, int portCentralisateur);
    void sendSignalement(string& userName);
    void receive(UDPpacket* packet);
};

#endif
