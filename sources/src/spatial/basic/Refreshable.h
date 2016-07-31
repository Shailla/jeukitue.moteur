
#ifndef __JKT__MOUVE_H
#define __JKT__MOUVE_H

#include "spatial/basic/Object.h"

class CGame;

namespace jkt
{

class Refreshable {
public:
	Refreshable();
	virtual ~Refreshable(void);

	virtual void refresh( CGame *game ) = 0;
};

}	// JktMoteur

#endif

