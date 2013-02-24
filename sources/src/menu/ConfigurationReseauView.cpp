#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"
#include "menu/Controller.h"
#include "main/Cfg.h"

#include "menu/ConfigurationReseauView.h"

extern CCfg Config;

ConfigurationReseauView::ConfigurationReseauView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	/* *******************************
	 * Initialisation de la fenêtre
	 * ******************************/

	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
    AG_WindowSetCaption(m_window, "Reseau");


    /* *********************************
     * Personnalisation de la fenêtre
     * ********************************/

    // Mode serveur ou client
    AG_CheckboxNewInt(m_window, 0, "Mode serveur :", &_isServeur);

	// IP du serveur
	AG_Textbox* ipServeurTexbox = AG_TextboxNew(m_window, 0, _ipServeur);
	AG_TextboxSetLabel(ipServeurTexbox, "Adresse IP :");
	AG_TextboxBindUTF8(ipServeurTexbox, _ipServeur, sizeof(_ipServeur));
	AG_TextboxSizeHint(ipServeurTexbox, "1234567890123456789012345678901234567890");

    // Champ port du serveur
	AG_NumericalNewIntR(m_window, 0, NULL, "Port :", &_portServeur, 0, 65000);


	/* *********************************
	 * Finalisation de la fenêtre
	 * ********************************/

	// Bouton retour
	AG_Box* box = AG_BoxNewHoriz(m_window, 0);
	AG_ExpandHoriz(box);
	AG_ButtonNewFn(box, 0, "Retour", controllerCallback, "%i", Controller::ShowConfigurationMenuAction);

	// Bouton appliquer
	AG_ButtonNewFn(box, 0, "Appliquer", controllerCallback, "%i", Controller::SaveConfigReseauAction);

	AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 50, 50);
	AG_WindowShow(m_window);
    hide();
}

ConfigurationReseauView::~ConfigurationReseauView(void) {
}

const char* ConfigurationReseauView::getIpServeur(void) const {
	return _ipServeur;
}

int ConfigurationReseauView::getPortServeur(void) const {
	return _portServeur;
}

bool ConfigurationReseauView::isServeur(void) const {
	if(_isServeur) {
		return true;
	}
	else {
		return false;
	}
}

void ConfigurationReseauView::show(void) {
	/* ********************************
	 * Initialisation des valeurs
	 * *******************************/

	// Activation du mode serveur
	_isServeur = Config.Reseau.isServeur();

	// IP du serveur
	memset(_ipServeur, '\0', sizeof(_ipServeur));
	Config.Reseau.getIpServer().copy(_ipServeur, Config.Reseau.getIpServer().length(), 0);

	// Port du serveur
	_portServeur = Config.Reseau.getPort();


	/* *********************************
	 * Affichage graphique
	 * ********************************/

	View::show();
}
