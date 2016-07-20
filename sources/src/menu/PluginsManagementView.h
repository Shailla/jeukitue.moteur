#ifndef __JKT__PLUGINSMANAGEMENTVIEW_H
#define __JKT__PLUGINSMANAGEMENTVIEW_H

#include <vector>
#include <string>

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"

class PluginsManagementView : public View
{
	AG_Box* _boxVert;
	std::vector<AG_Box*> _pluginBoxs;
	std::vector<std::string> _pluginNames;
public:
	PluginsManagementView(const AG_EventFn controllerCallback);
	~PluginsManagementView(void);

	void show(void);
	std::string getPluginName(const int pluginNumber);
};

#endif
