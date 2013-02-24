
#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/style.h>

#include "menu/View.h"
#include "menu/Controller.h"
#include "service/InformationService.h"
#include "service/dto/VersionsDto.h"

#include "menu/AboutView.h"

AboutView::AboutView(const AG_EventFn controllerCallback, Viewer* agarView)
:View(controllerCallback) {
	// Collecte des informations
	JktService::VersionsDto versions = JktService::InformationService::loadVersions();

	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS | AG_WINDOW_NOMOVE);
    AG_WindowSetCaption(m_window, "A propos");

    AG_LabelNew(m_window, 0, string("Date de compilation : ").append(__DATE__).c_str());
    AG_LabelNew(m_window, 0, string("Version OpenGL : ").append(versions.getOpenGlVersion()).c_str());
    AG_LabelNew(m_window, 0, string("Version GLU : ").append(versions.getGluVersion()).c_str());
    AG_LabelNew(m_window, 0, string("Version FMOD : ").append(versions.getFmodVersion()).c_str());
    AG_LabelNew(m_window, 0, string("Version Agar : ").append(versions.getAgarVersion()).c_str());

    AG_SeparatorNewHoriz(m_window);
    AG_LabelNew(m_window, 0, "JKT a ete developpe par Jean-Claude Vogel, ahuut@yahoo.fr");

	// Bouton retour
    AG_SeparatorNewHoriz(m_window);

	_buttonRetour = AG_ButtonNewFn(m_window, 0, "Retour", controllerCallback, "%i", Controller::ShowMainMenuAction);
    AG_ExpandHoriz(_buttonRetour);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 50, 50);
    hide();
}

AboutView::~AboutView(void)
{
}
