/*
 * JktEvent.cpp
 *
 *  Created on: 3 ao�t 2016
 *      Author: VGDJ7997
 */

#include "main/event/JktEvent.h"

namespace jkt {

JktEvent::JktEvent(int actionId) {
	_actionId = actionId;
}

JktEvent::~JktEvent() {
}

int JktEvent::getActionId() const {
	return _actionId;
}

} /* namespace JktNet */
