/*
 * PluginEventSource.cpp
 *
 *  Created on: 11 févr. 2014
 *      Author: vgdj7997
 */

#include "plugin/PluginEventSource.h"

namespace jkt {

PluginEventSource::PluginEventSource(LunarProxy* source) {
	_source = source;
}

PluginEventSource::~PluginEventSource() {
}

LunarProxy* PluginEventSource::getSource() {
	return _source;
}

} /* namespace jkt */
