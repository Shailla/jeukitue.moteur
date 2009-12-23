
#ifndef __JKT__LIGHTMAKER_H
#define __JKT__LIGHTMAKER_H

#include "tinyxml.h"

namespace JKT_PACKAGE_MOTEUR3D
{
class CLight;

class CLightMaker
{
public:
	static CLight* Lit(TiXmlElement* element);
};

}

#endif


