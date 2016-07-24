/*
 * PluginWidgetEvent.cpp
 *
 *  Created on: 11 févr. 2014
 *      Author: vgdj7997
 */

#include <plugin/PluginWidgetEvent.h>

namespace jkt {

PluginWidgetEvent::PluginWidgetEvent(LunarProxy* source) {
	_source = source;
}

PluginWidgetEvent::~PluginWidgetEvent() {
}

LunarProxy* PluginWidgetEvent::getSource() {
	return _source;
}

} /* namespace jkt */
