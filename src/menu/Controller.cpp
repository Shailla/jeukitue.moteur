#include <iostream>

using namespace std;

#include "menu/CentralisateurView.h"
#include "menu/ConfigCentralisateurView.h"
#include "menu/OpenSceneMapView.h"
#include "menu/OpenSceneASEView.h"
#include "main/Fabrique.h"
#include "centralisateur/Centralisateur.h"
#include "Menu/Portail.h"
#include "util/Trace.h"
#include "main/Game.h"
#include "main/Focus.h"
#include "main/Cfg.h"

#include "menu/Controller.h"

extern CGame Game;
extern CCfg Config;
extern JKT_PACKAGE_MENU::CFocus *pFocus;
extern bool Aide;
void quit_tutorial( int code );
bool openMAP(const void *arg);

Viewer* Controller::m_agarView;

Controller::Controller(Viewer* agarView)
{
    m_agarView = agarView;
}

Controller::~Controller(void)
{
}

void Controller::executeAction(AG_Event *event)
{
    int action = AG_INT(1);

    switch(action)
    {
	case MainMenuAction:
		m_agarView->showMainMenu();
		break;

	case MultijoueursAction:
		m_agarView->showMultijoueurs();
		break;

    case ConfigurationAction:
        m_agarView->showConfiguration();
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

	// Ouverture d'une Map en mode de jeu local
	case OpenMapAction:
        {
            // Récupération du nom de la Map à ouvrir
			int mapNumber = AG_INT(2);
			OpenSceneMapView* view = m_agarView->getOpenSceneMapView();
			string mapName = view->getMapName(mapNumber);

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
			{		// Si un répertoire existe déjà, demande s'il faut l'écraser
				m_agarView->showOpenSceneASEEcraseRep();
			}
			else {
				// TODO Import du fichier ASE
			}
        }
        break;

	case OpenSceneASEEcraseRepOuiAction:
		break;

	case OpenSceneASEEcraseRepNonAction:
		break;


	// Mise à jour de la configuration du centralisateur
	case SaveConfigCentralisateurAction:
		{
            ConfigCentralisateurView* view = m_agarView->getConfigCentralisateurView();
			Config.Centralisateur.setIp(view->getAdresse());
			Config.Centralisateur.setPort(view->getPort());
			Config.Ecrit();

			AG_TextMsg(AG_MSG_WARNING, "Configuration mise à jour");
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

void Controller::openMap(const string& mapName)
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
