#ifndef __JKT__ENTRYPOINTMAKER_H
#define __JKT__ENTRYPOINTMAKER_H

#include <string>

#include "tinyxml.h"

#include "spatial/geo/EntryPoint.h"

namespace jkt
{

class EntryPointMaker
{
public:
	static EntryPoint* Lit(CMap& map, TiXmlElement* el, MapLogger* mapLogger) noexcept(false);
};

}

#endif
