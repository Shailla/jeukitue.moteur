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
		ShowMainMenuAction = 0,
		ShowAboutViewAction,
		ShowMultijoueursMenuAction,
		ShowLanceServeurViewAction,
		LanceServeurMapAction,
		ShowCentralisateurViewAction,
		ShowConfigurationMenuAction,
		QuitGameAction,
		ShowConfigurationVideoViewAction,
		ShowConfigurationReseauViewAction,
		ShowConfigurationJoueurViewAction,
		ShowMenuConfigCentralisateurAction,
		ShowMenuOpenSceneAction,
		ShowOpenAseViewAction,
		ShowOpenMapViewAction,
		OpenSceneASEEcraseRepOuiAction,
		OpenSceneASEEcraseRepNonAction,
		OpenMapAction,
		ImportAseAction,
		SendGlobalChatTextAction,
		ExecuteUserCommandeAction,
		QuitAction,
		SaveConfigCentralisateurAction,
		SaveConfigJoueurAction,
		SaveConfigReseauAction,
		ReloadDownloadFilesAction,
		DownloadOneFileAction,
		ShowPluginsManagementViewAction,
		PluginActivateAction,
		PluginDeactivateAction,
		ShowAgarConfigurationViewAction,
		ShowDebugViewAction,						// Affiche le menu de degbug
		ShowLastOpenGlErrorViewAction,				// Affiche la derniere erreur OpenGL dans une popup
		ShowMapTreeViewAction,						// Affiche les données de la MAP courante sous forme d'un arbre
		ShowDataTreeViewAction
	};

    static Viewer* m_agarView;

    Controller(Viewer* agarView);
    ~Controller(void);

    static void executeAction(AG_Event *event);
};

#endif
