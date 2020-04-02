#ifndef __JKT__SIGNALEMENTDATA_H
#define __JKT__SIGNALEMENTDATA_H

#include <string>

#include <SDL_net.h>

#include "centralisateur/data/Data.h"

class SignalementData : public Data {
	std::string m_userName;
public:
    SignalementData(void);
    ~SignalementData(void);

    void setUserName(const std::string& userName);

    UDPpacket* toPacket(void);
};

#endif
