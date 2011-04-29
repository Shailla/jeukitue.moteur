
#ifndef __JKT__LIGHTMAKER_H
#define __JKT__LIGHTMAKER_H

#include "tinyxml.h"

namespace JktMoteur
{
class CLight;

class CLightMaker
{
public:
	static CLight* Lit(TiXmlElement* element);
};

}

#endif


