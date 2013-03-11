#ifndef __JKT__CENTRALISATEURSTATE_H
#define __JKT__CENTRALISATEURSTATE_H

class CentralisateurState {
    enum State {
        STATE_Disconnected = 0,
        STATE_ConnectionInProgress,
        STATE_Connected
    };
    State m_state;
public:
    CentralisateurState(void);
    ~CentralisateurState(void);

    bool setConnectionInProgress(void);
};

#endif
