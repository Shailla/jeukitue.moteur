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

	AG_Button* buttonMultijoueurs = AG_ButtonNewFn(m_window, 0, "Lancer un serveur", controllerCallback, "%i", Controller::LanceServeurAction);
    AG_Button* buttonCentralisateur = AG_ButtonNewFn(m_window, 0, "Connexion au centralisateur", controllerCallback, "%i", Controller::CentralisateurAction);
    AG_SeparatorNewHoriz(m_window);
    AG_Button* buttonRetour = AG_ButtonNewFn(m_window, 0, "Retour", controllerCallback, "%i", Controller::MainMenuAction);

    AG_ExpandHoriz(buttonMultijoueurs);
	AG_ExpandHoriz(buttonCentralisateur);
    AG_ExpandHoriz(buttonRetour);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 50, 50);
	AG_WindowShow(m_window);
    hide();
}

MultijoueursView::~MultijoueursView(void)
{
}
