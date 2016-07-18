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
#include "service/dto/MapInformationDto.h"
#include "service/MapService.h"

#include "menu/OpenSceneMapView.h"

/*
    OpenSceneMapView affiche dans une scrollview la liste des map disponibles dans
    le r�pertoire Map du jeu.
    On peut en choisir une et lancer une partie avec cette map.
*/
OpenSceneMapView::OpenSceneMapView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
	AG_WindowSetCaption(m_window, "Ouvrir une scene Map");

    // Scrollview qui contiendra toutes les map disponibles
	_scrollview = AG_ScrollviewNew(m_window, AG_SCROLLVIEW_NOPAN_X);
	AG_Expand(_scrollview);

    // Bouton retour
    AG_SeparatorNewHoriz(m_window);
	AG_Button* buttonRetour = AG_ButtonNewFn(m_window, 0, "Retour", m_controllerCallback, "%i", Controller::ShowMenuOpenSceneAction);
	AG_ExpandHoriz(buttonRetour);

	AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 25, 50);
	AG_WindowShow(m_window);

    hide();
}

OpenSceneMapView::~OpenSceneMapView(void) {
}

void OpenSceneMapView::show(void) {
	// Supprime les map trouv�es pr�c�demment
	vector<AG_Button*>::iterator iterDel = _mapButtons.begin();
	
	while(iterDel < _mapButtons.end()) {
		AG_ObjectDelete(*iterDel);
		iterDel++;
	}

	_mapButtons.clear();
	_maps.clear();

    // Cr�ation d'un bouton pour chaque Map disponible
	MapService::loadMapDirectoryContent(_maps);

	int mapNumber = 0;

	for(MapInformationDto& map : _maps) {
		AG_Button* button = AG_ButtonNewFn(_scrollview, 0, map.getMapFileMinimalName().c_str(), m_controllerCallback, "%i,%i", Controller::OpenMapAction, mapNumber++);
		_mapButtons.push_back(button);
	}

	// Rafraichissement de la page
	AG_WindowUpdate(m_window);
	View::show();
}

string OpenSceneMapView::getMapName(const int mapNumber) {
	MapInformationDto map = _maps.at(mapNumber);
	return map.getMapFileMinimalName();
}
