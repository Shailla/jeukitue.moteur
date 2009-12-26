#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/style.h>

#include "View.h"
#include "Controller.h"
#include "Viewer.h"

#include "OpenSceneView.h"

OpenSceneView::OpenSceneView(const AG_EventFn controllerCallback)
{
	m_window = AG_WindowNew(0);
    AG_WindowSetCaption(m_window, "Ouvrir une scene");

	m_buttonConvertirSceneASE = AG_ButtonNewFn(m_window, 0, "Convertir une scene ASE", controllerCallback, "%d", Controller::OpenSceneASEAction);
    m_buttonOuvrirSceneMap = AG_ButtonNewFn(m_window, 0, "Ouvrir une scene Map", controllerCallback, "%d", Controller::OpenSceneMapAction);
    m_buttonRetour = AG_ButtonNewFn(m_window, 0, "Retour", controllerCallback, "%d", Controller::MainMenuAction);

    AG_ExpandHoriz(m_buttonConvertirSceneASE);
    AG_ExpandHoriz(m_buttonOuvrirSceneMap);
    AG_ExpandHoriz(m_buttonRetour);

	AG_WindowShow(m_window);
    hide();
}


OpenSceneView::~OpenSceneView(void)
{
}
