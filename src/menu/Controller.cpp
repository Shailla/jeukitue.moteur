#include <iostream>

using namespace std;

#include "menu/CentralisateurView.h"
#include "menu/ConfigCentralisateurView.h"
#include "menu/OpenSceneMapView.h"
#include "menu/OpenSceneASEView.h"
#include "menu/OpenSceneASEEcraseRepView.h"
#include "menu/ConsoleAvancementView.h"
#include "menu/ConfigurationJoueurView.h"
#include "menu/LanceServeurView.h"
#include "main/Fabrique.h"
#include "centralisateur/Centralisateur.h"
#include "Menu/Portail.h"
#include "util/Trace.h"
#include "main/Game.h"
#include "main/Focus.h"
#include "main/Cfg.h"
#include "reseau/SPA.h"
#include "reseau/Client.h"
#include "reseau/Server.h"
#include "reseau/Reseau.h"
#include "spatial/Map.h"
#include "spatial/materiau/MaterialTexture.h"
#include "spatial/materiau/MaterialMulti.h"
#include "spatial/AseImporter.h"
#include "util/FindFolder.h"

#include "menu/Controller.h"

using namespace JKT_PACKAGE_MENU;
using namespace JKT_PACKAGE_MOTEUR3D;

extern CGame Game;
extern CCfg Config;
extern JKT_PACKAGE_MENU::CFocus *pFocus;
extern bool Aide;
void quit_tutorial( int code );
bool copieTexture( CMaterialTexture *mat, CMap *pMapASE, string &nomRep );
void openMap(const std::string& mapName);
bool openMAP( const void *arg );
void openMAP2( const string &nomFichierMap );
extern JKT_PACKAGE_RESEAU::CReseau Reseau;

Viewer* Controller::m_agarView;

Controller::Controller(Viewer* agarView) {
    m_agarView = agarView;
}

Controller::~Controller(void) {
}

void Controller::executeAction(AG_Event *event) {
    int action = AG_INT(1);

    switch(action) {
	case MainMenuAction:
		m_agarView->showMainMenu();
		break;

	case MultijoueursAction:
		m_agarView->showMultijoueurs();
		break;

    case ConfigurationAction:
        m_agarView->showConfiguration();
        break;

	case ConfigurationVideoAction:
		m_agarView->showConfigurationVideoView();
		break;

	case ConfigurationJoueurAction:
		m_agarView->showConfigurationJoueurView();
		break;

    case CentralisateurAction:
		{
			Centralisateur* centralisateur = Fabrique::getCentralisateur();
			centralisateur->connecter((char*)Config.Joueur.nom.c_str(),
			                          (char*)Config.Centralisateur.getIp().c_str(),
				                      Config.Centralisateur.getPort());
	        centralisateur->sendSignalement();

			m_agarView->showCentralisateur();
		}
        break;

    case ConfigCentralisateurAction:
        m_agarView->showConfigCentralisateur();
        break;

	// Envoi d'un message de chat sur le serveur centralisateur
	case SendGlobalChatTextAction:
		{
			CentralisateurView* view = m_agarView->getCentralisateurView();
			const char* textToSend = view->getTextToSend();
			Centralisateur* centralisateur = Fabrique::getCentralisateur();
			centralisateur->sendMessageChat(string(textToSend), Config.Joueur.nom.c_str());
			view->clearTextToSend();
		}
		break;

    case OpenSceneAction:
        m_agarView->showOpenScene();
        break;

    case OpenSceneASEAction:
        m_agarView->showOpenSceneASE();
        break;

	// Ouverture de l'écran de choix d'une Map en mode de jeu local
	case OpenSceneMapAction:
        m_agarView->showOpenSceneMap();
        break;

	// Ecran de choix d'une Map à lancer en mode serveur
	case LanceServeurAction:
		m_agarView->showLanceServeurView();
		break;

	case LanceServeurMapAction:
        {
            // Récupération du nom de la Map à ouvrir
			int mapNumber = AG_INT(2);
			LanceServeurView* view = m_agarView->getLanceServeurView();
			string mapName = view->getMapName(mapNumber);

			// Connexion du serveur
			if( !Reseau.ouvreServer( Config.Reseau.getPort() ) )
			{
				AG_TextMsg(AG_MSG_ERROR, "Echec de connexion du serveur");
			}
			else
			{
				// Ouverture de la Map sur le serveur
				JKT_PACKAGE_RESEAU::CServer *server = Game.getServer();
				pFocus->SetPlayFocus();
				Aide = false;
				server->nomMAP = mapName;		// Informe le serveur sur le nom de la MAP lancée
				openMAP2( server->nomMAP );		// Ouvre cette MAP
				Game.setPlayerList( server->maxPlayers );
				server->setStatut( JKT_PACKAGE_RESEAU::JKT_STATUT_SERVER_PLAY );
				server->bGame = true;				// Indique qu'une partie est en cours
			}
        }
		break;

	// Ouverture d'une Map en mode de jeu local
	case OpenMapAction:
        {
            // Récupération du nom de la Map à ouvrir
			int mapNumber = AG_INT(2);
			OpenSceneMapView* view = m_agarView->getOpenSceneMapView();
			string mapName = view->getMapName(mapNumber);

			cout << "Ouverture de la Map '" << mapName << "'";

			// Ouverture de la Map
            Game.RequeteProcess.setOuvreMapLocal(mapName);
        }
        break;

	// Import d'un fichier ASE
	case OpenASEAction:
        {
            // Récupération du nom de la Map à ouvrir
			int aseNumber = AG_INT(2);
			OpenSceneASEView* view = m_agarView->getOpenSceneASEView();
			string aseName = view->getAseName(aseNumber);

			// Ouverture de la Map
			string nomRep = "./Map/" + aseName;

			if( CFindFolder::mkdir( nomRep.c_str() )!=0 )	// Création du répertoire pour les textures
			{	
				// Si un répertoire existe déjà, demande s'il faut l'écraser
				OpenSceneASEEcraseRepView* view = m_agarView->getOpenSceneASEEcraseRepView();
				view->setRepName(nomRep);	// Nom du répertoire du fichier Ase
				view->setAseName(aseName);	// Nom du fichier Ase

				m_agarView->showOpenSceneASEEcraseRep();
			}
			else {
				try {
					ConsoleAvancementView* console = m_agarView->getConsoleAvancementView();
					console->init("Import fichier ASE");
					m_agarView->showConsoleAvancementView();

					// Import du fichier ASE
					AseImporter::lanceImportAse(aseName, console);
				}
				catch(CErreur erreur)
				{
					stringstream texte;
					texte << "Echec d'import ASE : " << erreur.toString();
					AG_TextMsg(AG_MSG_ERROR, texte.str().c_str());
				}
			}
        }
        break;

	case OpenSceneASEEcraseRepOuiAction:
		{
			OpenSceneASEEcraseRepView* view = m_agarView->getOpenSceneASEEcraseRepView();
			
			try {
				// Suppression du répertoire
				string repName = view->getRepName();
				AseImporter::supprimeRepertoire(repName);
				
				// Import du fichier ASE
				ConsoleAvancementView* console = m_agarView->getConsoleAvancementView();
				console->init("Import fichier ASE");
				m_agarView->showConsoleAvancementView();

				string aseName = view->getAseName();
				AseImporter::lanceImportAse(aseName, console);
			}
			catch(CErreur erreur)
			{
				stringstream texte;
				texte << "Echec d'import ASE : " << erreur.toString();
				AG_TextMsg(AG_MSG_ERROR, texte.str().c_str());
			}
		}
		break;

	case OpenSceneASEEcraseRepNonAction:
		m_agarView->showMainMenu();
		AG_TextMsg(AG_MSG_INFO, "Import annule");
		break;

	case SaveConfigJoueurAction:
		{
			ConfigurationJoueurView* view = m_agarView->getConfigurationJoueurView();
			Config.Joueur.nom = view->getJoueurName();
			Config.Joueur.mapName = view->getJoueurMapName();
			Config.Ecrit();
		}
		break;


	// Mise à jour de la configuration du centralisateur
	case SaveConfigCentralisateurAction:
		{
            ConfigCentralisateurView* view = m_agarView->getConfigCentralisateurView();
			Config.Centralisateur.setIp(view->getAdresse());
			Config.Centralisateur.setPort(view->getPort());
			Config.Ecrit();

			AG_TextMsg(AG_MSG_WARNING, "Configuration mise a jour");
		}
		break;

    case QuitAction:
		quit_tutorial( 0 );
		break;

    default:
		TRACE().p( TRACE_ERROR, "Unknown action : %d!", action );
        AG_TextMsg(AG_MSG_WARNING, "Unknown action : %d!", action);
        break;
    }
}

void openMap(const string& mapName)
{
	if(openMAP( mapName.c_str() )) {
		Aide = false;
		pFocus->SetPlayFocus();
		Game.setModeLocal();
	}
	else {
		AG_TextMsg(AG_MSG_WARNING, "Echec d'ouverture de la Map %s", mapName);
	}
}

