/*
 * S2DisconnectionTechnicalMessage.cpp
 *
 *  Created on: 23 juin 2013
 *      Author: vgdj7997
 */

#include "SDL.h"
#include "SDL_net.h"

#include "reseau/new/message/S2CDisconnectionTechnicalMessage.h"

using namespace JktUtils;

S2DisconnectionTechnicalMessage::S2DisconnectionTechnicalMessage() : TechnicalMessage(S2C_DISCONNECTION) {
}

S2DisconnectionTechnicalMessage::~S2DisconnectionTechnicalMessage() {
}

JktUtils::Bytes* S2DisconnectionTechnicalMessage::toBytes() {
	char* msg = new char[2];
	SDLNet_Write16(getCode(), msg);

	return new JktUtils::Bytes(msg, 2);
}
