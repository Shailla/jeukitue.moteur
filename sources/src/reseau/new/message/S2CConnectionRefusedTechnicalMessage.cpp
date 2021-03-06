/*
 * S2CConnectionRefusedTechnicalMessage.cpp
 *
 *  Created on: 23 juin 2013
 *      Author: vgdj7997
 */

#include "SDL.h"
#include "SDL_net.h"

#include "reseau/new/message/S2CConnectionRefusedTechnicalMessage.h"

using namespace JktUtils;

S2CConnectionRefusedTechnicalMessage::S2CConnectionRefusedTechnicalMessage() : TechnicalMessage(S2C_CONNECTION_REFUSED) {
}

S2CConnectionRefusedTechnicalMessage::~S2CConnectionRefusedTechnicalMessage() {
}

JktUtils::Bytes* S2CConnectionRefusedTechnicalMessage::toBytes() {
	char* msg = new char[2];
	SDLNet_Write16(getCode(), msg);

	return new JktUtils::Bytes(msg, 2);
}
