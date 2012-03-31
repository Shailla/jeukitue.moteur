#ifndef __JKT__DATA_H
#define __JKT__DATA_H

#include "SDL_net.h"

class Data
{
protected:
	static const int CODE_DATA_SIZE = 4;
public:
    Data(void);
    virtual ~Data(void);

    virtual UDPpacket* toPacket() = 0;
};

#endif
