#ifndef __JKT__CONFIGURATIONRESEAUVIEW_H
#define __JKT__CONFIGURATIONRESEAUVIEW_H

using namespace std;

#include "SDL.h"
#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"

class ConfigurationReseauView : public View
{
	char _ipServeur[40];	// Adresse IP du serveur de jeu
	int _portServeur;		// Port du serveur
	int _isServeur;			// Mode serveur si vrai

public:
	ConfigurationReseauView(const AG_EventFn controllerCallback);
	~ConfigurationReseauView(void);

	void show(void);
	const char* getIpServeur(void) const;
	int getPortServeur() const;
	bool isServeur() const;
};

#endif
