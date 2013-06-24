/*
 * S2CConnectionAcceptedTechnicalMessage.cpp
 *
 *  Created on: 23 juin 2013
 *      Author: vgdj7997
 */

#include "SDL.h"
#include "SDL_net.h"

#include "reseau/new/message/S2CConnectionAcceptedTechnicalMessage.h"

S2CConnectionAcceptedTechnicalMessage::S2CConnectionAcceptedTechnicalMessage(Uint16 port) : TechnicalMessage(S2C_CONNECTION_ACCEPTED) {
	_port = port;
}

S2CConnectionAcceptedTechnicalMessage::~S2CConnectionAcceptedTechnicalMessage() {
}

int S2CConnectionAcceptedTechnicalMessage::getPort() const {
	return _port;
}

char* S2CConnectionAcceptedTechnicalMessage::toBytes() const {
	char* msg = new char[4];
	SDLNet_Write16(getCode(), msg);
	SDLNet_Write16(_port, msg);

	return msg;
}
