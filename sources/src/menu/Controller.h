#ifndef __JKT__AGARCONTROLLER_H
#define __JKT__AGARCONTROLLER_H

#include <string>

#include "SDL.h"

#include <agar/core.h>
#include <agar/gui.h>

#include "Viewer.h"

namespace JktPlugin {
	class PluginEngine;
}

class Controller
{
public:
	enum Action {
		Unknown,
		ShowLanceServeurViewAction,
		LanceServeurMapAction,
		ShowCentralisateurViewAction,
		ChangeCommande,
		WaitUserCommandChoice,
		ShowOpenAseViewAction,
		ShowOpenMapViewAction,
		OpenSceneASEEcraseRepOuiAction,
		OpenSceneASEEcraseRepNonAction,
		OpenMapAction,
		ImportAseAction,
		SendGlobalChatTextAction,
		ExecuteUserCommandeAction,
		SaveConfigCentralisateurAction,
		SaveConfigJoueurAction,
		SaveConfigReseauAction,
		ReloadDownloadFilesAction,
		DownloadOneFileAction,
		PluginActivateAction,
		PluginDeactivateAction,
		ShowLastOpenGlErrorViewAction,				// Affiche la derniere erreur OpenGL dans une popup
		ShowMapTreeViewAction,						// Affiche les données de la MAP courante sous forme d'un arbre
		ShowDataTreeViewAction,						// Affiche l'arbre de données
		ShowPlayersListViewAction,					// Affiche la liste des joueurs
		ShowNetworkConnectionsViewAction,			// Affiche la liste courante des connexions résea
		Refresh,									// Demande de rafraichissement de la fenêtre
		RejoindrePartieServerAction,				// Rejoindre la partie en cours sur le serveur
		InfoServerAction,							// Obtenir les informations du serveur
		PingServerAction,							// Afficher dans la console le résultat d'un ping sur le serveur
		ConnectClientAction,						// Connecte le client au serveur
		DeconnectClientAction,						// Déconnecte le client du serveur

		// Actions available or redistibuted to the plugins
		PLUGIN_BASE_ID							= 1000,		// All action higher than PLUGIN_BASE_ID is available for plugin usage
		HideMenuAction 							= 1001,		// Hide the menu (action event recognized by the menu plugin as the action to hide the menu)
		ShowMenuAction 							= 1002,		// Show the menu (action event recognized by the menu plugin as the action to show the menu)
		ShowConfigurationMenuAction				= 1004,		// Show the configuration menu
		ShowMultijoueursMenuAction				= 1005,		// Show the multiplayer configuration
		ShowConfigurationJoueurViewAction		= 1007,		// Show player configuration
		ShowConfigurationVideoViewAction		= 1008,		// Show video configuration
		ShowConfigurationReseauViewAction		= 1009,		// Show network configuration
		ShowMenuConfigCentralisateurAction		= 1010,		// Show centralisateur configuration
		ShowPluginsManagementViewAction			= 1011,		// Show plugin configuration
		ShowAgarConfigurationViewAction			= 1012,		// Show agar configuration
		ShowConfigurationCommandesViewAction	= 1013,		// Show commandes configuration
		ShowDebugViewAction						= 1014,		// Show the debug menu
		ShowMultijoueursClientMenuAction		= 1015,		// Show the multiplayer menu
		ShowMenuOpenSceneAction					= 1016,		// Show the open scene (MAP or ASE) menu
		QuitGameAction							= 1017,		// Quit the current MAP
		QuitAction								= 1018,		// Quit the game

		// The actions created by the plugins should not be referenced in C++ code, their identifiers are higher than 10000
	};

    static Viewer* _agarView;
    static JktPlugin::PluginEngine* _pluginEngine;
    static SDL_mutex* _controllerMutex;

    Controller(Viewer* agarView, JktPlugin::PluginEngine* pluginEngine);
    ~Controller(void);

    static void executeAction(AG_Event *event);
};

#endif
