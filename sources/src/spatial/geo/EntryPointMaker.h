#ifndef __JKT__ENTRYPOINTMAKER_H
#define __JKT__ENTRYPOINTMAKER_H

#include <string>

#include "tinyxml.h"

#include "spatial/geo/EntryPoint.h"

namespace JktMoteur
{

class EntryPointMaker
{
public:
	static EntryPoint* Lit(TiXmlElement* el, MapLogger* mapLogger);
};

}

#endif
