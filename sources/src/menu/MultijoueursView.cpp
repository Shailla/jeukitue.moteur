#include <agar/core.h>
#include <agar/gui.h>

#include "util/StringUtils.h"
#include "menu/View.h"
#include "menu/Controller.h"

#include "menu/MultijoueursView.h"

MultijoueursView::MultijoueursView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	AG_MutexInit(&_agMutex);

	_serverName = new char[41];
	_serverName[0] = '\0';

	_activeMap = new char[41];
	_activeMap[0] = '\0';

	_ping = -1;

	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
    AG_WindowSetCaption(m_window, "Multi-joueurs");

	// Fonctions client
	AG_Button* buttonRejoindre = AG_ButtonNewFn(m_window, 0, "Rejoindre la partie", controllerCallback, "%i", Controller::RejoindrePartieServerAction);
	AG_Button* buttonInfo = AG_ButtonNewFn(m_window, 0, "Obtenir info serveur", controllerCallback, "%i", Controller::InfoServerAction);
	AG_Button* buttonPing = AG_ButtonNewFn(m_window, 0, "Pinguer le serveur", controllerCallback, "%i", Controller::PingServerAction);
    AG_SeparatorNewHoriz(m_window);

	// Fonctions serveur
	AG_Button* buttonLancerServer = AG_ButtonNewFn(m_window, 0, "Lancer un serveur", controllerCallback, "%i", Controller::ShowLanceServeurViewAction);
	AG_SeparatorNewHoriz(m_window);

	// Fonctions centralisateur
	AG_Button* buttonCentralisateur = AG_ButtonNewFn(m_window, 0, "Connexion au centralisateur", controllerCallback, "%i", Controller::ShowCentralisateurViewAction);
    AG_SeparatorNewHoriz(m_window);

	// Bouton retour
    AG_Button* buttonRetour = AG_ButtonNewFn(m_window, 0, "Retour", controllerCallback, "%i", Controller::ShowMainMenuAction);

    // Informations serveur
    AG_SeparatorNewHoriz(m_window);
    AG_Label* labelServerName = AG_LabelNewPolledMT(m_window, 0, &_agMutex, "Nom du serveur : %s", _serverName);
    AG_Label* labelActiveMap = AG_LabelNewPolledMT(m_window, 0, &_agMutex, "Map active : %s", _activeMap);
    AG_Label* labelPing = AG_LabelNewPolledMT(m_window, 0, &_agMutex, "Ping : %i", &_ping);

    AG_ExpandHoriz(buttonRejoindre);
    AG_ExpandHoriz(buttonInfo);
    AG_ExpandHoriz(buttonPing);
    AG_ExpandHoriz(buttonLancerServer);
	AG_ExpandHoriz(buttonCentralisateur);
    AG_ExpandHoriz(labelServerName);
    AG_ExpandHoriz(labelActiveMap);
    AG_ExpandHoriz(labelPing);
    AG_ExpandHoriz(buttonRetour);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 25, 50);
	AG_WindowShow(m_window);
    hide();
}

MultijoueursView::~MultijoueursView(void) {
}

void MultijoueursView::setServerName(const std::string& serverName) {
	AG_MutexLock(&_agMutex);
	JktUtils::StringUtils::toChars(serverName, _serverName, 40);
	AG_MutexUnlock(&_agMutex);
}

void MultijoueursView::setActiveMap(const std::string& activeMap) {
	AG_MutexLock(&_agMutex);
	JktUtils::StringUtils::toChars(activeMap, _activeMap, 40);
	AG_MutexUnlock(&_agMutex);
}

void MultijoueursView::setPing(int ping) {
	AG_MutexLock(&_agMutex);
	_ping = ping;
	AG_MutexUnlock(&_agMutex);
}
