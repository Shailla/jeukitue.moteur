#include <vector>

using namespace std;

#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/style.h>

#include "util/FindFolder.h"
#include "menu/View.h"
#include "menu/Controller.h"
#include "menu/Viewer.h"

#include "menu/OpenSceneASEEcraseRepView.h"

OpenSceneASEEcraseRepView::OpenSceneASEEcraseRepView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_NOMOVE);
	AG_WindowSetCaption(m_window, "Importer une scene ASE");

	AG_LabelNew(m_window, 0, "Un répertoire existe déjà, voulez-vous le supprimer ?");

	AG_ButtonNewFn(m_window, 0, "Oui", m_controllerCallback, "%i", Controller::OpenSceneASEEcraseRepOuiAction);
	AG_ButtonNewFn(m_window, 0, "Non", m_controllerCallback, "%i", Controller::OpenSceneASEEcraseRepNonAction);

    // Bouton annuler
    AG_SeparatorNewHoriz(m_window);
	AG_Button* buttonAnnuler = AG_ButtonNewFn(m_window, 0, "Annuler", m_controllerCallback, "%i", Controller::OpenSceneASEAction);
	AG_ExpandHoriz(buttonAnnuler);

	AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 50, 50);
	AG_WindowShow(m_window);
    hide();
}


OpenSceneASEEcraseRepView::~OpenSceneASEEcraseRepView(void)
{
}
