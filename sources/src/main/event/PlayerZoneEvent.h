/*
 * PlayerZoneEvent.h
 *
 *  Created on: 3 août 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_MAIN_EVENT_PLAYERZONEEVENT_H_
#define SRC_MAIN_EVENT_PLAYERZONEEVENT_H_

#include "main/Player.h"
#include "main/event/JktEvent.h"

namespace jkt {

class PlayerZoneEvent : public JktEvent {
	CPlayer* _player;
public:
	PlayerZoneEvent(int actionId, CPlayer* player);
	virtual ~PlayerZoneEvent();

	CPlayer* getPlayer();
};

} /* namespace jkt */

#endif /* SRC_MAIN_EVENT_PLAYERZONEEVENT_H_ */
