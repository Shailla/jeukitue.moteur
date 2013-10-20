#include <fstream>
#include <iostream>
#ifdef WIN32
	#include <io.h>
#endif

using namespace std;

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"
#include "menu/Controller.h"
#include "menu/Viewer.h"
#include "util/StringUtils.h"
#include "ressource/RessourcesLoader.h"
#include "service/LuaService.h"

#include "menu/PluginsManagementView.h"

using namespace JktUtils;

/*
    OpenSceneMapView affiche dans une scrollview la liste des map disponibles dans
    le répertoire Map du jeu.
    On peut en choisir une et lancer une partie avec cette map.
*/
PluginsManagementView::PluginsManagementView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
	AG_WindowSetCaption(m_window, "Gestion des plugins");

	_scrollview = AG_ScrollviewNew(m_window, AG_SCROLLVIEW_EXPAND | AG_SCROLLVIEW_NOPAN_X);

    // Bouton retour
    AG_SeparatorNewHoriz(m_window);
	AG_Button* buttonRetour = AG_ButtonNewFn(m_window, 0, "Retour", m_controllerCallback, "%i", Controller::ShowMainMenuAction);
	AG_ExpandHoriz(buttonRetour);

	AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 50, 50);
	AG_WindowShow(m_window);

    hide();
}

PluginsManagementView::~PluginsManagementView(void) {
}

void PluginsManagementView::show(void) {
	// Supprime boxes des plugins
	vector<AG_Box*>::iterator iterBox = _pluginBoxs.begin();

	while(iterBox < _pluginBoxs.end()) {
		AG_ObjectDelete(*iterBox);
		iterBox++;
	}

	_pluginBoxs.clear();

	// Supprime la liste des noms plugins
	_pluginNames.clear();

    // Création d'un label et d'un bouton d'exécution pour chaque Map disponible
	vector<string> pluginNames;
	JktService::LuaService::loadLuaDirectoryContent(_pluginNames);

	vector<string>::iterator iterPluginName = _pluginNames.begin();



	int pluginNumber = 0;
	while(iterPluginName < _pluginNames.end()) {
		string pluginName = *iterPluginName;
		AG_Box* box = AG_BoxNewHoriz(_scrollview, AG_BOX_HOMOGENOUS | AG_BOX_EXPAND);

		// Nom du plugin
		AG_Box* boxPluginName = AG_BoxNewHoriz(box, AG_BOX_HOMOGENOUS | AG_BOX_EXPAND);
		AG_Label* label = AG_LabelNew(boxPluginName, 0, pluginName.c_str());
		AG_ExpandHoriz(label);

		// Boutons associés au plugin ("Activer", "Désactiver", "Exécuter")
		AG_Box* boxButtons = AG_BoxNewHoriz(box, AG_BOX_HOMOGENOUS | AG_BOX_EXPAND);

		AG_Button* buttonActivate = AG_ButtonNewFn(boxButtons,
									   	   0,
									   	   "Activer",
									   	   m_controllerCallback,
									   	   "%i,%i",
									   	   Controller::PluginActivateAction,
									   	   pluginNumber);
		AG_ExpandHoriz(buttonActivate);

		AG_Button* buttonDeactivate = AG_ButtonNewFn(boxButtons,
									   	   0,
									   	   "Desactiver",
									   	   m_controllerCallback,
									   	   "%i,%i",
									   	   Controller::PluginDeactivateAction,
									   	   pluginNumber);
		AG_ExpandHoriz(buttonDeactivate);

		_pluginBoxs.push_back(box);

		iterPluginName++;
		pluginNumber++;
	}

	// Rafraichissement de la page
	AG_WindowUpdate(m_window);

	View::show();
}

string PluginsManagementView::getPluginName(const int pluginNumber) {
	return _pluginNames.at(pluginNumber);
}
