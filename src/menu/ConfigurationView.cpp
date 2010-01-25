#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"
#include "menu/Controller.h"

#include "menu/ConfigurationView.h"

ConfigurationView::ConfigurationView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(0);
    AG_WindowSetCaption(m_window, "Configuration");

    AG_Button* buttonCentralisateur = AG_ButtonNewFn(m_window, 0, "Centralisateur", controllerCallback, "%i", Controller::ConfigCentralisateurAction);
    AG_SeparatorNewHoriz(m_window);
    AG_Button* buttonRetour = AG_ButtonNewFn(m_window, 0, "Retour", controllerCallback, "%i", Controller::MainMenuAction);

    AG_ExpandHoriz(buttonCentralisateur);
    AG_ExpandHoriz(buttonRetour);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 50, 50);
	AG_WindowShow(m_window);
    hide();
}

ConfigurationView::~ConfigurationView(void)
{
}
