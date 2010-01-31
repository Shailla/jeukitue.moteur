#ifndef __JKT__AGARCONTROLLER_H
#define __JKT__AGARCONTROLLER_H

#include <string>

#include <agar/core.h>
#include <agar/gui.h>

#include "Viewer.h"

class Controller
{
public:
	enum {
		MainMenuAction = 0,
		MultijoueursAction,
		LanceServeurAction,
		LanceServeurMapAction,
		CentralisateurAction,
		ConfigurationAction,
		ConfigCentralisateurAction,
		OpenSceneAction,
		OpenSceneASEAction,
		OpenSceneMapAction,
		OpenSceneASEEcraseRepOuiAction,
		OpenSceneASEEcraseRepNonAction,
		OpenMapAction,
		OpenASEAction,
		SendGlobalChatTextAction,
		QuitAction,
		SaveConfigCentralisateurAction};

    static Viewer* m_agarView;

    Controller(Viewer* agarView);
    ~Controller(void);

    static void executeAction(AG_Event *event);
};

#endif
