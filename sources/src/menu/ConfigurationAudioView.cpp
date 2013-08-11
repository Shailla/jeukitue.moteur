
#include <sstream>

using namespace std;

#include <agar/core.h>
#include <agar/gui.h>

#include "SDL.h"
#include "menu/View.h"
#include "menu/Controller.h"
#include "main/Cfg.h"

#include "menu/ConfigurationAudioView.h"

ConfigurationAudioView::ConfigurationAudioView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
    AG_WindowSetCaption(m_window, "Vidéo");

    AG_SeparatorNewHoriz(m_window);
	
	// Bouton retour
	AG_Box* box = AG_BoxNewHoriz(m_window, 0);
	AG_ExpandHoriz(box);
	AG_ButtonNewFn(box, 0, "Retour", controllerCallback, "%i", Controller::ShowConfigurationMenuAction);
    
	// Bouton appliquer
	AG_ButtonNewFn(box, 0, "Appliquer", controllerCallback, "%i", Controller::ShowConfigurationVideoViewAction);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 50, 50);
	AG_WindowShow(m_window);
    hide();
}

ConfigurationAudioView::~ConfigurationAudioView(void) {
}

void ConfigurationAudioView::show(void) {
	View::show();
}
