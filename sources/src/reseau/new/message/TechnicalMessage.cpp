/*
 * TechnicalMessage.cpp
 *
 *  Created on: 20 juin 2013
 *      Author: vgdj7997
 */

#include "SDL.h"
#include "SDL_net.h"

#include "util/types/Bytes.h"

using namespace JktUtils;

#include "reseau/new/message/C2SHelloTechnicalMessage.h"
#include "reseau/new/message/C2SByeTechnicalMessage.h"
#include "reseau/new/message/S2CConnectionAcceptedTechnicalMessage.h"
#include "reseau/new/message/S2CConnectionRefusedTechnicalMessage.h"
#include "reseau/new/message/S2CDisconnectionTechnicalMessage.h"

#include "reseau/new/message/TechnicalMessage.h"

TechnicalMessage::TechnicalMessage(int code) {
	_code = code;
}

TechnicalMessage::~TechnicalMessage() {
}

int TechnicalMessage::getCode() const {
	return _code;
}

TechnicalMessage* TechnicalMessage::traduct(JktUtils::Bytes* bytes) {
	TechnicalMessage* msg = 0;

	const char* data = bytes->getBytes();
	int size = bytes->size();

	if(size >= 2) {
		int code = SDLNet_Read16((void*)data);

		switch(code) {
		case TechnicalMessage::C2S_HELLO:
			msg = new C2SHelloTechnicalMessage();
			break;

		case TechnicalMessage::S2C_CONNECTION_ACCEPTED:
			if(size >=4) {
				int port = SDLNet_Read16((void*)data);
				msg = new S2CConnectionAcceptedTechnicalMessage(port);
			}
			break;

		case TechnicalMessage::S2C_CONNECTION_REFUSED:
			msg = new S2CConnectionRefusedTechnicalMessage();
			break;

		case TechnicalMessage::C2S_BYE:
			msg = new C2SByeTechnicalMessage();
			break;

		case TechnicalMessage::S2C_DISCONNECTION:
			msg = new S2DisconnectionTechnicalMessage();
			break;
		}
	}

	return msg;
}
