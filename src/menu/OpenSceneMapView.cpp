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
	m_window = AG_WindowNew(AG_WINDOW_NOMOVE);
    AG_WindowSetCaption(m_window, "Ouvrir une scene Map");

    // Scrollview qui contiendra toutes les map disponibles
	m_scrollview = AG_ScrollviewNew(m_window, AG_SCROLLVIEW_NOPAN_X|AG_SCROLLVIEW_FRAME);
	AG_Expand(m_scrollview);

    // Bouton retour
    AG_SeparatorNewHoriz(m_window);
	m_buttonRetour = AG_ButtonNewFn(m_window, 0, "Retour", controllerCallback, "%d", Controller::OpenSceneAction);
	AG_ExpandHoriz(m_buttonRetour);

	AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 50, 50);
	AG_WindowShow(m_window);
    hide();
}

void OpenSceneMapView::show(void)
{
    // Purge de la scrollview... mais comment faire ???
    AG_ObjectFreeChildren(m_scrollview);
    //AG_WidgetUpdate(m_scrollview);

    // Création d'un bouton pour chaque Map disponible
    string name;
	CFindFolder folder( "./Map/", 0, ".map.xml" );
	folder.nbr();   // TODO : Cette ligne ne sert à rien, mais lorsqu'elle n'est pas présente il y a un bug
	folder.reset();

	while( folder.findNext( name ) )
	{
		name.erase( name.find_last_of( "." ) );
		name.erase( name.find_last_of( "." ) );

        AG_ButtonNewFn(m_scrollview, 0, name.c_str(), m_controllerCallback, "%d%s", Controller::OpenOneSceneMapAction, name.c_str());
	}

	View::show();
}

OpenSceneMapView::~OpenSceneMapView(void)
{
}
