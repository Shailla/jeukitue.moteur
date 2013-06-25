/*
 * C2SHelloTechnicalMessage.cpp
 *
 *  Created on: 23 juin 2013
 *      Author: vgdj7997
 */

#include "SDL.h"
#include "SDL_net.h"

#include "reseau/new/message/C2SHelloTechnicalMessage.h"

using namespace JktUtils;

C2SHelloTechnicalMessage::C2SHelloTechnicalMessage() : TechnicalMessage(C2S_HELLO) {
}

C2SHelloTechnicalMessage::~C2SHelloTechnicalMessage() {
}

Bytes* C2SHelloTechnicalMessage::toBytes() const {
	char* msg = new char[2];
	SDLNet_Write16(getCode(), msg);

	return new Bytes(msg, 2);
}