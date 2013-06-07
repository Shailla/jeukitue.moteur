#ifndef __JKT__CONFIGURATIONCOMMANDESVIEW_H
#define __JKT__CONFIGURATIONCOMMANDESVIEW_H

#include "SDL.h"

#include <agar/core.h>
#include <agar/gui.h>

#include "main/Cfg.h"

#include "menu/View.h"

class ConfigurationCommandesView : public View {
	enum COMMANDE_ID {
		AVANCER = 1,
		RECULER,
		GAUCHE,
		DROITE,
		TIR1,
		TIR2,
		MONTER
	};

	static int _commandToWait;

	AG_Label* _labelAvancer;
	AG_Label* _labelReculer;
	AG_Label* _labelGauche;
	AG_Label* _labelDroite;
	AG_Label* _labelTir1;
	AG_Label* _labelTir2;
	AG_Label* _labelMonter;

	static void updateCommande(CCfg::CComID& commandeToUpdate, const SDLKey& key, const int& mouse);

public:
	ConfigurationCommandesView(const AG_EventFn controllerCallback);
	~ConfigurationCommandesView(void);

	void show(void);
	static bool eventInterceptor(SDL_Event* event);
	static void beginWaitUserCommandChoice(AG_Event* event);
};

#endif

