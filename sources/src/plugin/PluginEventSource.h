/*
 * PluginEventSource.h
 *
 *  Created on: 11 févr. 2014
 *      Author: vgdj7997
 */

#ifndef PLUGINEVENTSOURCE_H_
#define PLUGINEVENTSOURCE_H_

namespace jkt {

class LunarProxy;

class PluginEventSource {
	LunarProxy* _source;

public:
	PluginEventSource(LunarProxy* source);
	virtual ~PluginEventSource();

	LunarProxy* getSource();
};

} /* namespace jkt */

#endif /* PLUGINEVENTSOURCE_H_ */
