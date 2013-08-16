
#include <iostream>

#include <agar/core.h>
#include <agar/gui.h>

#include "util/StringUtils.h"
#include "menu/View.h"
#include "menu/Controller.h"
#include "reseau/NetworkManager.h"

#include "menu/MultijoueursView.h"

using namespace std;

MultijoueursView::MultijoueursView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
    AG_WindowSetCaption(m_window, "Multi-joueurs");

	// Fonctions client
	AG_Button* buttoModeClient = AG_ButtonNewFn(m_window, 0, "Mode client", controllerCallback, "%i", Controller::ShowMultijoueursClientMenuAction);
    AG_SeparatorNewHoriz(m_window);

	// Fonctions serveur
	AG_Button* buttonLancerServer = AG_ButtonNewFn(m_window, 0, "Lancer un serveur", controllerCallback, "%i", Controller::ShowLanceServeurViewAction);
	AG_SeparatorNewHoriz(m_window);

	// Fonctions centralisateur
	AG_Button* buttonCentralisateur = AG_ButtonNewFn(m_window, 0, "Connexion au centralisateur", controllerCallback, "%i", Controller::ShowCentralisateurViewAction);
    AG_SeparatorNewHoriz(m_window);

	// Bouton retour
    AG_Button* buttonRetour = AG_ButtonNewFn(m_window, 0, "Retour", controllerCallback, "%i", Controller::ShowMainMenuAction);

    AG_ExpandHoriz(buttoModeClient);
    AG_ExpandHoriz(buttonLancerServer);
	AG_ExpandHoriz(buttonCentralisateur);
    AG_ExpandHoriz(buttonRetour);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 25, 50);
	AG_WindowShow(m_window);
    hide();
}

MultijoueursView::~MultijoueursView(void) {
}

void MultijoueursView::show() {
	View::show();
}
