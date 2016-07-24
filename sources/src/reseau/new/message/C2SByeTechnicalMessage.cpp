/*
 * ByeTechnicalMessage.cpp
 *
 *  Created on: 23 juin 2013
 *      Author: vgdj7997
 */

#include "SDL.h"
#include "SDL_net.h"

#include "reseau/new/message/C2SByeTechnicalMessage.h"

using namespace jkt;

C2SByeTechnicalMessage::C2SByeTechnicalMessage() : TechnicalMessage(C2S_BYE) {
}

C2SByeTechnicalMessage::~C2SByeTechnicalMessage() {
}

jkt::Bytes* C2SByeTechnicalMessage::toBytes() {
	char* msg = new char[2];
	SDLNet_Write16(getCode(), msg);

	return new jkt::Bytes(msg, 2);
}
