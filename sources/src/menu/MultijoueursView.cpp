#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"
#include "menu/Controller.h"

#include "menu/MultijoueursView.h"

MultijoueursView::MultijoueursView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
    AG_WindowSetCaption(m_window, "Multi-joueurs");


	// Fonctions serveur
	AG_Button* buttonMultijoueurs = AG_ButtonNewFn(m_window, 0, "Lancer un serveur", controllerCallback, "%i", Controller::ShowLanceServeurViewAction);
	AG_SeparatorNewHoriz(m_window);

	// Fonctions centralisateur
	AG_Button* buttonCentralisateur = AG_ButtonNewFn(m_window, 0, "Connexion au centralisateur", controllerCallback, "%i", Controller::ShowCentralisateurViewAction);
    AG_SeparatorNewHoriz(m_window);

	// Fonctions client
	AG_Button* buttonRejoindre = AG_ButtonNewFn(m_window, 0, "Rejoindre la partie", controllerCallback, "%i", Controller::RejoindrePartieServerAction);
	AG_Button* buttonInfo = AG_ButtonNewFn(m_window, 0, "Obtenir info serveur", controllerCallback, "%i", Controller::InfoServerAction);
	AG_Button* buttonPing = AG_ButtonNewFn(m_window, 0, "Pinguer le serveur", controllerCallback, "%i", Controller::PingServerAction);
    AG_SeparatorNewHoriz(m_window);

    AG_Label*

	// Bouton retour
    AG_Button* buttonRetour = AG_ButtonNewFn(m_window, 0, "Retour", controllerCallback, "%i", Controller::ShowMainMenuAction);

    AG_ExpandHoriz(buttonMultijoueurs);
	AG_ExpandHoriz(buttonCentralisateur);
    AG_ExpandHoriz(buttonRejoindre);
    AG_ExpandHoriz(buttonInfo);
    AG_ExpandHoriz(buttonPing);
    AG_ExpandHoriz(buttonRetour);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 25, 50);
	AG_WindowShow(m_window);
    hide();
}

MultijoueursView::~MultijoueursView(void) {
}
