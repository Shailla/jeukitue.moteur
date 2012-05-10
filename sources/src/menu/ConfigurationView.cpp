#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"
#include "menu/Controller.h"

#include "menu/ConfigurationView.h"

ConfigurationView::ConfigurationView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
    AG_WindowSetCaption(m_window, "Configuration");

	// Configuration Joueur
    AG_Button* buttonJoueur = AG_ButtonNewFn(m_window, 0, "Joueur", controllerCallback, "%i", Controller::ConfigurationJoueurAction);
	AG_ExpandHoriz(buttonJoueur);

	// Configuration Audio
    AG_Button* buttonAudio = AG_ButtonNewFn(m_window, 0, "Audio", controllerCallback, "%i", -1);
	AG_ExpandHoriz(buttonAudio);

	// Configuration Vidéo
    AG_Button* buttonVideo = AG_ButtonNewFn(m_window, 0, "Video", controllerCallback, "%i", Controller::ConfigurationVideoAction);
	AG_ExpandHoriz(buttonVideo);

	// Configuration Centralisateur
    AG_Button* buttonCentralisateur = AG_ButtonNewFn(m_window, 0, "Centralisateur", controllerCallback, "%i", Controller::ConfigCentralisateurAction);
	AG_ExpandHoriz(buttonCentralisateur);
	
	// Configuration Plugins
    AG_Button* buttonPluginsManagement = AG_ButtonNewFn(m_window, 0, "Plugins", controllerCallback, "%i", Controller::PluginsManagementAction);
	AG_ExpandHoriz(buttonPluginsManagement);

	// Configuration Agar
    AG_Button* buttonAgarConfiguration = AG_ButtonNewFn(m_window, 0, "Configuration Agar", controllerCallback, "%i", Controller::AgarConfigurationAction);
	AG_ExpandHoriz(buttonAgarConfiguration);

	// Bouton retour
    AG_SeparatorNewHoriz(m_window);

	AG_Button* buttonRetour = AG_ButtonNewFn(m_window, 0, "Retour", controllerCallback, "%i", Controller::MainMenuAction);
    AG_ExpandHoriz(buttonRetour);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 50, 50);
	AG_WindowShow(m_window);
    hide();
}

ConfigurationView::~ConfigurationView(void)
{
}
