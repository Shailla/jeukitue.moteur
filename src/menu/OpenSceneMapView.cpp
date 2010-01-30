#include <fstream>
#include <iostream>
#ifdef WIN32
	#include <io.h>
#endif
using namespace std;

#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/style.h>

#include "util/FindFolder.h"
#include "menu/View.h"
#include "menu/Controller.h"
#include "menu/Viewer.h"

#include "menu/OpenSceneMapView.h"

/*
    OpenSceneMapView affiche dans une scrollview la liste des map disponibles dans
    le répertoire Map du jeu.
    On peut en choisir une, visualiser son apperçu et lancer une partie avec cette map.
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
	AG_Button* buttonRetour = AG_ButtonNewFn(m_window, 0, "Retour", m_controllerCallback, "%i", Controller::OpenSceneAction);
	AG_ExpandHoriz(buttonRetour);

	AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 50, 50);
	AG_WindowShow(m_window);

    hide();
}

OpenSceneMapView::~OpenSceneMapView(void)
{
	// TODO détruire les éléments de mapNames si ce n'est pas fait par la list
}

void OpenSceneMapView::show(void)
{
	// Supprime les map trouvées précédemment
	vector<AG_Button*>::iterator iter = _mapButtons.begin();
	
	while(iter<_mapButtons.end()) {
		AG_ObjectDelete(*iter);
		iter++;
	}

	_mapButtons.clear();
	_mapNames.clear();

    // Création d'un bouton pour chaque Map disponible
    string mapName;
	CFindFolder folder( "./Map/", 0, ".map.xml" );
	folder.nbr();   // TODO : Cette ligne ne sert à rien, mais lorsqu'elle n'est pas présente il y a un bug
	folder.reset();

	int mapNumber = 0;
	while( folder.findNext( mapName ) )
	{
		mapName.erase( mapName.find_last_of( "." ) );
		mapName.erase( mapName.find_last_of( "." ) );

		AG_Button* button = AG_ButtonNewFn(_scrollview,
										   0,
										   mapName.c_str(),
										   m_controllerCallback,
										   "%i,%i",
										   Controller::OpenMapAction,
										   mapNumber++);		
		_mapButtons.push_back(button);
		_mapNames.push_back(mapName);
	}

	// Rafraichissement de la page
	AG_WindowUpdate(m_window);

	View::show();
}

string OpenSceneMapView::getMapName(const int mapNumber)
{
	return _mapNames.at(mapNumber);
}
