#include <iostream>

using namespace std;

#include "OpenCentralisateurView.h"
#include "Fabrique.h"
#include "Centralisateur.h"

#include "AgarController.h"

const int AgarController::OpenCentralisateurAction = 1;
const int AgarController::OpenSceneAction = 2;
const int AgarController::QuitAction = 3;
const int AgarController::ConnectCentralisateurAction = 4;

AgarView* AgarController::m_agarView;

AgarController::AgarController(AgarView* agarView)
{
    m_agarView = agarView;
}

AgarController::~AgarController(void)
{
}

void AgarController::executeAction(AG_Event *event)
{
    int action = AG_INT(1);

    switch(action)
    {
    case OpenSceneAction:
        AG_TextMsg(AG_MSG_WARNING, "Action not implemented %s!", "OpenSceneAction");
        break;
    case OpenCentralisateurAction:
        m_agarView->showOpenCentralisateur();
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
        AG_TextMsg(AG_MSG_WARNING, "Action not implemented %s!", "QuitAction");
        break;
    default:
        AG_TextMsg(AG_MSG_WARNING, "Unknown action : %d!", action);
        break;
    }
}
