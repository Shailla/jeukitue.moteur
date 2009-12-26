#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/style.h>

#include "View.h"
#include "Controller.h"
#include "Viewer.h"

#include "OpenSceneMapView.h"

OpenSceneMapView::OpenSceneMapView(const AG_EventFn controllerCallback)
{
	m_window = AG_WindowNew(0);
    AG_WindowSetCaption(m_window, "Ouvrir une scene Map");
	AG_LabelNewStatic(m_window, 0, "Not-implemented");
	
	m_buttonRetour = AG_ButtonNewFn(m_window, 0, "Retour", controllerCallback, "%d", Controller::OpenSceneAction);
	AG_ExpandHoriz(m_buttonRetour);

	AG_WindowShow(m_window);

    hide();
}


OpenSceneMapView::~OpenSceneMapView(void)
{
}
