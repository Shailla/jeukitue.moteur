#ifndef __JKT__DATA_H
#define __JKT__DATA_H

#include "SDL_net.h"

class Data
{
public:
    Data(void);
    ~Data(void);

    virtual UDPpacket* toPacket() = 0;
};

#endif
