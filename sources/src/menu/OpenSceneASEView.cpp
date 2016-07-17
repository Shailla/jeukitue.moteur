#include <vector>

using namespace std;

#include <agar/core.h>
#include <agar/gui.h>

#include "service/MapService.h"
#include <service/dto/AseInformationDto.h>
#include "menu/View.h"
#include "menu/Controller.h"
#include "menu/Viewer.h"

#include "menu/OpenSceneASEView.h"

OpenSceneASEView::OpenSceneASEView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
	AG_WindowSetCaption(m_window, "Importer une scene ASE");

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


OpenSceneASEView::~OpenSceneASEView(void)
{
}

void OpenSceneASEView::show(void)
{
	// Supprime les map trouvées précédemment
	vector<AG_Button*>::iterator iter = _aseButtons.begin();
	
	while(iter<_aseButtons.end()) {
		AG_ObjectDelete(*iter);
		iter++;
	}

	_aseButtons.clear();
	_aseNames.clear();

    // Création d'un bouton pour chaque Map disponible
	int mapNumber = 0;

	vector<AseInformationDto> content;
	MapService::loadAseDirectoryContent(content);

	for(AseInformationDto dto : content) {
		string aseName = dto.getAseFileMinimalName();

		AG_Button* button = AG_ButtonNewFn(_scrollview,
										   0,
										   aseName.c_str(),
										   m_controllerCallback,
										   "%i,%i",
										   Controller::ImportAseAction,
										   mapNumber++);
		_aseButtons.push_back(button);
		_aseNames.push_back(aseName);
	}

	// Rafraichissement de la page
	AG_WindowUpdate(m_window);

	View::show();
}

string OpenSceneASEView::getAseName(const int aseNumber)
{
	return _aseNames.at(aseNumber);
}
