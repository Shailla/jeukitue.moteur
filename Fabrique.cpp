#include "Fabrique.h"

Centralisateur* Fabrique::m_centralisateur = 0;
Viewer* Fabrique::m_agarView = 0;
Controller* Fabrique::m_agarController = 0;

void Fabrique::construct(void)
{
    UdpController* udpController = new UdpController();
    m_centralisateur = new Centralisateur(udpController);

    // Agar initialization
    m_agarView = new Viewer(Controller::executeAction);
    m_agarController = new Controller(m_agarView);
}

Centralisateur* Fabrique::getCentralisateur()
{
    return m_centralisateur;
}

Viewer* Fabrique::getAgarView()
{
    return m_agarView;
}

Controller* Fabrique::getAgarController()
{
    return m_agarController;
}
