#ifndef __JKT__CONFIGURATIONRESEAUVIEW_H
#define __JKT__CONFIGURATIONRESEAUVIEW_H

using namespace std;

#include "SDL.h"
#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"

class ConfigurationReseauView : public View
{
	AG_Textbox* _ipServeurTexbox;

	char _ipServeur[40];			// Adresse IP du serveur de jeu
	int _port;						// Port du serveur
	int _portTree;
	int _isModeServeur;				// Mode serveur si vrai

public:
	ConfigurationReseauView(const AG_EventFn controllerCallback);
	~ConfigurationReseauView(void);

	void show(void);
	const char* getIpServeur(void) const;
	int getPort() const;
	int getPortTree() const;
	bool isModeServeur() const;
};

#endif

