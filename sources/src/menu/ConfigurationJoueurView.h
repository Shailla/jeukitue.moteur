#ifndef __JKT__CONFIGURATIONJOUEURVIEW_H
#define __JKT__CONFIGURATIONJOUEURVIEW_H

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"

class ConfigurationJoueurView : public View
{
	char _joueurName[10];			// Texbox du nol du joueur
	AG_Combo* _playerMapCombo;
	char _joueurMapName[20];		// Nom de map sélectionné pour le joueur
public:
	ConfigurationJoueurView(const AG_EventFn controllerCallback);
	~ConfigurationJoueurView(void);

	void show(void);
	const char* getJoueurName(void);
	const char* getJoueurMapName(void);
};

#endif

