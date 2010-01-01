#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/style.h>

#include "menu/View.h"
#include "menu/Controller.h"
#include "menu/Viewer.h"

#include "menu/OpenSceneASEView.h"

OpenSceneASEView::OpenSceneASEView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(0);
    AG_WindowSetCaption(m_window, "Ouvrir une scene ASE");
	AG_LabelNewStatic(m_window, 0, "Not-implemented");

    AG_SeparatorNewHoriz(m_window);
	m_buttonRetour = AG_ButtonNewFn(m_window, 0, "Retour", controllerCallback, "%d", Controller::OpenSceneAction);
	AG_ExpandHoriz(m_buttonRetour);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 50, 50);
	AG_WindowShow(m_window);
    hide();
}


OpenSceneASEView::~OpenSceneASEView(void)
{
}
