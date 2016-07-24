/*
 * S2DisconnectionTechnicalMessage.cpp
 *
 *  Created on: 23 juin 2013
 *      Author: vgdj7997
 */

#include "SDL.h"
#include "SDL_net.h"

#include "reseau/new/message/S2CDisconnectionTechnicalMessage.h"

using namespace jkt;

S2DisconnectionTechnicalMessage::S2DisconnectionTechnicalMessage() : TechnicalMessage(S2C_DISCONNECTION) {
}

S2DisconnectionTechnicalMessage::~S2DisconnectionTechnicalMessage() {
}

jkt::Bytes* S2DisconnectionTechnicalMessage::toBytes() {
	char* msg = new char[2];
	SDLNet_Write16(getCode(), msg);

	return new jkt::Bytes(msg, 2);
}
