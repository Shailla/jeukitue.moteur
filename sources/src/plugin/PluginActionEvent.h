/*
 * PluginActionEvent.h
 *
 *  Created on: 11 f�vr. 2014
 *      Author: vgdj7997
 */

#ifndef PLUGINACTIONEVENT_H_
#define PLUGINACTIONEVENT_H_

#include "menu/Controller.h"

namespace jkt {

class PluginActionEvent {
private:
	Controller::Action _actionId;

public:
	PluginActionEvent(Controller::Action type);
	virtual ~PluginActionEvent();

	Controller::Action getActionId() const;
};

} /* namespace jkt */

#endif /* PLUGINACTIONEVENT_H_ */
