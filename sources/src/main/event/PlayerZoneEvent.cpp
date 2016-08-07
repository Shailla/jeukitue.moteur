/*
 * PlayerZoneEvent.cpp
 *
 *  Created on: 3 août 2016
 *      Author: VGDJ7997
 */

#include "main/event/PlayerZoneEvent.h"

namespace jkt {

PlayerZoneEvent::PlayerZoneEvent(int actionId, CPlayer* player) : JktEvent(actionId) {
	_player = player;
}

PlayerZoneEvent::~PlayerZoneEvent() {
}

CPlayer* PlayerZoneEvent::getPlayer() {
	return _player;
}

} /* namespace jkt */
