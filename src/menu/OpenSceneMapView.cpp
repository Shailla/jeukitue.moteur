#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/style.h>

#include "menu/View.h"
#include "menu/Controller.h"
#include "menu/Viewer.h"

#include "menu/OpenSceneMapView.h"

OpenSceneMapView::OpenSceneMapView(const AG_EventFn controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_NOMOVE);
    AG_WindowSetCaption(m_window, "Ouvrir une scene Map");
	AG_LabelNewStatic(m_window, 0, "Not-implemented");

	AG_Scrollview* scrollview = AG_ScrollviewNew(m_window, AG_SCROLLVIEW_NOPAN_X | AG_SCROLLVIEW_FRAME);
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
    AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_LabelNewStatic(scrollview, 0, "Map");
	AG_ScrollviewSizeHint(scrollview, 150, 50);

	m_buttonRetour = AG_ButtonNewFn(m_window, 0, "Retour", controllerCallback, "%d", Controller::OpenSceneAction);
	AG_ExpandHoriz(m_buttonRetour);

	AG_WindowShow(m_window);

    hide();
}


OpenSceneMapView::~OpenSceneMapView(void)
{
}
