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
		ShowConfigurationAudio,
		ChangeCommande,
		ShowConfigurationCommandes,
		WaitUserCommandChoice,
		ShowConfigurationReseauViewAction,
		ShowConfigurationJoueurViewAction,
		ShowConfigurationVideoViewAction,
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
		ShowDataTreeViewAction,
		Refresh,									// Demande de rafraichissement de la fenêtre
		RejoindrePartieServerAction,				// Rejoindre la partie en cours sur le serveur
		InfoServerAction,							// Obtenir les informations du serveur
		PingServerAction							// Afficher dans la console le résultat d'un ping sur le serveur
	};

    static Viewer* m_agarView;

    Controller(Viewer* agarView);
    ~Controller(void);

    static void executeAction(AG_Event *event);
};

#endif
