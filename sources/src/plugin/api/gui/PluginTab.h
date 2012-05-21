/*
 * PluginTab.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINTAB_H_
#define PLUGINTAB_H_

#include <string>

#include <agar/core.h>
#include <agar/gui.h>

#include "plugin/api/gui/PluginPanel.h"

namespace JktPlugin {

class PluginTab : public PluginPanel{
	friend class PluginTabProxy;

	AG_NotebookTab* _tab;
public:
	PluginTab(AG_Notebook* parent, const std::string& tabTitle);
	virtual ~PluginTab();

	AG_Widget* getParent();
protected:
};

} /* namespace JktPlugin */
#endif /* PLUGINTAB_H_ */
