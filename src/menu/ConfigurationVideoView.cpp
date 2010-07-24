
#include <sstream>

using namespace std;

#include <agar/core.h>
#include <agar/gui.h>

#include "SDL.h"
#include "menu/View.h"
#include "menu/Controller.h"
#include "main/Cfg.h"

#include "menu/ConfigurationVideoView.h"

extern CCfg Config;

ConfigurationVideoView::ConfigurationVideoView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
    AG_WindowSetCaption(m_window, "Vidéo");

	// Résolution
	_resoCombo = AG_ComboNew(m_window, 0, "Resolution");
	AG_WidgetDisable(_resoCombo->tbox);
	AG_ExpandHoriz(_resoCombo);
	AG_ComboSizeHint(_resoCombo, "XXXX*XXXX", 10);

	// Mode plein écran
	AG_CheckboxNewInt(m_window, 0, "Plein ecran", &_fullScreen);
	
    AG_SeparatorNewHoriz(m_window);
	
	// Bouton retour
	AG_Box* box = AG_BoxNewHoriz(m_window, 0);
	AG_ExpandHoriz(box);
	AG_ButtonNewFn(box, 0, "Retour", controllerCallback, "%i", Controller::ConfigurationAction);
    
	// Bouton appliquer
	AG_ButtonNewFn(box, 0, "Appliquer", controllerCallback, "%i", Controller::ConfigurationVideoAction);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 50, 50);
	AG_WindowShow(m_window);
    hide();
}

ConfigurationVideoView::~ConfigurationVideoView(void) {
}

void ConfigurationVideoView::show(void)
{
	// Résolution d'écran active
//	ostringstream resoActive;
//	resoActive << Config.Display.X << " * " << Config.Display.Y;
//	AG_LabelText(_resoActive, resoActive.str().c_str());

	// Résolutions d'écran disponibles
	AG_TlistClear(_resoCombo->list);

	SDL_Rect **modes = SDL_ListModes(NULL, SDL_FULLSCREEN|SDL_HWSURFACE);

	for(int i=0 ; modes[i] ; i++)
	{
		ostringstream reso;
		reso << modes[i]->w << "*" << modes[i]->h;
		AG_TlistAdd(_resoCombo->list, NULL, reso.str().c_str());
	}

	// Mode plein écran actif
	if(Config.Display.Fullscreen())
	{
		_fullScreen = 1;
	}
	else
	{
		_fullScreen = 0;
	}

	View::show();
}
