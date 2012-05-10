#include <fstream>
#include <iostream>
#ifdef WIN32
	#include <io.h>
#endif

using namespace std;

#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/style.h>

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

    // Scrollview qui contiendra toutes les map disponibles
	_scrollview = AG_ScrollviewNew(m_window, AG_SCROLLVIEW_NOPAN_X);
	AG_Expand(_scrollview);

    // Bouton retour
    AG_SeparatorNewHoriz(m_window);
	AG_Button* buttonRetour = AG_ButtonNewFn(m_window, 0, "Retour", m_controllerCallback, "%i", Controller::MainMenuAction);
	AG_ExpandHoriz(buttonRetour);

	AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 50, 50);
	AG_WindowShow(m_window);

    hide();
}

PluginsManagementView::~PluginsManagementView(void) {
}

void PluginsManagementView::show(void)
{
	// Supprime les labels des plugins
	vector<AG_Label*>::iterator iterLabel = _pluginLabels.begin();

	while(iterLabel < _pluginLabels.end()) {
		AG_ObjectDelete(*iterLabel);
		iterLabel++;
	}

	_pluginLabels.clear();

	// Supprime les boutons d'activation des plugins
	vector<AG_Button*>::iterator iterActivateButton = _pluginActivateButtons.begin();

	while(iterActivateButton < _pluginActivateButtons.end()) {
		AG_ObjectDelete(*iterActivateButton);
		iterActivateButton++;
	}

	_pluginActivateButtons.clear();

	// Supprime les boutons de désactivation des plugins
	vector<AG_Button*>::iterator iterDeactivateButton = _pluginDeactivateButtons.begin();
	
	while(iterDeactivateButton < _pluginDeactivateButtons.end()) {
		AG_ObjectDelete(*iterDeactivateButton);
		iterDeactivateButton++;
	}

	_pluginDeactivateButtons.clear();

	// Supprime les boutons de désactivation des plugins
	vector<AG_Button*>::iterator iterExecuteButton = _pluginExecuteButtons.begin();

	while(iterExecuteButton < _pluginExecuteButtons.end()) {
		AG_ObjectDelete(*iterExecuteButton);
		iterExecuteButton++;
	}

	_pluginDeactivateButtons.clear();

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
		AG_Box* box = AG_BoxNewHoriz(_scrollview, AG_BOX_HOMOGENOUS | AG_BOX_HFILL);

		AG_Label* label = AG_LabelNew(box, 0, pluginName.c_str());
		AG_Button* buttonActivate = AG_ButtonNewFn(box,
									   	   0,
									   	   "Activer",
									   	   m_controllerCallback,
									   	   "%i,%i",
									   	   Controller::PluginActivateAction,
									   	   pluginNumber);
		AG_Button* buttonDeactivate = AG_ButtonNewFn(box,
									   	   0,
									   	   "Desactiver",
									   	   m_controllerCallback,
									   	   "%i,%i",
									   	   Controller::PluginDeactivateAction,
									   	   pluginNumber);
		AG_Button* buttonExecute = AG_ButtonNewFn(box,
									   	   0,
									   	   "Executer",
									   	   m_controllerCallback,
									   	   "%i,%i",
									   	   Controller::PluginExecuteAction,
									   	   pluginNumber);

		_pluginBoxs.push_back(box);
		_pluginLabels.push_back(label);
		_pluginActivateButtons.push_back(buttonActivate);
		_pluginDeactivateButtons.push_back(buttonDeactivate);
		_pluginExecuteButtons.push_back(buttonExecute);

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
