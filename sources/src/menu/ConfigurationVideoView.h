#ifndef __JKT__CONFIGURATIONVIDEOVIEW_H
#define __JKT__CONFIGURATIONVIDEOVIEW_H

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"

class ConfigurationVideoView : public View
{
	int _fullScreen;		// >0 sur mode plein écran, =0 sinon
	AG_Combo* _resoCombo;	// Combo présentant les résolutions vidéo disponibles
public:
	ConfigurationVideoView(const AG_EventFn controllerCallback);
	~ConfigurationVideoView(void);

	void show(void);
};

#endif

