
#include <iostream>

#include <agar/core.h>
#include <agar/gui.h>

#include "util/StringUtils.h"
#include "menu/View.h"
#include "menu/Controller.h"
#include "reseau/NetworkManager.h"

#include "menu/MultijoueursClientView.h"

using namespace std;
using namespace JktNet;

MultijoueursClientView::MultijoueursClientView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	AG_MutexInit(&_agMutex);

	_serverName = new char[41];
	_serverName[0] = '\0';

	_activeMap = new char[41];
	_activeMap[0] = '\0';

	_ping = -1;

	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
    AG_WindowSetCaption(m_window, "Multi-joueurs en mode client");

	// Actions en mode connecté au serveur
    AG_Button* buttonConnect = AG_ButtonNewFn(m_window, 0, "Connecter", controllerCallback, "%i", Controller::ConnectClientAction);
    AG_Button* buttonRejoindre = AG_ButtonNewFn(m_window, 0, "Rejoindre la partie", controllerCallback, "%i", Controller::RejoindrePartieServerAction);
	AG_Button* buttonDeconnect = AG_ButtonNewFn(m_window, 0, "Deconnecter", controllerCallback, "%i", Controller::DeconnectClientAction);
	AG_SeparatorNewHoriz(m_window);

	// Actions en mode déconnecté du serveur
	AG_Button* buttonInfo = AG_ButtonNewFn(m_window, 0, "Obtenir info serveur", controllerCallback, "%i", Controller::InfoServerAction);
	AG_Button* buttonPing = AG_ButtonNewFn(m_window, 0, "Pinguer le serveur", controllerCallback, "%i", Controller::PingServerAction);
    AG_SeparatorNewHoriz(m_window);

    // Informations serveur
    AG_SeparatorNewHoriz(m_window);
    AG_Label* labelServerName = AG_LabelNewPolledMT(m_window, 0, &_agMutex, "Nom du serveur : %s", _serverName);
    AG_Label* labelActiveMap = AG_LabelNewPolledMT(m_window, 0, &_agMutex, "Map active : %s", _activeMap);
    AG_Label* labelPing = AG_LabelNewPolledMT(m_window, 0, &_agMutex, "Ping : %i", &_ping);

	// Bouton retour
    AG_Button* buttonRetour = AG_ButtonNewFn(m_window, 0, "Retour", controllerCallback, "%i", Controller::ShowMultijoueursMenuAction);

    AG_ExpandHoriz(buttonConnect);
    AG_ExpandHoriz(buttonRejoindre);
    AG_ExpandHoriz(buttonDeconnect);
    AG_ExpandHoriz(buttonInfo);
    AG_ExpandHoriz(buttonPing);
    AG_ExpandHoriz(labelServerName);
    AG_ExpandHoriz(labelActiveMap);
    AG_ExpandHoriz(labelPing);
    AG_ExpandHoriz(buttonRetour);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 25, 50);
	AG_WindowShow(m_window);
    hide();
}

MultijoueursClientView::~MultijoueursClientView(void) {
}

void MultijoueursClientView::show() {
	View::show();
}

void MultijoueursClientView::hide() {
	View::hide();
}

void MultijoueursClientView::setServerName(const std::string& serverName) {
	AG_MutexLock(&_agMutex);
	JktUtils::StringUtils::toChars(serverName, _serverName, 40);
	AG_MutexUnlock(&_agMutex);
}

void MultijoueursClientView::setActiveMap(const std::string& activeMap) {
	AG_MutexLock(&_agMutex);
	JktUtils::StringUtils::toChars(activeMap, _activeMap, 40);
	AG_MutexUnlock(&_agMutex);
}

void MultijoueursClientView::setPing(int ping) {
	AG_MutexLock(&_agMutex);
	_ping = ping;
	AG_MutexUnlock(&_agMutex);
}
