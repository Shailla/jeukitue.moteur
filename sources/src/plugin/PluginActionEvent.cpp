/*
 * PluginMessageEvent.cpp
 *
 *  Created on: 11 févr. 2014
 *      Author: vgdj7997
 */

#include "plugin/PluginActionEvent.h"

namespace jkt {

PluginActionEvent::PluginActionEvent(Controller::Action actionId) {
	_actionId = actionId;
}

PluginActionEvent::~PluginActionEvent() {
}

Controller::Action PluginActionEvent::getActionId() const {
	return _actionId;
}

} /* namespace jkt */
