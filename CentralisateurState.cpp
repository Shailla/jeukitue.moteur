#include "CentralisateurState.h"

CentralisateurState::CentralisateurState(void)
{
}

CentralisateurState::~CentralisateurState(void)
{
}

bool CentralisateurState::setConnectionInProgress(void)
{
    if(m_state == STATE_Disconnected) {
        m_state = STATE_ConnectionInProgress;
        return true;
    } else {
        return false;
    }
}