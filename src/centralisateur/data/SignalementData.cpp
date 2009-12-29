#include <string>
using namespace std;

#include "SDL_net.h"

#include "centralisateur/UdpConnector.h"
#include "centralisateur/UdpController.h"

#include "centralisateur/data/SignalementData.h"

SignalementData::SignalementData(void)
{
}

SignalementData::~SignalementData(void)
{
}

void SignalementData::setUserName(string& userName)
{
    m_userName = userName;
}

UDPpacket* SignalementData::toPacket(void)
{
    int taille =  4     // Code signalement
                + 4     // Taille du nom d'utilisateur
                + (int)m_userName.length();  // Nom d'utilisateur

    Uint8* data = new Uint8[taille];
    SDLNet_Write32((Uint32)UdpController::CODE_Signalement, data);
    SDLNet_Write32((Uint32)m_userName.length(), data+4);
    strcpy((char*)&data[8], m_userName.c_str());

    UDPpacket *packet = new UDPpacket();
    packet->data = data;
    packet->len = taille;

    return packet;
}
