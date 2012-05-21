/*
 * PluginNotebook.h
 *
 *  Created on: 13 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINNOTEBOOK_H_
#define PLUGINNOTEBOOK_H_

#include <string>

#include <agar/core.h>
#include <agar/gui.h>

#include "plugin/PluginContext.h"

namespace JktPlugin {

class PluginTab;

class PluginNotebook {
	AG_Notebook* _notebook;
public:
	PluginNotebook(AG_Widget* parent);
	virtual ~PluginNotebook();

	// Enfants
	PluginTab* addTab(const std::string& tabTitle);
};

} /* namespace JktPlugin */
#endif /* PLUGINNOTEBOOK_H_ */
