#ifndef __JKT__CONFIGURATIONAUDIOVIEW_H
#define __JKT__CONFIGURATIONAUDIOVIEW_H

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"

class ConfigurationAudioView : public View
{
public:
	ConfigurationAudioView(const AG_EventFn controllerCallback);
	~ConfigurationAudioView(void);

	void show(void);
};

#endif

