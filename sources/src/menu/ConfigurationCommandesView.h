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

	static bool _waitingCommandUserChoice;
	static int _commandToWait;

	AG_Box* _boxCommandes;

	static void updateCommande(CCfg::CComID& commandeToUpdate, const SDLKey& key, const int& mouse);

public:
	ConfigurationCommandesView(const AG_EventFn controllerCallback);
	~ConfigurationCommandesView(void);

	void show(void);
	static bool eventInterceptor(SDL_Event* event);
	static void beginWaitUserCommandChoice(AG_Event* event);
	void addCommande(const COMMANDE_ID commandId, const char* commandeLabel, const char* commandName);
};

#endif

