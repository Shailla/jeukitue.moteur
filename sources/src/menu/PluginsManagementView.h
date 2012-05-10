#ifndef __JKT__PLUGINSMANAGEMENTVIEW_H
#define __JKT__PLUGINSMANAGEMENTVIEW_H

#include <vector>
#include <string>

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"

class PluginsManagementView : public View
{
	AG_Scrollview* _scrollview;
	std::vector<AG_Box*> _pluginBoxs;
	std::vector<AG_Button*> _pluginActivateButtons;
	std::vector<AG_Button*> _pluginDeactivateButtons;
	std::vector<AG_Button*> _pluginExecuteButtons;
	std::vector<AG_Label*> _pluginLabels;
	std::vector<std::string> _pluginNames;
public:
	PluginsManagementView(const AG_EventFn controllerCallback);
	~PluginsManagementView(void);

	void show(void);
	std::string getPluginName(const int pluginNumber);
};

#endif
