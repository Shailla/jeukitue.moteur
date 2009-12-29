#include "centralisateur/UdpConnector.h"
#include "centralisateur/data/SignalementData.h"

#include "centralisateur/UdpController.h"


int UdpController::CODE_Signalement = 100;
int UdpController::CODE_List = 101;
int UdpController::CODE_PingRequest = 104;
int UdpController::CODE_PingAck = 105;

UdpController::UdpController(void)
{
    m_udpConnector = 0;
}

UdpController::~UdpController(void)
{
}

void UdpController::connect(char* userName,
                            int portLocal,
                            char* ipCentralisateur,
                            int portCentralisateur)
{
    if(m_udpConnector) {
        m_udpConnector->stop();  // Kill the actual connector and wait he's dead
        delete m_udpConnector;
    }

    m_udpConnector = new UdpConnector(this, portLocal);
    m_udpConnector->connect(ipCentralisateur, portCentralisateur);
}

void UdpController::sendSignalement(string& userName)
{
    SignalementData data;
    data.setUserName(userName);
    UDPpacket* packet = data.toPacket();
    m_udpConnector->send(packet);
}

void UdpController::receive(UDPpacket* packet)
{
    Uint8* data = packet->data;
    Uint32 code = SDLNet_Read32(data);

    // Respond to an Ping request
    if(code == CODE_PingRequest) {
        SDLNet_Write32((Uint32)UdpController::CODE_PingAck, data);
        m_udpConnector->send(packet);
    }

    // Receive the list of connected users
    else if(code == CODE_List) {

    }

    // Unknown code
    else {
    }
}
