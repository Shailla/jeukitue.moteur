#include "centralisateur/CentralisateurState.h"

CentralisateurState::CentralisateurState(void) {
	m_state = STATE_Disconnected;
}

CentralisateurState::~CentralisateurState(void) {
}

bool CentralisateurState::setConnectionInProgress(void) {
    if(m_state == STATE_Disconnected) {
        m_state = STATE_ConnectionInProgress;
        return true;
    }
    else {
        return false;
    }
}
