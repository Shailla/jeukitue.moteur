#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/style.h>

#include "menu/View.h"
#include "menu/Controller.h"
#include "menu/Viewer.h"

#include "menu/OpenSceneView.h"

OpenSceneView::OpenSceneView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(0);
    AG_WindowSetCaption(m_window, "Ouvrir une scene");

	m_buttonConvertirSceneASE = AG_ButtonNewFn(m_window, 0, "Importer une scene ASE", controllerCallback, "%i", Controller::OpenSceneASEAction);
    m_buttonOuvrirSceneMap = AG_ButtonNewFn(m_window, 0, "Ouvrir une scene Map", controllerCallback, "%i", Controller::OpenSceneMapAction);
    AG_SeparatorNewHoriz(m_window);
    m_buttonRetour = AG_ButtonNewFn(m_window, 0, "Retour", controllerCallback, "%i", Controller::MainMenuAction);

    AG_ExpandHoriz(m_buttonConvertirSceneASE);
    AG_ExpandHoriz(m_buttonOuvrirSceneMap);
    AG_ExpandHoriz(m_buttonRetour);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 50, 50);
	AG_WindowShow(m_window);
    hide();
}


OpenSceneView::~OpenSceneView(void)
{
}
