#ifndef __JKT__SIGNALEMENTDATA_H
#define __JKT__SIGNALEMENTDATA_H

#include <string>
using namespace std;

#include "SDL_net.h"

#include "centralisateur/data/Data.h"

class SignalementData : public Data
{
    string m_userName;
public:
    SignalementData(void);
    ~SignalementData(void);

    void setUserName(const string& userName);

    UDPpacket* toPacket(void);
};

#endif
