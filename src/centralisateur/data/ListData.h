#ifndef __JKT__LISTDATA_H
#define __JKT__LISTDATA_H

#include "centralisateur/data/Data.h"

class ListData : public Data
{
public:
    ListData(UDPpacket* packet);
    ~ListData(void);
};

#endif
