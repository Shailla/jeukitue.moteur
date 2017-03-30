#include <string>

#include "SDL_net.h"

#include "centralisateur/UdpConnector.h"
#include "centralisateur/UdpController.h"

#include "centralisateur/data/SignalementData.h"

using namespace std;

SignalementData::SignalementData(void)
{
}

SignalementData::~SignalementData(void)
{
}

void SignalementData::setUserName(const string& userName)
{
    m_userName = userName;
}

UDPpacket* SignalementData::toPacket(void)
{
    int taille =  CODE_DATA_SIZE     // Code signalement
                + 4     // Taille du nom d'utilisateur
                + (int)m_userName.length();  // Nom d'utilisateur

    Uint8* data = new Uint8[taille];
	Uint8* current = data;

	// Code
    SDLNet_Write32((Uint32)UdpController::CODE_U_Signalement, data);
	current += CODE_DATA_SIZE;
	
	// Taille du nom d'utilisateur
	SDLNet_Write32((Uint32)m_userName.length(), current);
	current += 4;

	// Nom d'utilisateur
    strcpy((char*)current, m_userName.c_str());

    UDPpacket *packet = new UDPpacket();
    packet->data = data;
    packet->len = taille;

    return packet;
}
