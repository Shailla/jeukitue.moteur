#include <iostream>
#include <vector>

using namespace std;

#include <windows.h>
#include <GL/glew.h>
#include <GL/glu.h>

#include <shellapi.h>

#include "boost/filesystem/operations.hpp" // includes boost/filesystem/path.hpp
#include "boost/filesystem/fstream.hpp"
using namespace boost::filesystem;

#include <agar/core.h>
#include <agar/gui.h>
#include <agar/dev.h>

#include "main/divers.h"
#include "menu/ConsoleView.h"
#include "menu/CentralisateurView.h"
#include "menu/ConfigCentralisateurView.h"
#include "menu/OpenSceneMapView.h"
#include "menu/PluginsManagementView.h"
#include "menu/OpenSceneASEView.h"
#include "menu/OpenSceneASEEcraseRepView.h"
#include "menu/ConfigurationJoueurView.h"
#include "menu/ConfigurationReseauView.h"
#include "menu/LanceServeurView.h"
#include "menu/MultijoueursView.h"
#include "menu/MultijoueursClientView.h"
#include "menu/ProgressBarView.h"
#include "menu/ConfigurationCommandesView.h"
#include "main/Fabrique.h"
#include "centralisateur/Centralisateur.h"
#include "centralisateur/TcpConnector.h"
#include "centralisateur/data/DownloadFileItem.h"
#include "ihm/Portail.h"
#include "util/Trace.h"
#include "main/Game.h"
#include "main/Focus.h"
#include "main/Cfg.h"
#include "reseau/SPA.h"
#include "reseau/Client.h"
#include "reseau/Server.h"
#include "reseau/NetworkManager.h"
#include "spatial/Map.h"
#include "spatial/materiau/MaterialTexture.h"
#include "spatial/materiau/MaterialMulti.h"
#include "spatial/AseImporter.h"
#include "util/FindFolder.h"
#include "main/event/PlayerZoneEvent.h"
#include "plugin/lua/proxy/game/PluginPlayerZoneEventProxy.h"
#include "plugin/PluginEngine.h"
#include "util/StringUtils.h"

#include "plugin/PluginActionEvent.h"

#include "menu/Controller.h"

using namespace jkt;

extern CGame Game;
extern CCfg Config;
extern NetworkManager* _networkManager;

Viewer* Controller::_agarView;
jkt::PluginEngine* Controller::_pluginEngine;
SDL_mutex* Controller::_controllerMutex;

Controller::Controller(Viewer* agarView, jkt::PluginEngine* pluginEngine) {
	_agarView = agarView;
	_pluginEngine = pluginEngine;

	_controllerMutex = SDL_CreateMutex();
}

Controller::~Controller(void) {
}

void Controller::executeAction(int actionId) {
	JktEvent evt(actionId);
	executeAction(evt);
}

void Controller::executeAction(JktEvent& event) {
	SDL_LockMutex(_controllerMutex);

	int actionId = event.getActionId();

	if(actionId >= 10000) {	// Au delà de 10000 il s'agit d'une action générée  par un plugin, on la redispatche à tous les plugins
		_pluginEngine->dispatchEvent(actionId);
	}
	else {
		switch(actionId) {
		case HideMenuAction: {
			_agarView->hideAllMenuViews();

			_pluginEngine->dispatchEvent(Controller::Action::HideMenuAction);
		}
		break;

		case PlayerZoneDetectorActivated: {
			PlayerZoneEvent& evt = (PlayerZoneEvent&)event;
			CPlayer* player = evt.getPlayer();

			PluginEventProxy plugEvent(Controller::Action::PlayerZoneDetectorActivated);
			plugEvent.setInfo(new PluginPlayerZoneEventProxy(player->getId()));
			_pluginEngine->dispatchEvent(plugEvent);
		}
		break;

		case PlayerZoneDetectorUnactivated: {
			PlayerZoneEvent& evt = (PlayerZoneEvent&)event;
			CPlayer* player = evt.getPlayer();

			PluginEventProxy plugEvent(Controller::Action::PlayerZoneDetectorUnactivated);
			plugEvent.setInfo(new PluginPlayerZoneEventProxy(player->getId()));
			_pluginEngine->dispatchEvent(plugEvent);
		}
		break;

		case ShowConfigurationMenuAction:
			_agarView->hideAllMenuViews();
			_pluginEngine->dispatchEvent(Controller::Action::ShowConfigurationMenuAction);
			break;

		case ShowMenuAction:
			_agarView->hideAllMenuViews();
			_pluginEngine->dispatchEvent(Controller::Action::ShowMenuAction);
			break;

		case ShowMultijoueursMenuAction:
			_agarView->showMenuView(Viewer::MULTIJOUEURS_VIEW);
			break;

		case ShowMultijoueursClientMenuAction:
			_agarView->showMenuView(Viewer::MULTIJOUEURS_CLIENT_VIEW);
			break;

		case QuitGameAction:
			Game.Quit();
			break;

		case ShowConfigurationVideoViewAction:
			_agarView->showMenuView(Viewer::CONFIGURATION_VIDEO_VIEW);
			break;

		case ShowConfigurationReseauViewAction:
			_agarView->showMenuView(Viewer::CONFIGURATION_RESEAU_VIEW);
			break;

		case ShowConfigurationJoueurViewAction:
			_agarView->showMenuView(Viewer::CONFIGURATION_JOUEUR_VIEW);
			break;

		case ShowConfigurationCommandesViewAction:
			_agarView->showMenuView(Viewer::CONFIGURATION_COMMANDES_VIEW);
			break;

		case ShowConfigurationAdvancedViewAction:
			_agarView->hideAllMenuViews();
			_pluginEngine->dispatchEvent(Controller::Action::ShowConfigurationAdvancedViewAction);
			break;

		case ShowCentralisateurViewAction: {
			Centralisateur* centralisateur = Fabrique::getCentralisateur();
			centralisateur->connecter((char*)Config.Joueur.nom.c_str(), (char*)Config.Centralisateur.getIp().c_str(), Config.Centralisateur.getPort());
			centralisateur->sendSignalement();

			_agarView->showMenuView(Viewer::CENTRALISATEUR_VIEW);
		}
		break;

		case ShowMenuConfigCentralisateurAction:
			_agarView->showMenuView(Viewer::CONFIG_CENTRALISATEUR_VIEW);
			break;

		case SendGlobalChatTextAction: {
			CentralisateurView* view = (CentralisateurView*)_agarView->getView(Viewer::CENTRALISATEUR_VIEW);
			const char* textToSend = view->getTextToSend();
			Centralisateur* centralisateur = Fabrique::getCentralisateur();
			centralisateur->sendMessageChat(string(textToSend), Config.Joueur.nom.c_str());
			view->clearTextToSend();
		}
		break;

		case ShowMenuOpenSceneAction:
			_agarView->showMenuView(Viewer::OPEN_SCENE_VIEW);
			break;

		case ShowPluginsManagementViewAction:
			_agarView->showMenuView(Viewer::PLUGINS_MANAGEMENT_VIEW);
			break;

		case ShowOpenAseViewAction:
			_agarView->showMenuView(Viewer::OPEN_SCENE_ASE_VIEW);
			break;

			// Ouvre l'écran de choix d'une Map en mode de jeu local
		case ShowOpenMapViewAction:
			_agarView->showMenuView(Viewer::OPEN_SCENE_MAP_VIEW);
			break;

			// Ecran de choix d'une Map à lancer en mode serveur
		case ShowLanceServeurViewAction:
			_agarView->showMenuView(Viewer::LANCE_SERVEUR_VIEW);
			break;

		case QuitAction:
			quit_game("Quit game user action", 0);
			break;

		case OpenNavigator: {
			stringstream url;
			url << "http://localhost:" << (int)Config.Web.getHtmlServerPort() << "/index.html";
			ShellExecute(NULL, NULL, url.str().c_str(), NULL, NULL, SW_SHOWNORMAL);
			LOGINFO(("Opening in web browser : '%s'", url.str().c_str()));
		}
		break;

		// Menu de configuration d'Agar
		case ShowAgarConfigurationViewAction:
			DEV_ConfigShow();
			break;

			// Affiche le menu de debug
		case ShowDebugViewAction:
			_agarView->showMenuView(Viewer::DEBUG_MENU_VIEW);
			break;

			// Affiche les données de la MAP courante sous forme d'un arbre
		case ShowMapTreeViewAction:
			_agarView->showMenuView(Viewer::MAP_TREE_VIEW);
			break;

			// Affiche les données de la MAP courante sous forme d'un arbre
		case ShowDataTreeViewAction:
			_agarView->showMenuView(Viewer::DATA_TREE_VIEW);
			break;

		case ShowPlayersListViewAction:
			_agarView->showMenuView(Viewer::PLAYERS_LIST_VIEW);
			break;

		case ShowNetworkConnectionsViewAction:
			_agarView->showMenuView(Viewer::NETWORK_CONNECTIONS_VIEW);
			break;

		default:
			LOGERROR((" ==> unknown action %d! ", actionId));
			break;
		}
	}

	SDL_UnlockMutex(_controllerMutex);
}

void Controller::executeAgarAction(AG_Event* event) {
	SDL_LockMutex(_controllerMutex);

	int action = AG_INT(1);

	if(action >= 10000) {	// Au delà de 10000 il s'agit d'une action générée  par un plugin, on la redispatche à tous les plugins
		executeAction(action);
	}
	else {
		switch(action) {
		case HideMenuAction:
		case ShowMenuAction:
		case ShowConfigurationMenuAction:
		case ShowMultijoueursMenuAction:
		case ShowMultijoueursClientMenuAction:
		case QuitGameAction:
		case QuitAction:
		case OpenNavigator:
		case ShowConfigurationVideoViewAction:
		case ShowConfigurationReseauViewAction:
		case ShowConfigurationJoueurViewAction:
		case ShowConfigurationCommandesViewAction:
		case ShowConfigurationAdvancedViewAction:
		case ShowCentralisateurViewAction:
		case ShowMenuConfigCentralisateurAction:
		case SendGlobalChatTextAction: {
		case ShowMenuOpenSceneAction:
		case ShowPluginsManagementViewAction:
		case ShowOpenAseViewAction:
		case ShowOpenMapViewAction:
		case ShowLanceServeurViewAction:
		case ShowAgarConfigurationViewAction:
		case ShowDebugViewAction:
		case ShowMapTreeViewAction:
		case ShowDataTreeViewAction:
		case ShowPlayersListViewAction:
		case ShowNetworkConnectionsViewAction:
			executeAction(action);
			break;

		case WaitUserCommandChoice: {
			int commandId = AG_INT(2);

			ConfigurationCommandesView::beginWaitUserCommandChoice(commandId);

			// Réactualise la fenêtre
			_agarView->showMenuView(Viewer::CONFIGURATION_COMMANDES_VIEW);
		}
		break;

		// Ouvere une MAP en mode serveur
		case LanceServeurMapAction: {
			// Récupération du nom de la Map à ouvrir
			int mapNumber = AG_INT(2);
			LanceServeurView* view = (LanceServeurView*)_agarView->getView(Viewer::LANCE_SERVEUR_VIEW);
			string mapName = view->getMapName(mapNumber);

			// Ouverture de la Map
			Game.RequeteProcess.setOuvreMapServer(mapName);
		}
		break;

		// Ouverture d'une Map en mode de jeu local
		case OpenMapAction: {
			// Récupération du nom de la Map à ouvrir
			int mapNumber = AG_INT(2);
			OpenSceneMapView* view = (OpenSceneMapView*)_agarView->getView(Viewer::OPEN_SCENE_MAP_VIEW);
			string mapName = view->getMapName(mapNumber);

			cout << endl << "Ouverture de la Map '" << mapName << "'";

			// Ouverture de la Map
			Game.RequeteProcess.setOuvreMapLocal(mapName);
		}
		break;

		case RejoindrePartieServerAction: {
			CClient* client = _networkManager->getClient();

			if(client) {
				client->sendConnectedRequestJoinTheGame(Config.Joueur.nom);
			}
			else {
				AG_TextMsg(AG_MSG_ERROR, "Action impossible, vous n'etes pas connecte");
			}
		}
		break;

		case InfoServerAction: {
			MultijoueursClientView* view = (MultijoueursClientView*)_agarView->getView(Viewer::MULTIJOUEURS_CLIENT_VIEW);
			view->setServerName("");
			view->setActiveMap("");
			CClient* client = _networkManager->getClient();

			if(client) {
				client->sendNotConnectedRequestInfoToServer(Config.Reseau.getIpServer(), Config.Reseau.getServerPort());		// Demande ses infos au serveur
			}
			else {
				AG_TextMsg(AG_MSG_ERROR, "Action impossible, vous n'etes pas connecte");
			}
		}
		break;

		case PingServerAction: {
			MultijoueursClientView* view = (MultijoueursClientView*)_agarView->getView(Viewer::MULTIJOUEURS_CLIENT_VIEW);
			view->setPing(-1);
			CClient* client = _networkManager->getClient();

			if(client) {
				client->sendNotConnectedRequestPingToServer(Config.Reseau.getIpServer(), Config.Reseau.getServerPort());				// Demande au serveur de répondre à un ping
			}
			else {
				AG_TextMsg(AG_MSG_ERROR, "Action impossible, vous n'etes pas connecte");
			}
		}
		break;

		case ConnectClientAction: {
			if( !_networkManager->ouvreClient(Config.Reseau.getIpServer(), Config.Reseau.getServerPort(), Config.Reseau.getServerPortTree()) ) {
				_networkManager->setOn( false );		// Signale que le réseau ne peut pas être utilisé
				AG_TextMsg(AG_MSG_ERROR, "Echec de connexion au serveur");
			}
		}
		break;

		case DeconnectClientAction: {
			_networkManager->fermeClient();
		}
		break;

		// Activation d'un plugin
		case PluginActivateAction:
		{
			// Récupération du nom de la Map à ouvrir
			int pluginNumber = AG_INT(2);
			PluginsManagementView* view = (PluginsManagementView*)_agarView->getView(Viewer::PLUGINS_MANAGEMENT_VIEW);
			string pluginName = view->getPluginName(pluginNumber);

			cout << endl << "Activate plugin : '" << pluginName << "'";

			PluginEngine* pluginEngine = Fabrique::getPluginEngine();
			pluginEngine->activateGlobalPlugin(pluginName);
		}
		break;

		// Désactivation d'un plugin
		case PluginDeactivateAction:
		{
			// Récupération du nom de la Map à ouvrir
			int pluginNumber = AG_INT(2);
			PluginsManagementView* view = (PluginsManagementView*)_agarView->getView(Viewer::PLUGINS_MANAGEMENT_VIEW);
			string pluginName = view->getPluginName(pluginNumber);

			cout << endl << "Deactivate plugin : '" << pluginName << "'";

			PluginEngine* pluginEngine = Fabrique::getPluginEngine();
			pluginEngine->deactivateGlobalPlugin(pluginName);
		}
		break;

		// Affiche la denière erreur OpenGL dans une popup
		case ShowLastOpenGlErrorViewAction: {
			stringstream openGlError;
			openGlError << "Deniere erreur OpenGL : '" << gluErrorString(glGetError()) << "'";
			cerr << endl << __FILE__ << ":" << __LINE__ << " " << openGlError.str();
			AG_TextMsg(AG_MSG_ERROR, openGlError.str().c_str());
		}
		break;

		// Import d'un fichier ASE
		case ImportAseAction: {
			// Récupération du nom de la Map à ouvrir
			int aseNumber = AG_INT(2);
			OpenSceneASEView* view = (OpenSceneASEView*)_agarView->getView(Viewer::OPEN_SCENE_ASE_VIEW);
			string aseName = view->getAseName(aseNumber);

			// Ouverture de la Map
			path mapDir(string("./Map/") + aseName + "/");

			if(exists(mapDir)) {
				// Si le répertoire existe déjà, demande si on peut le supprimer
				OpenSceneASEEcraseRepView* view = (OpenSceneASEEcraseRepView*)_agarView->getView(Viewer::OPEN_SCENE_ASE_ECRASE_REP_VIEW);
				view->setAseName(aseName);	// Nom du fichier Ase

				_agarView->showMenuView(Viewer::OPEN_SCENE_ASE_ECRASE_REP_VIEW);
			}
			else {
				ConsoleView* console = (ConsoleView*)_agarView->getView(Viewer::CONSOLE_VIEW);

				// Import du fichier ASE
				AseImporter::lanceImportAse(aseName, console);
			}
		}
		break;

		case OpenSceneASEEcraseRepOuiAction: {
			OpenSceneASEEcraseRepView* view = (OpenSceneASEEcraseRepView*)_agarView->getView(Viewer::OPEN_SCENE_ASE_ECRASE_REP_VIEW);

			// Suppression du répertoire
			ConsoleView* console = (ConsoleView*)_agarView->getView(Viewer::CONSOLE_VIEW);

			// Import du fichier ASE
			string aseName = view->getAseName();
			AseImporter::lanceImportAse(aseName, console);

			_agarView->hideView(Viewer::OPEN_SCENE_ASE_ECRASE_REP_VIEW);
		}
		break;

		case OpenSceneASEEcraseRepNonAction: {
			_agarView->hideView(Viewer::OPEN_SCENE_ASE_ECRASE_REP_VIEW);

			_pluginEngine->dispatchEvent(Controller::Action::ShowMenuAction);

			AG_TextMsg(AG_MSG_INFO, "Import annul\u00e9");
		}
		break;

		case SaveConfigJoueurAction: {
			ConfigurationJoueurView* view = (ConfigurationJoueurView*)_agarView->getView(Viewer::CONFIGURATION_JOUEUR_VIEW);
			Config.Joueur.nom = view->getJoueurName();
			Config.Joueur.mapName = view->getJoueurMapName();
			Config.Ecrit();
		}
		break;

		case SaveConfigReseauAction: {
			ConfigurationReseauView* view = (ConfigurationReseauView*)_agarView->getView(Viewer::CONFIGURATION_RESEAU_VIEW);
			Config.Reseau.setIpServer(view->getIpServeur());
			Config.Reseau.setPort(view->getPort());
			Config.Reseau.setPortTree(view->getPortTree());
			Config.Ecrit();

			AG_TextMsg(AG_MSG_WARNING, "Configuration mise a jour");
		}
		break;


		// Mise à jour de la configuration du centralisateur
		case SaveConfigCentralisateurAction: {
			ConfigCentralisateurView* view = (ConfigCentralisateurView*)_agarView->getView(Viewer::CONFIG_CENTRALISATEUR_VIEW);
			Config.Centralisateur.setIp(view->getAdresse());
			Config.Centralisateur.setPort(view->getPort());
			Config.Ecrit();

			AG_TextMsg(AG_MSG_WARNING, "Configuration mise a jour");
		}
		break;

		// Mise à jour de la page des fichiers téléchargeables par le jeu
		case ReloadDownloadFilesAction: {
			try {
				Centralisateur* centralisateur = Fabrique::getCentralisateur();
				vector<DownloadFileItem> items = centralisateur->askDownloadFileList(4635);

				CentralisateurView* view = (CentralisateurView*)_agarView->getView(Viewer::CENTRALISATEUR_VIEW);
				view->updateDownloadFileList(items);
			}
			catch(CentralisateurTcpException& exception) {
				AG_TextMsg(AG_MSG_WARNING, "Impossible d'obtenir la listes des fichiers en telechargement");
			}
		}
		break;

		// Mise à jour de la page des fichiers téléchargeables par le jeu
		case DownloadOneFileAction: {
			long downloadId = AG_LONG(2);

			try {
				Centralisateur* centralisateur = Fabrique::getCentralisateur();

				ProgressBarView* view = (ProgressBarView*)_agarView->getView(Viewer::PROGRESS_BAR_VIEW);

				view->show();

				centralisateur->downloadOneFile(4635, downloadId, view);
			}
			catch(CentralisateurTcpException& exception) {
				AG_TextMsg(AG_MSG_WARNING, "Erreur lors du telechargement");
			}
		}
		break;

		// Demande de rafraichissement de la vue passée en paramètre
		case Refresh: {
			View* view = (View*)AG_PTR(2);
			view->refresh();
		}
		break;

		case ExecuteUserCommandeAction: {
			ConsoleView* view = (ConsoleView*)_agarView->getView(Viewer::CONSOLE_VIEW);
			string commande = view->getCommandAndClearCommandLine();

			if(!jkt::StringUtils::isBlank(commande)) {
				Fabrique::getCommandeInterpreter()->interpreteCommande(commande, true);
			}

			break;
		}

		default:
			LOGERROR((" ==> unknown action %d! ", action));
			break;
		}
		}

		SDL_UnlockMutex(_controllerMutex);
	}

}
