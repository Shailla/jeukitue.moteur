#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"
#include "menu/Controller.h"
#include "menu/Viewer.h"

#include "menu/OpenSceneView.h"

OpenSceneView::OpenSceneView(const AG_EventFn controllerCallback) : View(controllerCallback) {
	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
    AG_WindowSetCaption(m_window, "Ouvrir une sc\u00e8ne");

	m_buttonConvertirSceneASE = AG_ButtonNewFn(m_window, 0, "Importer une scene ASE", controllerCallback, "%i", Controller::ShowOpenAseViewAction);
    m_buttonOuvrirSceneMap = AG_ButtonNewFn(m_window, 0, "Ouvrir une scene Map", controllerCallback, "%i", Controller::ShowOpenMapViewAction);

    AG_SeparatorNewHoriz(m_window);
    m_buttonRetour = AG_ButtonNewFn(m_window, 0, "Retour", controllerCallback, "%i", Controller::ShowMenuAction);

    AG_ExpandHoriz(m_buttonConvertirSceneASE);
    AG_ExpandHoriz(m_buttonOuvrirSceneMap);
    AG_ExpandHoriz(m_buttonRetour);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 25, 50);
    hide();
}

OpenSceneView::~OpenSceneView(void) {
	if(m_window) {
		AG_ObjectDelete(m_window);
	}
}
