
#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"
#include "menu/Controller.h"

#include "menu/DebugMenuView.h"

#include <sstream>

DebugMenuView::DebugMenuView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
	AG_WindowSetCaption(m_window, "Debug");

	AG_Button* _buttonMapTree = AG_ButtonNewFn(m_window, 0, "Arbre de la MAP", controllerCallback, "%i", Controller::ShowMapTreeViewAction);
	AG_Button* _buttonErreurOpenGl = AG_ButtonNewFn(m_window, 0, "Derni\u00e8re erreur OpenGL", controllerCallback, "%i", Controller::ShowLastOpenGlErrorViewAction);
	AG_Button* _buttonDataTree = AG_ButtonNewFn(m_window, 0, "Arbre donn\u00e9es", controllerCallback, "%i", Controller::ShowDataTreeViewAction);
	AG_Button* _buttonPlayersList = AG_ButtonNewFn(m_window, 0, "Liste des joueurs", controllerCallback, "%i", Controller::ShowPlayersListViewAction);
	AG_Button* _buttonNetworkConnections = AG_ButtonNewFn(m_window, 0, "Connexions r\u00e9seau", controllerCallback, "%i", Controller::ShowNetworkConnectionsViewAction);

    AG_ExpandHoriz(_buttonMapTree);
	AG_ExpandHoriz(_buttonErreurOpenGl);
	AG_ExpandHoriz(_buttonDataTree);
	AG_ExpandHoriz(_buttonPlayersList);
	AG_ExpandHoriz(_buttonNetworkConnections);

	// Bouton retour
    AG_SeparatorNewHoriz(m_window);

    AG_Button* _buttonRetour = AG_ButtonNewFn(m_window, 0, "Retour", controllerCallback, "%i", Controller::ShowMenuAction);
    AG_ExpandHoriz(_buttonRetour);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 25, 50);
    hide();
}

DebugMenuView::~DebugMenuView(void)
{
}
