#ifndef __JKT__CONFIGURATIONVIEW_H
#define __JKT__CONFIGURATIONVIEW_H

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"

class ConfigurationView : public View
{
public:
	ConfigurationView(const AG_EventFn controllerCallback);
	~ConfigurationView(void);
};

#endif

