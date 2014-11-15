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
	memset(_ipServeur, '\0', sizeof(_ipServeur));

	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
    AG_WindowSetCaption(m_window, "Reseau");


    /* *********************************
     * Personnalisation de la fenêtre
     * ********************************/

    // Mode serveur ou client
    AG_CheckboxNewInt(m_window, 0, "Mode serveur :", &_isModeServeur);

	// IP du serveur
	_ipServeurTexbox = AG_TextboxNew(m_window, 0, _ipServeur);
	AG_TextboxSetLabel(_ipServeurTexbox, "Adresse IP du serveur :");
	AG_TextboxBindUTF8(_ipServeurTexbox, _ipServeur, sizeof(_ipServeur));
	AG_TextboxSizeHint(_ipServeurTexbox, "1234567890123456789012345678901234567890");

    // Champs ports du serveur ou du client selon le mode
	AG_NumericalNewIntR(m_window, 0, NULL, "Port serveur primitif :", &_port, 0, 65000);
	AG_NumericalNewIntR(m_window, 0, NULL, "Port serveur moderne :", &_portTree, 0, 65000);


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

int ConfigurationReseauView::getPort(void) const {
	return _port;
}

int ConfigurationReseauView::getPortTree(void) const {
	return _portTree;
}

bool ConfigurationReseauView::isModeServeur(void) const {
	if(_isModeServeur) {
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
	_isModeServeur = Config.Reseau.isServeur();

	// IP du serveur
	memset(_ipServeur, '\0', sizeof(_ipServeur));
	Config.Reseau.getIpServer().copy(_ipServeur, Config.Reseau.getIpServer().length(), 0);

	// Ports du serveur ou du client selon le mode
	_port = Config.Reseau.getServerPort();
	_portTree = Config.Reseau.getServerPortTree();


	/* *********************************
	 * Affichage graphique
	 * ********************************/

	View::show();
}
