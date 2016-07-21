#ifndef __JKT__CONFIGURATIONCOMMANDESVIEW_H
#define __JKT__CONFIGURATIONCOMMANDESVIEW_H

#include <map>

#include "SDL.h"

#include <agar/core.h>
#include <agar/gui.h>

#include "main/Cfg.h"

#include "menu/View.h"

class ConfigurationCommandesView : public View {
	enum COMMANDE_ID {
		NONE = 0,
		AVANCER,
		RECULER,
		GAUCHE,
		DROITE,
		CAMERA_MONTER,
		CAMERA_DESCENDRE,
		CAMERA_DROITE,
		CAMERA_GAUCHE,
		CAMERA_RECULER,
		CAMERA_AVANCER,
		TIR1,
		TIR2,
		MONTER,
		SELECT_WEAPON_UP,
		SELECT_WEAPON_DOWN,
		GRAVITY,
		TEXTURES
	};

	static ConfigurationCommandesView* THIZ;

	std::map<COMMANDE_ID, AG_Button*> _buttons;
	std::map<COMMANDE_ID, AG_Label*> _labels;

	bool _waitingCommandUserChoice;
	COMMANDE_ID _commandToWait;

	static void setCommande(CCfg::CComID& commandeToUpdate, const SDLKey& key, const int& mouse);
	void refreshCommandButton(COMMANDE_ID commandId);

public:
	ConfigurationCommandesView(const AG_EventFn controllerCallback);
	~ConfigurationCommandesView(void);

	void refresh(void);
	void show(void);

	static bool eventInterceptor(SDL_Event* event);
	static void beginWaitUserCommandChoice(int commandId);

	void addCommandeButton(AG_Object* parent, const COMMANDE_ID commandId, const char* commandeLabel, const char* commandName);
};

#endif

