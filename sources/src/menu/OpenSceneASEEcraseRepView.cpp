#include <vector>

using namespace std;

#include <agar/core.h>
#include <agar/gui.h>

#include "util/FindFolder.h"
#include "menu/View.h"
#include "menu/Controller.h"
#include "menu/Viewer.h"

#include "menu/OpenSceneASEEcraseRepView.h"

OpenSceneASEEcraseRepView::OpenSceneASEEcraseRepView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
	AG_WindowSetCaption(m_window, "Importer une scene ASE");

	AG_LabelNew(m_window, 0, "Un repertoire existe deja, voulez-vous le supprimer ?");

	AG_Box* box = AG_BoxNewHoriz(m_window, AG_BOX_HOMOGENOUS);
	AG_ExpandHoriz(box);
	AG_ButtonNewFn(box, 0, "Oui", m_controllerCallback, "%i", Controller::OpenSceneASEEcraseRepOuiAction);
	AG_ButtonNewFn(box, 0, "Non", m_controllerCallback, "%i", Controller::OpenSceneASEEcraseRepNonAction);

	AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 50, 50);
	AG_WindowShow(m_window);
    hide();
}
OpenSceneASEEcraseRepView::~OpenSceneASEEcraseRepView(void)
{
}

void OpenSceneASEEcraseRepView::setAseName(const string& aseName)
{
	_aseName = aseName;
}

string OpenSceneASEEcraseRepView::getAseName() const
{
	return _aseName;
}

void OpenSceneASEEcraseRepView::setRepName(const string& repName)
{
	_repName = repName;
}

string OpenSceneASEEcraseRepView::getRepName() const
{
	return _repName;
}
