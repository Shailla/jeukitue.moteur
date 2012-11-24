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
		AboutAction,
		MultijoueursAction,
		LanceServeurAction,
		LanceServeurMapAction,
		CentralisateurAction,
		ConfigurationAction,
		QuitGameAction,
		ConfigurationVideoAction,
		ConfigurationJoueurAction,
		ConfigCentralisateurAction,
		OpenSceneAction,
		OpenSceneASEAction,
		OpenSceneMapAction,
		OpenSceneASEEcraseRepOuiAction,
		OpenSceneASEEcraseRepNonAction,
		OpenMapAction,
		OpenASEAction,
		SendGlobalChatTextAction,
		ConsoleUserExecuteAction,
		QuitAction,
		SaveConfigCentralisateurAction,
		SaveConfigJoueurAction,
		ReloadDownloadFilesAction,
		DownloadOneFileAction,
		PluginsManagementAction,
		PluginActivateAction,
		PluginDeactivateAction,
		AgarConfigurationAction,
		DebugAction,						// Affiche le menu de degbug
		LastOpenGlErrorAction,				// Affiche la derniere erreur OpenGL dans une popup
		MapTreeAction						// Affiche les données de la MAP courante sous forme d'un arbre
	};

    static Viewer* m_agarView;

    Controller(Viewer* agarView);
    ~Controller(void);

    static void executeAction(AG_Event *event);
};

#endif
