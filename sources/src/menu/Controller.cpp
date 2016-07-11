#include <iostream>
#include <vector>

using namespace std;

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/glu.h>

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
#include "menu/ConsoleAvancementView.h"
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
#include "plugin/PluginEngine.h"
#include "util/StringUtils.h"

#include "plugin/PluginActionEvent.h"

#include "menu/Controller.h"

using namespace JktMenu;
using namespace JktMoteur;
using namespace JktPlugin;
using namespace JktNet;

extern CGame Game;
extern CCfg Config;
extern NetworkManager* _networkManager;

Viewer* Controller::_agarView;
JktPlugin::PluginEngine* Controller::_pluginEngine;
SDL_mutex* Controller::_controllerMutex;

Controller::Controller(Viewer* agarView, JktPlugin::PluginEngine* pluginEngine) {
	_agarView = agarView;
	_pluginEngine = pluginEngine;

	_controllerMutex = SDL_CreateMutex();
}

Controller::~Controller(void) {
}

void Controller::executeAction(AG_Event* event) {
	LOGINFO((" ==> UNE ACTION"));

	SDL_LockMutex(_controllerMutex);

	int action = AG_INT(1);

	LOGINFO((" ==> ACTION %d", action));

	if(action >= 10000) {	// Au delà de 10000 il s'agit d'une action d'un plugin, on la redispatche à tous les plugins
		PluginActionEvent evt((Controller::Action)action);
		_pluginEngine->dispatchEvent(evt);
	}
	else {
		switch(action) {
		case HideMenuAction:
		{
			LOGINFO((" ==> action controller %d (HideMenuAction)", action));

			_agarView->hideAllMenuViews();

			PluginActionEvent evt(Controller::Action::HideMenuAction);
			_pluginEngine->dispatchEvent(evt);
		}
		break;

		case ShowConfigurationMenuAction:
		{
			LOGINFO((" ==> action controller %d (ShowConfigurationMenuAction)", action));
			_agarView->hideAllMenuViews();

			PluginActionEvent evt(Controller::Action::ShowConfigurationMenuAction);
			_pluginEngine->dispatchEvent(evt);
		}
		break;

		case ShowMenuAction:
		{
			LOGINFO((" ==> action controller %d (ShowMenuAction)", action));
			_agarView->hideAllMenuViews();

			PluginActionEvent evt(Controller::Action::ShowMenuAction);
			_pluginEngine->dispatchEvent(evt);
		}
		break;

		case ShowMultijoueursMenuAction:
			LOGINFO((" ==> action controller %d (ShowMultijoueursMenuAction)", action));
			_agarView->showMenuView(Viewer::MULTIJOUEURS_VIEW);
			break;

		case ShowMultijoueursClientMenuAction:
			LOGINFO((" ==> action controller %d (ShowMultijoueursClientMenuAction)", action));
			_agarView->showMenuView(Viewer::MULTIJOUEURS_CLIENT_VIEW);
			break;

		case QuitGameAction:
			LOGINFO((" ==> action controller %d (QuitGameAction)", action));
			Game.Quit();
			break;

		case ShowConfigurationVideoViewAction:
			LOGINFO((" ==> action controller %d (ShowConfigurationVideoViewAction)", action));
			_agarView->showMenuView(Viewer::CONFIGURATION_VIDEO_VIEW);
			break;

		case ShowConfigurationReseauViewAction:
			LOGINFO((" ==> action controller %d (ShowConfigurationReseauViewAction)", action));
			_agarView->showMenuView(Viewer::CONFIGURATION_RESEAU_VIEW);
			break;

		case ShowConfigurationJoueurViewAction:
			LOGINFO((" ==> action controller %d (ShowConfigurationJoueurViewAction)", action));
			_agarView->showMenuView(Viewer::CONFIGURATION_JOUEUR_VIEW);
			break;

		case ShowConfigurationCommandesViewAction:
			LOGINFO((" ==> action controller %d (ShowConfigurationCommandesViewAction)", action));
			_agarView->showMenuView(Viewer::CONFIGURATION_COMMANDES_VIEW);
			break;

		case showConfigurationAdvancedViewAction:
			{
				LOGINFO((" ==> action controller %d (showConfigurationAdvancedViewAction)", action));

				_agarView->hideAllMenuViews();

				PluginActionEvent evt(Controller::Action::showConfigurationAdvancedViewAction);
				_pluginEngine->dispatchEvent(evt);
			}
			break;

		case WaitUserCommandChoice:
		{
			LOGINFO((" ==> action controller %d (WaitUserCommandChoice)", action));

			int commandId = AG_INT(2);

			ConfigurationCommandesView::beginWaitUserCommandChoice(commandId);

			// Réactualise la fenêtre
			_agarView->showMenuView(Viewer::CONFIGURATION_COMMANDES_VIEW);
		}
		break;

		case ShowCentralisateurViewAction:
		{
			LOGINFO((" ==> action controller %d (ShowCentralisateurViewAction)", action));

			Centralisateur* centralisateur = Fabrique::getCentralisateur();
			centralisateur->connecter((char*)Config.Joueur.nom.c_str(), (char*)Config.Centralisateur.getIp().c_str(), Config.Centralisateur.getPort());
			centralisateur->sendSignalement();

			_agarView->showMenuView(Viewer::CENTRALISATEUR_VIEW);
		}
		break;

		case ShowMenuConfigCentralisateurAction:
			LOGINFO((" ==> action controller %d (ShowMenuConfigCentralisateurAction)", action));
			_agarView->showMenuView(Viewer::CONFIG_CENTRALISATEUR_VIEW);
			break;

			// Envoi d'un message de chat sur le serveur centralisateur
		case SendGlobalChatTextAction:
		{
			LOGINFO((" ==> action controller %d (SendGlobalChatTextAction)", action));

			CentralisateurView* view = (CentralisateurView*)_agarView->getView(Viewer::CENTRALISATEUR_VIEW);
			const char* textToSend = view->getTextToSend();
			Centralisateur* centralisateur = Fabrique::getCentralisateur();
			centralisateur->sendMessageChat(string(textToSend), Config.Joueur.nom.c_str());
			view->clearTextToSend();
		}
		break;

		case ShowMenuOpenSceneAction:
			LOGINFO((" ==> action controller %d (ShowMenuOpenSceneAction)", action));
			_agarView->showMenuView(Viewer::OPEN_SCENE_VIEW);
			break;

		case ShowPluginsManagementViewAction:
			LOGINFO((" ==> action controller %d (ShowPluginsManagementViewAction)", action));
			_agarView->showMenuView(Viewer::PLUGINS_MANAGEMENT_VIEW);
			break;

		case ShowOpenAseViewAction:
			LOGINFO((" ==> action controller %d (ShowOpenAseViewAction)", action));
			_agarView->showMenuView(Viewer::OPEN_SCENE_ASE_VIEW);
			break;

			// Ouvre l'écran de choix d'une Map en mode de jeu local
		case ShowOpenMapViewAction:
			LOGINFO((" ==> action controller %d (ShowOpenMapViewAction)", action));
			_agarView->showMenuView(Viewer::OPEN_SCENE_MAP_VIEW);
			break;

			// Ecran de choix d'une Map à lancer en mode serveur
		case ShowLanceServeurViewAction:
			LOGINFO((" ==> action controller %d (ShowLanceServeurViewAction)", action));
			_agarView->showMenuView(Viewer::LANCE_SERVEUR_VIEW);
			break;

			// Ouvere une MAP en mode serveur
		case LanceServeurMapAction:
		{
			LOGINFO((" ==> action controller %d (LanceServeurMapAction)", action));

			// Récupération du nom de la Map à ouvrir
			int mapNumber = AG_INT(2);
			LanceServeurView* view = (LanceServeurView*)_agarView->getView(Viewer::LANCE_SERVEUR_VIEW);
			string mapName = view->getMapName(mapNumber);

			cout << endl << "Ouverture de la Map '" << mapName << "'";

			// Ouverture de la Map
			Game.RequeteProcess.setOuvreMapServer(mapName);
		}
		break;

		// Ouverture d'une Map en mode de jeu local
		case OpenMapAction:
		{
			LOGINFO((" ==> action controller %d (OpenMapAction)", action));

			// Récupération du nom de la Map à ouvrir
			int mapNumber = AG_INT(2);
			OpenSceneMapView* view = (OpenSceneMapView*)_agarView->getView(Viewer::OPEN_SCENE_MAP_VIEW);
			string mapName = view->getMapName(mapNumber);

			cout << endl << "Ouverture de la Map '" << mapName << "'";

			// Ouverture de la Map
			Game.RequeteProcess.setOuvreMapLocal(mapName);
		}
		break;

		case RejoindrePartieServerAction:
		{
			LOGINFO((" ==> action controller %d (RejoindrePartieServerAction)", action));

			CClient* client = _networkManager->getClient();

			if(client) {
				client->sendConnectedRequestJoinTheGame(Config.Joueur.nom);
			}
			else {
				AG_TextMsg(AG_MSG_ERROR, "Action impossible, vous n'etes pas connecte");
			}
		}
		break;

		case InfoServerAction:
		{
			LOGINFO((" ==> action controller %d (InfoServerAction)", action));

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

		case PingServerAction:
		{
			LOGINFO((" ==> action controller %d (PingServerAction)", action));

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

		case ConnectClientAction:
		{
			LOGINFO((" ==> action controller %d (ConnectClientAction)", action));

			if( !_networkManager->ouvreClient(Config.Reseau.getIpServer(), Config.Reseau.getServerPort(), Config.Reseau.getServerPortTree()) ) {
				_networkManager->setOn( false );		// Signale que le réseau ne peut pas être utilisé
				AG_TextMsg(AG_MSG_ERROR, "Echec de connexion au serveur");
			}
		}
		break;

		case DeconnectClientAction:
		{
			LOGINFO((" ==> action controller %d (DeconnectClientAction)", action));

			_networkManager->fermeClient();
		}
		break;

		// Activation d'un plugin
		case PluginActivateAction:
		{
			LOGINFO((" ==> action controller %d (PluginActivateAction)", action));

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
			LOGINFO((" ==> action controller %d (PluginDeactivateAction)", action));

			// Récupération du nom de la Map à ouvrir
			int pluginNumber = AG_INT(2);
			PluginsManagementView* view = (PluginsManagementView*)_agarView->getView(Viewer::PLUGINS_MANAGEMENT_VIEW);
			string pluginName = view->getPluginName(pluginNumber);

			cout << endl << "Deactivate plugin : '" << pluginName << "'";

			PluginEngine* pluginEngine = Fabrique::getPluginEngine();
			pluginEngine->deactivateGlobalPlugin(pluginName);
		}
		break;

		// Menu de configuration d'Agar
		case ShowAgarConfigurationViewAction:
		{
			LOGINFO((" ==> action controller %d (ShowAgarConfigurationViewAction)", action));

			DEV_ConfigShow();
		}
		break;

		// Affiche le menu de debug
		case ShowDebugViewAction:
		{
			LOGINFO((" ==> action controller %d (ShowDebugViewAction)", action));

			_agarView->showMenuView(Viewer::DEBUG_MENU_VIEW);
		}
		break;

		// Affiche les données de la MAP courante sous forme d'un arbre
		case ShowMapTreeViewAction:
		{
			LOGINFO((" ==> action controller %d (ShowMapTreeViewAction)", action));

			_agarView->showMenuView(Viewer::MAP_TREE_VIEW);
		}
		break;

		// Affiche les données de la MAP courante sous forme d'un arbre
		case ShowDataTreeViewAction:
		{
			LOGINFO((" ==> action controller %d (ShowDataTreeViewAction)", action));

			_agarView->showMenuView(Viewer::DATA_TREE_VIEW);
		}
		break;

		case ShowPlayersListViewAction:
		{
			LOGINFO((" ==> action controller %d (ShowPlayersListViewAction)", action));

			_agarView->showMenuView(Viewer::PLAYERS_LIST_VIEW);
		}
		break;

		case ShowNetworkConnectionsViewAction:
		{
			LOGINFO((" ==> action controller %d (ShowNetworkConnectionsViewAction)", action));

			_agarView->showMenuView(Viewer::NETWORK_CONNECTIONS_VIEW);
		}
		break;

		// Affiche la denière erreur OpenGL dans une popup
		case ShowLastOpenGlErrorViewAction:
		{
			LOGINFO((" ==> action controller %d (ShowLastOpenGlErrorViewAction)", action));

			stringstream openGlError;
			openGlError << "Deniere erreur OpenGL : '" << gluErrorString(glGetError()) << "'";
			cerr << endl << __FILE__ << ":" << __LINE__ << " " << openGlError.str();
			AG_TextMsg(AG_MSG_ERROR, openGlError.str().c_str());
		}
		break;

		// Import d'un fichier ASE
		case ImportAseAction:
		{
			LOGINFO((" ==> action controller %d (ImportAseAction)", action));

			// Récupération du nom de la Map à ouvrir
			int aseNumber = AG_INT(2);
			OpenSceneASEView* view = (OpenSceneASEView*)_agarView->getView(Viewer::OPEN_SCENE_ASE_VIEW);
			string aseName = view->getAseName(aseNumber);

			// Ouverture de la Map
			string nomRep = "./Map/" + aseName + "/";

			if(CFindFolder::mkdir(nomRep.c_str()) != 0)	{	// Création du répertoire pour les textures
				// Si un répertoire existe déjà, demande s'il faut l'écraser
				OpenSceneASEEcraseRepView* view = (OpenSceneASEEcraseRepView*)_agarView->getView(Viewer::OPEN_SCENE_ASE_ECRASE_REP_VIEW);
				view->setRepName(nomRep);	// Nom du répertoire du fichier Ase
				view->setAseName(aseName);	// Nom du fichier Ase

				_agarView->showMenuView(Viewer::OPEN_SCENE_ASE_ECRASE_REP_VIEW);
			}
			else {
				try {
					ConsoleAvancementView* console = (ConsoleAvancementView*)_agarView->getView(Viewer::CONSOLE_AVANCEMENT_VIEW);
					console->init("Import fichier ASE");
					_agarView->showMenuView(Viewer::CONSOLE_AVANCEMENT_VIEW);

					// Import du fichier ASE
					AseImporter::lanceImportAse(aseName, console);
				}
				catch(CErreur& erreur) {
					LOGWARN(("Echec d'import ASE : %s", erreur.what()));

					stringstream texte;
					texte << "Echec d'import ASE : " << erreur.what();
					AG_TextMsg(AG_MSG_ERROR, texte.str().c_str());
				}
			}
		}
		break;

		case OpenSceneASEEcraseRepOuiAction:
		{
			LOGINFO((" ==> action controller %d (OpenSceneASEEcraseRepOuiAction)", action));

			OpenSceneASEEcraseRepView* view = (OpenSceneASEEcraseRepView*)_agarView->getView(Viewer::OPEN_SCENE_ASE_ECRASE_REP_VIEW);

			try {
				// Suppression du répertoire
				string repName = view->getRepName();
				AseImporter::supprimeRepertoire(repName);

				// Import du fichier ASE
				ConsoleAvancementView* console = (ConsoleAvancementView*)_agarView->getView(Viewer::CONSOLE_AVANCEMENT_VIEW);
				console->init("Import fichier ASE");
				_agarView->showMenuView(Viewer::CONSOLE_AVANCEMENT_VIEW);

				string aseName = view->getAseName();
				AseImporter::lanceImportAse(aseName, console);
			}
			catch(CErreur &erreur) {
				LOGWARN(("Echec d'import ASE : %s", erreur.what()));

				stringstream texte;
				texte << "Echec d'import ASE : " << erreur.what();
				AG_TextMsg(AG_MSG_ERROR, texte.str().c_str());
			}
		}
		break;

		case OpenSceneASEEcraseRepNonAction:
		{
			LOGINFO((" ==> action controller %d (OpenSceneASEEcraseRepNonAction)", action));

			PluginActionEvent evt(Controller::Action::ShowMenuAction);
			_pluginEngine->dispatchEvent(evt);

			AG_TextMsg(AG_MSG_INFO, "Import annule");
		}
		break;

		case SaveConfigJoueurAction:
		{
			LOGINFO((" ==> action controller %d (SaveConfigJoueurAction)", action));

			ConfigurationJoueurView* view = (ConfigurationJoueurView*)_agarView->getView(Viewer::CONFIGURATION_JOUEUR_VIEW);
			Config.Joueur.nom = view->getJoueurName();
			Config.Joueur.mapName = view->getJoueurMapName();
			Config.Ecrit();
		}
		break;

		case SaveConfigReseauAction:
		{
			LOGINFO((" ==> action controller %d (SaveConfigReseauAction)", action));

			ConfigurationReseauView* view = (ConfigurationReseauView*)_agarView->getView(Viewer::CONFIGURATION_RESEAU_VIEW);
			Config.Reseau.setIpServer(view->getIpServeur());
			Config.Reseau.setPort(view->getPort());
			Config.Reseau.setPortTree(view->getPortTree());
			Config.Ecrit();

			AG_TextMsg(AG_MSG_WARNING, "Configuration mise a jour");
		}
		break;


		// Mise à jour de la configuration du centralisateur
		case SaveConfigCentralisateurAction:
		{
			LOGINFO((" ==> action controller %d (SaveConfigCentralisateurAction)", action));

			ConfigCentralisateurView* view = (ConfigCentralisateurView*)_agarView->getView(Viewer::CONFIG_CENTRALISATEUR_VIEW);
			Config.Centralisateur.setIp(view->getAdresse());
			Config.Centralisateur.setPort(view->getPort());
			Config.Ecrit();

			AG_TextMsg(AG_MSG_WARNING, "Configuration mise a jour");
		}
		break;

		// Mise à jour de la page des fichiers téléchargeables par le jeu
		case ReloadDownloadFilesAction:
		{
			LOGINFO((" ==> action controller %d (ReloadDownloadFilesAction)", action));

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
		case DownloadOneFileAction:
		{
			LOGINFO((" ==> action controller %d (DownloadOneFileAction)", action));

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
		case Refresh:
		{
			LOGINFO((" ==> action controller %d (Refresh)", action));

			View* view = (View*)AG_PTR(2);
			view->refresh();
		}
		break;

		case QuitAction:
			LOGINFO((" ==> action controller %d (QuitAction)", action));
			quit_game("Quit game user action", 0);
			break;

		case ExecuteUserCommandeAction:
		{
			LOGINFO((" ==> action controller %d (ExecuteUserCommandeAction)", action));

			ConsoleView* view = (ConsoleView*)_agarView->getView(Viewer::CONSOLE_VIEW);
			string commande = view->getCommandAndClearCommandLine();

			if(!JktUtils::StringUtils::isBlank(commande)) {
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

