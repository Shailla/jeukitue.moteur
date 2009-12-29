#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/style.h>

#include "menu/View.h"
#include "menu/Controller.h"
#include "menu/Viewer.h"

#include "menu/OpenCentralisateurView.h"

OpenCentralisateurView::OpenCentralisateurView(const AG_EventFn controllerCallback)
{
    m_window = AG_WindowNew(0);
    AG_WindowSetCaption(m_window, "Centralisateur");

    // Champ adress IP du serveur centralisateur
    m_userNameStr[0] = '\0';
    m_userName = AG_TextboxNew(m_window, AG_TEXTBOX_HFILL, "Nom d'utilisateur");
    AG_TextboxBindUTF8(m_userName, m_userNameStr, sizeof(m_userNameStr));

    // Champ adress IP du serveur centralisateur
    m_adresseStr[0] = '\0';
    m_adresse = AG_TextboxNew(m_window, AG_TEXTBOX_HFILL, "Adresse");
    AG_TextboxBindUTF8(m_adresse, m_adresseStr, sizeof(m_adresseStr));

    // Champ port du serveur centralisateur
    m_port = AG_TextboxNew(m_window, AG_TEXTBOX_HFILL|AG_TEXTBOX_INT_ONLY, "Port");

    AG_SeparatorNewHoriz(m_window);

    // Bouton de connexion au serveur centralisateur
    m_connection = AG_ButtonNewFn(m_window, 0, "Connecter", controllerCallback, "%d", Controller::ConnectCentralisateurAction);

	// Bouton retour
	m_retour = AG_ButtonNewFn(m_window, 0, "Retour", controllerCallback, "%d", Controller::MainMenuAction);

    AG_WindowSetGeometryAligned(m_window, AG_WINDOW_MC, 220, 150);

	AG_WindowShow(m_window);
    hide();
}

OpenCentralisateurView::~OpenCentralisateurView(void)
{
    delete[] &m_adresseStr;
}

int OpenCentralisateurView::getPort(void)
{
    return AG_TextboxInt(m_port);
}

char* OpenCentralisateurView::getAdresse(void)
{
    return m_adresseStr;
}

char* OpenCentralisateurView::getUserName(void)
{
    return m_userNameStr;
}
