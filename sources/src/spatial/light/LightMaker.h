
#ifndef __JKT__LIGHTMAKER_H
#define __JKT__LIGHTMAKER_H

#include "tinyxml.h"

namespace jkt
{
class CLight;

class CLightMaker
{
public:
	static CLight* Lit(TiXmlElement* element, MapLogger* mapLogger, CMap& map) throw(CErreur);
};

}

#endif


