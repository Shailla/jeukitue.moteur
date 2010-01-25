#include <sstream>

#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/style.h>

#include "menu/View.h"
#include "menu/Controller.h"
#include "menu/Viewer.h"
#include "main/Cfg.h"

#include "menu/ConfigCentralisateurView.h"

extern CCfg Config;

ConfigCentralisateurView::ConfigCentralisateurView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
    m_window = AG_WindowNew(0);
    AG_WindowSetCaption(m_window, "Centralisateur");

    // Champ adress IP du serveur centralisateur
	const std::string ip = Config.Centralisateur.getIp().c_str();
	memset(m_adresseStr, '\0', sizeof(m_adresseStr));
	ip.copy(m_adresseStr, ip.length(), 0);
    m_adresse = AG_TextboxNew(m_window, AG_TEXTBOX_HFILL, "Adresse");
    AG_TextboxBindUTF8(m_adresse, m_adresseStr, sizeof(m_adresseStr));

    // Champ port du serveur centralisateur
    m_port = AG_TextboxNew(m_window, AG_TEXTBOX_HFILL|AG_TEXTBOX_INT_ONLY, "Port");
	AG_TextboxSetIntOnly(m_port, true);
	std::ostringstream port;
	port << Config.Centralisateur.getPort();
	AG_TextboxSetString(m_port, port.str().c_str());

    AG_SeparatorNewHoriz(m_window);

    // Bouton de connexion au serveur centralisateur
    m_connection = AG_ButtonNewFn(m_window, 0, "Enregistrer", controllerCallback, "%i", Controller::SaveConfigCentralisateurAction);

	// Bouton retour
	AG_SeparatorNewHoriz(m_window);
	m_retour = AG_ButtonNewFn(m_window, 0, "Retour", controllerCallback, "%i", Controller::MainMenuAction);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 50, 50);
	AG_WindowShow(m_window);
    hide();
}

ConfigCentralisateurView::~ConfigCentralisateurView(void)
{
    delete m_adresseStr;
}

int ConfigCentralisateurView::getPort(void)
{
    return AG_TextboxInt(m_port);
}

char* ConfigCentralisateurView::getAdresse(void)
{
    return m_adresseStr;
}
