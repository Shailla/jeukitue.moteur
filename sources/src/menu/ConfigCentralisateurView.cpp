#include <sstream>

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"
#include "menu/Controller.h"
#include "menu/Viewer.h"
#include "main/Cfg.h"

#include "menu/ConfigCentralisateurView.h"

extern CCfg Config;

ConfigCentralisateurView::ConfigCentralisateurView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	/* *******************************
	 * Initialisation de la fenêtre
	 * ******************************/

	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
    AG_WindowSetCaption(m_window, "Centralisateur");


    /* *********************************
     * Personnalisation de la fenêtre
     * ********************************/

    // Champ adress IP du serveur centralisateur
	AG_Textbox* ipTb = AG_TextboxNew(m_window, 0, _adresse);
	AG_TextboxSetLabel(ipTb, "Adresse IP :");
	AG_TextboxBindUTF8(ipTb, _adresse, sizeof(_adresse));
	AG_TextboxSizeHint(ipTb, "1234567890123456789012345678901234567890");

    // Champ port du serveur centralisateur
	AG_NumericalNewIntR(m_window, 0, NULL, "Port :", &_port, 0, 65000);


	/* *********************************
	 * Finalisation de la fenêtre
	 * ********************************/

	// Bouton retour
	AG_Box* box = AG_BoxNewHoriz(m_window, 0);
	AG_ExpandHoriz(box);
	AG_ButtonNewFn(box, 0, "Retour", controllerCallback, "%i", Controller::ShowConfigurationMenuAction);

	// Bouton appliquer
	AG_ButtonNewFn(box, 0, "Enregistrer", controllerCallback, "%i", Controller::SaveConfigCentralisateurAction);

	AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 50, 50);
	AG_WindowShow(m_window);
    hide();
}

ConfigCentralisateurView::~ConfigCentralisateurView(void) {
    delete[] &_adresse;
}

int ConfigCentralisateurView::getPort(void) {
    return _port;
}

char* ConfigCentralisateurView::getAdresse(void) {
    return _adresse;
}

void ConfigCentralisateurView::show(void) {
	/* ********************************
	 * Initialisation des valeurs
	 * *******************************/

	// IP du centralisateur
	memset(_adresse, '\0', sizeof(_adresse));
	Config.Centralisateur.getIp().copy(_adresse, Config.Centralisateur.getIp().length(), 0);

	// Port du centralisateur
	_port = Config.Centralisateur.getPort();


	/* *********************************
	 * Affichage graphique
	 * ********************************/

	View::show();
}
