
#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/style.h>

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
	AG_Button* _buttonErreurOpenGl = AG_ButtonNewFn(m_window, 0, "Derniere erreur OpenGL", controllerCallback, "%i", Controller::ShowLastOpenGlErrorViewAction);
	AG_Button* _buttonDataTree = AG_ButtonNewFn(m_window, 0, "Arbre donnees", controllerCallback, "%i", Controller::ShowDataTreeViewAction);


    AG_ExpandHoriz(_buttonMapTree);
	AG_ExpandHoriz(_buttonErreurOpenGl);
	AG_ExpandHoriz(_buttonDataTree);

	// Bouton retour
    AG_SeparatorNewHoriz(m_window);

    AG_Button* _buttonRetour = AG_ButtonNewFn(m_window, 0, "Retour", controllerCallback, "%i", Controller::ShowMainMenuAction);
    AG_ExpandHoriz(_buttonRetour);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 25, 50);
    hide();
}

DebugMenuView::~DebugMenuView(void)
{
}
