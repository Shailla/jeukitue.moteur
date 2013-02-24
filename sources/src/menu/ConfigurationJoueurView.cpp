
#include <sstream>

using namespace std;

#include <agar/core.h>
#include <agar/gui.h>

#include "SDL.h"
#include "menu/View.h"
#include "menu/Controller.h"
#include "main/Cfg.h"
#include "ressource/RessourcesLoader.h"

#include "menu/ConfigurationJoueurView.h"

extern CCfg Config;

ConfigurationJoueurView::ConfigurationJoueurView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	/* *******************************
	 * Initialisation de la fenêtre
	 * ******************************/

	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
    AG_WindowSetCaption(m_window, "Joueur");


    /* *********************************
     * Personnalisation de la fenêtre
     * ********************************/

	// Nom du joueur
	memset(_joueurName, '\0', sizeof(_joueurName));
	Config.Joueur.nom.copy(_joueurName, Config.Joueur.nom.length(), 0);

	AG_Textbox* joueurNameTexbox = AG_TextboxNew(m_window, 0, _joueurName);
	AG_TextboxSetLabel(joueurNameTexbox, "Nom du joueur :");
	AG_TextboxBindUTF8(joueurNameTexbox, _joueurName, sizeof(_joueurName));

	// Map du skin du joueur
	memset(_joueurMapName, '\0', sizeof(_joueurMapName));
	Config.Joueur.mapName.copy(_joueurMapName, Config.Joueur.mapName.length(), 0);

	_playerMapCombo = AG_ComboNew(m_window, 0, "Map :");
	AG_TextboxBindUTF8(_playerMapCombo->tbox, _joueurMapName, sizeof(_joueurMapName));
	AG_WidgetDisable(_playerMapCombo->tbox);
	AG_ExpandHoriz(_playerMapCombo);
	AG_ComboSizeHint(_playerMapCombo, "12345678901234567890", 10);


	/* *********************************
	 * Finalisation de la fenêtre
	 * ********************************/

	// Bouton retour
	AG_Box* box = AG_BoxNewHoriz(m_window, 0);
	AG_ExpandHoriz(box);
	AG_ButtonNewFn(box, 0, "Retour", controllerCallback, "%i", Controller::ShowConfigurationMenuAction);
    
	// Bouton appliquer
	AG_ButtonNewFn(box, 0, "Appliquer", controllerCallback, "%i", Controller::SaveConfigJoueurAction);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 50, 50);
	AG_WindowShow(m_window);
    hide();
}

ConfigurationJoueurView::~ConfigurationJoueurView(void) {
}

const char* ConfigurationJoueurView::getJoueurName(void) {
	return _joueurName;
}

const char* ConfigurationJoueurView::getJoueurMapName(void) {
	return _joueurMapName;
}

void ConfigurationJoueurView::show(void) {
	// MAP disponibles
	AG_TlistClear(_playerMapCombo->list);

	std::vector<string> mapNames = JktUtils::RessourcesLoader::getPlayerMaps();

	std::vector<string>::iterator iter = mapNames.begin();

	for( ; iter < mapNames.end() ; iter++) {
		string mapName = (*iter);

		AG_TlistAdd(_playerMapCombo->list, NULL, mapName.c_str());
	}

	// Mode plein écran actif
	memset(_joueurName, '\0', sizeof(_joueurName));
	Config.Joueur.nom.copy(_joueurName, Config.Joueur.nom.length(), 0);

	View::show();
}
