#ifndef __JKT__CENRALISATEUR_H
#define __JKT__CENRALISATEUR_H

#include <string>
using namespace std;

#include "UdpController.h"

class Centralisateur
{
    int m_portLocal;
    UdpController* m_udpController;
    string m_userName;
public:
    Centralisateur(UdpController* udpController);
    ~Centralisateur(void);

    void connecter(char* userName, char* adresseIp, int port);
    void sendSignalement(void);
	void sendMessageChat(const string& message, const string& userName);
};

#endif
