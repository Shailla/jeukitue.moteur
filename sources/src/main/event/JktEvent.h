/*
 * JktEvent.h
 *
 *  Created on: 3 août 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_MAIN_EVENT_JKTEVENT_H_
#define SRC_MAIN_EVENT_JKTEVENT_H_

namespace jkt {

class JktEvent {
	int _actionId;
public:
	JktEvent(int actionId);
	virtual ~JktEvent();

	int getActionId() const;
};

} /* namespace JktNet */

#endif /* SRC_MAIN_EVENT_JKTEVENT_H_ */
