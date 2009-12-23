#include "Fabrique.h"

Centralisateur* Fabrique::m_centralisateur = 0;
AgarView* Fabrique::m_agarView = 0;
AgarController* Fabrique::m_agarController = 0;

void Fabrique::construct(void)
{
    UdpController* udpController = new UdpController();
    m_centralisateur = new Centralisateur(udpController);

    // Agar initialization
    m_agarView = new AgarView(AgarController::executeAction);
    m_agarController = new AgarController(m_agarView);
}

Centralisateur* Fabrique::getCentralisateur()
{
    return m_centralisateur;
}

AgarView* Fabrique::getAgarView()
{
    return m_agarView;
}

AgarController* Fabrique::getAgarController()
{
    return m_agarController;
}