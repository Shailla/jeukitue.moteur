/*
 * S2CConnectionRefusedTechnicalMessage.cpp
 *
 *  Created on: 23 juin 2013
 *      Author: vgdj7997
 */

#include "SDL.h"
#include "SDL_net.h"

#include "reseau/new/message/S2CConnectionRefusedTechnicalMessage.h"

using namespace jkt;

S2CConnectionRefusedTechnicalMessage::S2CConnectionRefusedTechnicalMessage() : TechnicalMessage(S2C_CONNECTION_REFUSED) {
}

S2CConnectionRefusedTechnicalMessage::~S2CConnectionRefusedTechnicalMessage() {
}

jkt::Bytes* S2CConnectionRefusedTechnicalMessage::toBytes() {
	char* msg = new char[2];
	SDLNet_Write16(getCode(), msg);

	return new jkt::Bytes(msg, 2);
}
