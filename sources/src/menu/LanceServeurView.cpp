#include <fstream>
#include <iostream>
#ifdef WIN32
	#include <io.h>
#endif

#include <agar/core.h>
#include <agar/gui.h>

#include "service/dto/MapInformationDto.h"
#include "service/MapService.h"
#include "util/FindFolder.h"
#include "menu/View.h"
#include "menu/Controller.h"
#include "menu/Viewer.h"

#include "menu/LanceServeurView.h"

using namespace std;

/*
    LanceServeurView affiche dans une scrollview la liste des map disponibles dans
    le r�pertoire Map du jeu.
    On peut en choisir une, visualiser son apper�u et lancer un serveir de jeu avec cette map.
*/
LanceServeurView::LanceServeurView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
	AG_WindowSetCaption(m_window, "Ouvrir une sc\u00e8ne Map");

    // Scrollview qui contiendra toutes les map disponibles
	_scrollview = AG_ScrollviewNew(m_window, AG_SCROLLVIEW_NOPAN_X);
	AG_Expand(_scrollview);

    // Bouton retour
    AG_SeparatorNewHoriz(m_window);
	AG_Button* buttonRetour = AG_ButtonNewFn(m_window, 0, "Retour", m_controllerCallback, "%i", Controller::ShowMultijoueursMenuAction);
	AG_ExpandHoriz(buttonRetour);

	AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 50, 50);
	AG_WindowShow(m_window);

    hide();
}

LanceServeurView::~LanceServeurView(void) {
}

void LanceServeurView::show(void) {
	// Supprime les map trouv�es pr�c�demment
	vector<AG_Button*>::iterator iter = _mapButtons.begin();
	
	while(iter<_mapButtons.end()) {
		AG_ObjectDelete(*iter);
		iter++;
	}

	_mapButtons.clear();
	_mapNames.clear();

    // Cr�ation d'un bouton pour chaque Map disponible
    string mapName;
	int mapNumber = 0;
	
	vector<MapInformationDto> content;
	MapService::loadMapDirectoryContent(content);

	for(MapInformationDto& dto : content) {
		mapName = dto.getMapFileMinimalName();
		AG_Button* button = AG_ButtonNewFn(_scrollview,
										   0,
										   mapName.c_str(),
										   m_controllerCallback,
										   "%i,%i",
										   Controller::LanceServeurMapAction,
										   mapNumber++);		
		_mapButtons.push_back(button);
		_mapNames.push_back(mapName);
	}

	// Rafraichissement de la page
	AG_WindowUpdate(m_window);

	View::show();
}

string LanceServeurView::getMapName(const int mapNumber) {
	return _mapNames.at(mapNumber);
}
