/*
 * PluginWidgetEvent.h
 *
 *  Created on: 11 f�vr. 2014
 *      Author: vgdj7997
 */

#ifndef PLUGINWIDGETEVENT_H_
#define PLUGINWIDGETEVENT_H_

namespace JktPlugin {

class LunarProxy;

class PluginWidgetEvent {
	LunarProxy* _source;

public:
	PluginWidgetEvent(LunarProxy* source);
	virtual ~PluginWidgetEvent();

	LunarProxy* getSource();
};

} /* namespace JktPlugin */

#endif /* PLUGINWIDGETEVENT_H_ */
