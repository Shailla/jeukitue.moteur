#include <iostream>
#include <vector>

using namespace std;

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#include <agar/core.h>
#include <agar/gui.h>
#include <agar/dev.h>

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

#include "menu/Controller.h"

using namespace JktMenu;
using namespace JktMoteur;
using namespace JktPlugin;
using namespace JktNet;

extern CGame Game;
extern CCfg Config;
extern NetworkManager Reseau;
void quit_game(int code);

Viewer* Controller::m_agarView;

Controller::Controller(Viewer* agarView) {
	m_agarView = agarView;
}

Controller::~Controller(void) {
}

void Controller::executeAction(AG_Event *event) {
	int action = AG_INT(1);

	switch(action) {
	case ShowMainMenuAction:
		m_agarView->showMenuView(Viewer::MAIN_MENU_VIEW);
		break;

	case ShowMultijoueursMenuAction:
		m_agarView->showMenuView(Viewer::MULTIJOUEURS_VIEW);
		break;

	case ShowMultijoueursClientMenuAction:
		m_agarView->showMenuView(Viewer::MULTIJOUEURS_CLIENT_VIEW);
		break;

	case ShowConfigurationMenuAction:
		m_agarView->showMenuView(Viewer::CONFIGURATION_VIEW);
		break;

	case QuitGameAction:
		Game.Quit();
		break;

	case ShowConfigurationVideoViewAction:
		m_agarView->showMenuView(Viewer::CONFIGURATION_VIDEO_VIEW);
		break;

	case ShowConfigurationReseauViewAction:
		m_agarView->showMenuView(Viewer::CONFIGURATION_RESEAU_VIEW);
		break;

	case ShowConfigurationJoueurViewAction:
		m_agarView->showMenuView(Viewer::CONFIGURATION_JOUEUR_VIEW);
		break;

	case ShowConfigurationCommandes:
		m_agarView->showMenuView(Viewer::CONFIGURATION_COMMANDES_VIEW);
		break;

	case WaitUserCommandChoice:
	{
		int commandId = AG_INT(2);

		ConfigurationCommandesView::beginWaitUserCommandChoice(commandId);

		// Réactualise la fenêtre
		m_agarView->showMenuView(Viewer::CONFIGURATION_COMMANDES_VIEW);
	}
	break;

	case ShowConfigurationAudio:
		m_agarView->showMenuView(Viewer::CONFIGURATION_AUDIO_VIEW);
		break;

	case ShowCentralisateurViewAction:
	{
		Centralisateur* centralisateur = Fabrique::getCentralisateur();
		centralisateur->connecter((char*)Config.Joueur.nom.c_str(),
				(char*)Config.Centralisateur.getIp().c_str(),
				Config.Centralisateur.getPort());
		centralisateur->sendSignalement();

		m_agarView->showMenuView(Viewer::CENTRALISATEUR_VIEW);
	}
	break;

	case ShowMenuConfigCentralisateurAction:
		m_agarView->showMenuView(Viewer::CONFIG_CENTRALISATEUR_VIEW);
		break;

		// Envoi d'un message de chat sur le serveur centralisateur
	case SendGlobalChatTextAction:
	{
		CentralisateurView* view = (CentralisateurView*)m_agarView->getView(Viewer::CENTRALISATEUR_VIEW);
		const char* textToSend = view->getTextToSend();
		Centralisateur* centralisateur = Fabrique::getCentralisateur();
		centralisateur->sendMessageChat(string(textToSend), Config.Joueur.nom.c_str());
		view->clearTextToSend();
	}
	break;

	case ShowMenuOpenSceneAction:
		m_agarView->showMenuView(Viewer::OPEN_SCENE_VIEW);
		break;

	case ShowPluginsManagementViewAction:
		m_agarView->showMenuView(Viewer::PLUGINS_MANAGEMENT_VIEW);
		break;

	case ShowAboutViewAction:
		m_agarView->showMenuView(Viewer::ABOUT_VIEW);
		break;

	case ShowOpenAseViewAction:
		m_agarView->showMenuView(Viewer::OPEN_SCENE_ASE_VIEW);
		break;

		// Ouvre l'écran de choix d'une Map en mode de jeu local
	case ShowOpenMapViewAction:
		m_agarView->showMenuView(Viewer::OPEN_SCENE_MAP_VIEW);
		break;

		// Ecran de choix d'une Map à lancer en mode serveur
	case ShowLanceServeurViewAction:
		m_agarView->showMenuView(Viewer::LANCE_SERVEUR_VIEW);
		break;

		// Ouvere une MAP en mode serveur
	case LanceServeurMapAction:
	{
		// Récupération du nom de la Map à ouvrir
		int mapNumber = AG_INT(2);
		LanceServeurView* view = (LanceServeurView*)m_agarView->getView(Viewer::LANCE_SERVEUR_VIEW);
		string mapName = view->getMapName(mapNumber);

		cout << endl << "Ouverture de la Map '" << mapName << "'";

		// Ouverture de la Map
		Game.RequeteProcess.setOuvreMapServer(mapName);
	}
	break;

	// Ouverture d'une Map en mode de jeu local
	case OpenMapAction:
	{
		// Récupération du nom de la Map à ouvrir
		int mapNumber = AG_INT(2);
		OpenSceneMapView* view = (OpenSceneMapView*)m_agarView->getView(Viewer::OPEN_SCENE_MAP_VIEW);
		string mapName = view->getMapName(mapNumber);

		cout << endl << "Ouverture de la Map '" << mapName << "'";

		// Ouverture de la Map
		Game.RequeteProcess.setOuvreMapLocal(mapName);
	}
	break;

	case RejoindrePartieServerAction:
	{
		CClient* client = Reseau.getClient();

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
		MultijoueursClientView* view = (MultijoueursClientView*)m_agarView->getView(Viewer::MULTIJOUEURS_CLIENT_VIEW);
		view->setServerName("");
		view->setActiveMap("");
		CClient* client = Reseau.getClient();

		if(client) {
			client->sendNotConnectedRequestInfoToServer();		// Demande ses infos au serveur
		}
		else {
			AG_TextMsg(AG_MSG_ERROR, "Action impossible, vous n'etes pas connecte");
		}
	}
	break;

	case PingServerAction:
	{
		MultijoueursClientView* view = (MultijoueursClientView*)m_agarView->getView(Viewer::MULTIJOUEURS_CLIENT_VIEW);
		view->setPing(-1);
		CClient* client = Reseau.getClient();

		if(client) {
			client->sendNotConnectedRequestPingToServer();				// Demande au serveur de répondre à un ping
		}
		else {
			AG_TextMsg(AG_MSG_ERROR, "Action impossible, vous n'etes pas connecte");
		}
	}
	break;

	case ConnectClientAction:
	{
		if( !Reseau.ouvreClient() ) {
			Reseau.setOn( false );		// Signale que le réseau ne peut pas être utilisé
			AG_TextMsg(AG_MSG_ERROR, "Echec de connexion au serveur");
		}
	}
	break;

	case DeconnectClientAction:
	{
		Reseau.fermeClient();
	}
	break;

	// Activation d'un plugin
	case PluginActivateAction:
	{
		// Récupération du nom de la Map à ouvrir
		int pluginNumber = AG_INT(2);
		PluginsManagementView* view = (PluginsManagementView*)m_agarView->getView(Viewer::PLUGINS_MANAGEMENT_VIEW);
		string pluginName = view->getPluginName(pluginNumber);

		cout << endl << "Activate plugin : '" << pluginName << "'";

		PluginEngine* pluginEngine = Fabrique::getPluginEngine();
		pluginEngine->activatePlugin(pluginName);
	}
	break;

	// Désactivation d'un plugin
	case PluginDeactivateAction:
	{
		// Récupération du nom de la Map à ouvrir
		int pluginNumber = AG_INT(2);
		PluginsManagementView* view = (PluginsManagementView*)m_agarView->getView(Viewer::PLUGINS_MANAGEMENT_VIEW);
		string pluginName = view->getPluginName(pluginNumber);

		cout << endl << "Deactivate plugin : '" << pluginName << "'";

		PluginEngine* pluginEngine = Fabrique::getPluginEngine();
		pluginEngine->deactivatePlugin(pluginName);
	}
	break;

	// Menu de configuration d'Agar
	case ShowAgarConfigurationViewAction:
	{
		DEV_ConfigShow();
	}
	break;

	// Affiche le menu de debug
	case ShowDebugViewAction:
	{
		m_agarView->showMenuView(Viewer::DEBUG_MENU_VIEW);
	}
	break;

	// Affiche les données de la MAP courante sous forme d'un arbre
	case ShowMapTreeViewAction:
	{
		m_agarView->showMenuView(Viewer::MAP_TREE_VIEW);
	}
	break;

	// Affiche les données de la MAP courante sous forme d'un arbre
	case ShowDataTreeViewAction:
	{
		m_agarView->showMenuView(Viewer::DATA_TREE_VIEW);
	}
	break;

	case ShowPlayersListViewAction:
	{
		m_agarView->showMenuView(Viewer::PLAYERS_LIST_VIEW);
	}
	break;

	// Affiche la denière erreur OpenGL dans une popup
	case ShowLastOpenGlErrorViewAction:
	{
		stringstream openGlError;
		openGlError << "Deniere erreur OpenGL : '" << gluErrorString(glGetError()) << "'";
		cerr << endl << __FILE__ << ":" << __LINE__ << " " << openGlError.str();
		AG_TextMsg(AG_MSG_ERROR, openGlError.str().c_str());
	}
	break;

	// Import d'un fichier ASE
	case ImportAseAction:
	{
		// Récupération du nom de la Map à ouvrir
		int aseNumber = AG_INT(2);
		OpenSceneASEView* view = (OpenSceneASEView*)m_agarView->getView(Viewer::OPEN_SCENE_ASE_VIEW);
		string aseName = view->getAseName(aseNumber);

		// Ouverture de la Map
		string nomRep = "./Map/" + aseName + "/";

		if(CFindFolder::mkdir(nomRep.c_str()) != 0)	{	// Création du répertoire pour les textures
			// Si un répertoire existe déjà, demande s'il faut l'écraser
			OpenSceneASEEcraseRepView* view = (OpenSceneASEEcraseRepView*)m_agarView->getView(Viewer::OPEN_SCENE_ASE_ECRASE_REP_VIEW);
			view->setRepName(nomRep);	// Nom du répertoire du fichier Ase
			view->setAseName(aseName);	// Nom du fichier Ase

			m_agarView->showMenuView(Viewer::OPEN_SCENE_ASE_ECRASE_REP_VIEW);
		}
		else {
			try {
				ConsoleAvancementView* console = (ConsoleAvancementView*)m_agarView->getView(Viewer::CONSOLE_AVANCEMENT_VIEW);
				console->init("Import fichier ASE");
				m_agarView->showMenuView(Viewer::CONSOLE_AVANCEMENT_VIEW);

				// Import du fichier ASE
				AseImporter::lanceImportAse(aseName, console);
			}
			catch(CErreur& erreur) {
				stringstream texte;
				texte << "Echec d'import ASE : " << erreur.toString();
				AG_TextMsg(AG_MSG_ERROR, texte.str().c_str());
			}
		}
	}
	break;

	case OpenSceneASEEcraseRepOuiAction:
	{
		OpenSceneASEEcraseRepView* view = (OpenSceneASEEcraseRepView*)m_agarView->getView(Viewer::OPEN_SCENE_ASE_ECRASE_REP_VIEW);

		try {
			// Suppression du répertoire
			string repName = view->getRepName();
			AseImporter::supprimeRepertoire(repName);

			// Import du fichier ASE
			ConsoleAvancementView* console = (ConsoleAvancementView*)m_agarView->getView(Viewer::CONSOLE_AVANCEMENT_VIEW);
			console->init("Import fichier ASE");
			m_agarView->showMenuView(Viewer::CONSOLE_AVANCEMENT_VIEW);

			string aseName = view->getAseName();
			AseImporter::lanceImportAse(aseName, console);
		}
		catch(CErreur &erreur) {
			stringstream texte;
			texte << "Echec d'import ASE : " << erreur.toString();
			AG_TextMsg(AG_MSG_ERROR, texte.str().c_str());
		}
	}
	break;

	case OpenSceneASEEcraseRepNonAction:
		m_agarView->showMenuView(Viewer::MAIN_MENU_VIEW);
		AG_TextMsg(AG_MSG_INFO, "Import annule");
		break;

	case SaveConfigJoueurAction:
	{
		ConfigurationJoueurView* view = (ConfigurationJoueurView*)m_agarView->getView(Viewer::CONFIGURATION_JOUEUR_VIEW);
		Config.Joueur.nom = view->getJoueurName();
		Config.Joueur.mapName = view->getJoueurMapName();
		Config.Ecrit();
	}
	break;

	case SaveConfigReseauAction:
	{
		ConfigurationReseauView* view = (ConfigurationReseauView*)m_agarView->getView(Viewer::CONFIGURATION_RESEAU_VIEW);
		Config.Reseau.setIpServer(view->getIpServeur());
		Config.Reseau.setPort(view->getPort());
		Config.Reseau.setPortTree(view->getPortTree());
		Config.Reseau.setServeur(view->isModeServeur());
		Config.Ecrit();

		AG_TextMsg(AG_MSG_WARNING, "Configuration mise a jour");
	}
	break;


	// Mise à jour de la configuration du centralisateur
	case SaveConfigCentralisateurAction:
	{
		ConfigCentralisateurView* view = (ConfigCentralisateurView*)m_agarView->getView(Viewer::CONFIG_CENTRALISATEUR_VIEW);
		Config.Centralisateur.setIp(view->getAdresse());
		Config.Centralisateur.setPort(view->getPort());
		Config.Ecrit();

		AG_TextMsg(AG_MSG_WARNING, "Configuration mise a jour");
	}
	break;

	// Mise à jour de la page des fichiers téléchargeables par le jeu
	case ReloadDownloadFilesAction:
	{
		try {
			Centralisateur* centralisateur = Fabrique::getCentralisateur();
			vector<DownloadFileItem> items = centralisateur->askDownloadFileList(4635);

			CentralisateurView* view = (CentralisateurView*)m_agarView->getView(Viewer::CENTRALISATEUR_VIEW);
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
		long downloadId = AG_LONG(2);

		try {
			Centralisateur* centralisateur = Fabrique::getCentralisateur();

			ProgressBarView* view = (ProgressBarView*)m_agarView->getView(Viewer::PROGRESS_BAR_VIEW);

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
		View* view = (View*)AG_PTR(2);
		view->refresh();
	}
	break;

	case QuitAction:
		quit_game(0);
		break;

	case ExecuteUserCommandeAction:
	{
		ConsoleView* view = (ConsoleView*)m_agarView->getView(Viewer::CONSOLE_VIEW);
		string commande = view->getCommandAndClearCommandLine();

		if(!JktUtils::StringUtils::isBlank(commande)) {
			Fabrique::getCommandeInterpreter()->interpreteCommande(commande, true);
		}

		break;
	}

	default:
		TRACE().p( TRACE_ERROR, "Unknown action : %d!", action );
		AG_TextMsg(AG_MSG_WARNING, "Action inconnue : %d!", action);
		break;
	}
}

