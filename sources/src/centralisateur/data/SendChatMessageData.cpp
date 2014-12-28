#include <string>
#include <iostream>
using namespace std;

#include "SDL_net.h"

#include "util/Trace.h"
#include "centralisateur/UdpConnector.h"
#include "centralisateur/UdpController.h"

#include "centralisateur/data/SendChatMessageData.h"

SendChatMessageData::SendChatMessageData(void)
{
}

SendChatMessageData::~SendChatMessageData(void)
{
}

void SendChatMessageData::setMessage(const string& message)
{
    _message = message;
}

void SendChatMessageData::setUserName(const string& userName)
{
    _userName = userName;
}

UDPpacket* SendChatMessageData::toPacket(void)
{
    const int taille =
				CODE_DATA_SIZE				// Code signalement
                + 4							// Taille du nom d'utilisateur
                + (int)_userName.length()	// Nom d'utilisateur
                + 4							// Taille du texte message
                + (int)_message.length();	// Texte du message

    Uint8* current = new Uint8[taille];
	Uint8* data = current;

	// Code
    SDLNet_Write32((Uint32)UdpController::CODE_U_ChatMessage, data);
    current += CODE_DATA_SIZE;

	// Taille du nom d'utilisateur
	SDLNet_Write32((Uint32)_userName.length(), current);
	current += 4;

	// Nom d'utilisateur
    strncpy((char*)current, _userName.c_str(), _userName.length());
	current += _userName.length();

	// Taille du texte du message
	SDLNet_Write32((Uint32)_message.length(), current);
    current += 4;

	// Texte du message
	strncpy((char*)current, _message.c_str(), _message.length());

	TRACE().info("Envoi message chat>'%s'", _message.c_str());

    UDPpacket *packet = new UDPpacket();
    packet->data = data;
    packet->len = taille;

    return packet;
}
