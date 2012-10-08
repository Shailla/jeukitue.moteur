
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

	_buttonMapTree = AG_ButtonNewFn(m_window, 0, "Arbre de la MAP", controllerCallback, "%i", Controller::MapTreeAction);
	_buttonErreurOpenGl = AG_ButtonNewFn(m_window, 0, "Derniere erreur OpenGL", controllerCallback, "%i", Controller::LastOpenGlErrorAction);

    AG_ExpandHoriz(_buttonMapTree);
	AG_ExpandHoriz(_buttonErreurOpenGl);

	// Bouton retour
    AG_SeparatorNewHoriz(m_window);

	_buttonRetour = AG_ButtonNewFn(m_window, 0, "Retour", controllerCallback, "%i", Controller::MainMenuAction);
    AG_ExpandHoriz(_buttonRetour);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 25, 50);
    hide();
}



DebugMenuView::~DebugMenuView(void)
{
}
