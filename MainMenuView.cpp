
#include <agar/core.h>
#include <agar/gui.h>

#include "AgarController.h"

#include "MainMenuView.h"

MainMenuView::MainMenuView(const AG_EventFn controllerCallback)
{
    m_window = AG_WindowNew(0);
    AG_WindowSetCaption(m_window, "Menu principal");
	//AG_LabelNewStatic(m_window, 0, "Hello, world!");

    m_buttonOuvrirScene = AG_ButtonNewFn(m_window, 0, "Ouvrir une scene", controllerCallback, "%d", AgarController::OpenSceneAction);
    m_buttonCentralisateur = AG_ButtonNewFn(m_window, 0, "Centralisateur", controllerCallback, "%d", AgarController::OpenCentralisateurAction);
    m_buttonQuitter = AG_ButtonNewFn(m_window, 0, "Quitter", controllerCallback, "%d", AgarController::QuitAction);

    AG_ExpandHoriz(m_buttonOuvrirScene);
    AG_ExpandHoriz(m_buttonCentralisateur);
    AG_ExpandHoriz(m_buttonQuitter);

	AG_WindowShow(m_window);

    show();
}

MainMenuView::~MainMenuView(void)
{
}

void MainMenuView::show(void)
{
    AG_WindowShow(m_window);
    AG_WidgetFocus(m_window);
}

void MainMenuView::hide(void)
{
    AG_WindowHide(m_window);
    AG_WidgetUnfocus(m_window);
}
