#include <iostream>

using namespace std;

#include "menu/OpenCentralisateurView.h"
#include "main/Fabrique.h"
#include "centralisateur/Centralisateur.h"
#include "Portail.h"
#include "util/Trace.h"

#include "menu/Controller.h"

void quit_tutorial( int code );

const int Controller::MainMenuAction = 0;
const int Controller::OpenCentralisateurAction = 1;
const int Controller::OpenSceneAction = 2;
const int Controller::OpenSceneASEAction = 3;
const int Controller::OpenSceneMapAction = 4;
const int Controller::QuitAction = 5;
const int Controller::ConnectCentralisateurAction = 6;

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
    case OpenCentralisateurAction:
        m_agarView->showOpenCentralisateur();
        break;
    case OpenSceneAction:
		//AG_TextMsg(AG_MSG_WARNING, "Action not implemented %s!", "OpenSceneAction");
        m_agarView->showOpenScene();
        break;
    case OpenSceneASEAction:
		//AG_TextMsg(AG_MSG_WARNING, "Action not implemented %s!", "OpenSceneASEAction");
        m_agarView->showOpenSceneASE();
        break;
	case OpenSceneMapAction:
		//AG_TextMsg(AG_MSG_WARNING, "Action not implemented %s!", "OpenSceneMapAction");
        m_agarView->showOpenSceneMap();
        break;
	case ConnectCentralisateurAction:
        {
            OpenCentralisateurView* view = m_agarView->getOpenCentralisateurView();
            cout << "Connect [" << view->getUserName() << ", " << view->getAdresse() << ", " << view->getPort() << "]";
            Centralisateur* centralisateur = Fabrique::getCentralisateur();
            centralisateur->connecter(view->getUserName(),
                                      view->getAdresse(),
                                      view->getPort());
            centralisateur->sendSignalement();
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
