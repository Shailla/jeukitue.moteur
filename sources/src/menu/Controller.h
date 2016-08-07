#ifndef __JKT__AGARCONTROLLER_H
#define __JKT__AGARCONTROLLER_H

#include <string>

#include "SDL.h"

#include <agar/core.h>
#include <agar/gui.h>
#include <main/event/JktEvent.h>

#include "Viewer.h"

using namespace jkt;

namespace jkt {
	class PluginEngine;
}

class Controller
{
public:
	enum Action {
		Unknown = 10,
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
		RefreshMap								= 1003,		// Regular refresh event (to update a plugin 3D object position for instance)
		Widget									= 1004,		// GUI Widget event (plugin button pressed for instance)

		ShowConfigurationMenuAction				= 1104,		// Show the configuration menu
		ShowMultijoueursMenuAction				= 1105,		// Show the multiplayer configuration
		ShowConfigurationJoueurViewAction		= 1107,		// Show player configuration
		ShowConfigurationVideoViewAction		= 1108,		// Show video configuration
		ShowConfigurationReseauViewAction		= 1109,		// Show network configuration
		ShowMenuConfigCentralisateurAction		= 1110,		// Show centralisateur configuration
		ShowPluginsManagementViewAction			= 1111,		// Show plugin configuration
		ShowAgarConfigurationViewAction			= 1112,		// Show agar configuration
		ShowConfigurationCommandesViewAction	= 1113,		// Show commandes configuration
		ShowDebugViewAction						= 1114,		// Show the debug menu
		ShowMultijoueursClientMenuAction		= 1115,		// Show the multiplayer menu
		ShowMenuOpenSceneAction					= 1116,		// Show the open scene (MAP or ASE) menu
		QuitGameAction							= 1117,		// Quit the current MAP
		QuitAction								= 1118,		// Quit the game
		ShowConfigurationAdvancedViewAction		= 1119,		// Advanced configuration window
		PlayerZoneDetectorActivated				= 1120,		// Le joueur entre dans une zone de détection
		PlayerZoneDetectorUnactivated			= 1121		// Le joueur sort d'une zone de détection

		// The actions created by the plugins should not be referenced in C++ code, their identifiers are higher than 10000
	};

    static Viewer* _agarView;
    static jkt::PluginEngine* _pluginEngine;
    static SDL_mutex* _controllerMutex;

    Controller(Viewer* agarView, jkt::PluginEngine* pluginEngine);
    ~Controller(void);

    static void executeAction(int actionId);
    static void executeAction(JktEvent& event);
    static void executeAgarAction(AG_Event* event);
};

#endif
