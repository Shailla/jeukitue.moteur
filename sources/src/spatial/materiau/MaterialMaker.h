#ifndef __JKT__MATERIALMAKER_H
#define __JKT__MATERIALMAKER_H

#include <string>

#include "tinyxml.h"

#include "spatial/materiau/Material.h"

namespace jkt
{

class CMaterialMaker
{
public:
	static CMaterial* Lit(TiXmlElement* el, std::string &repertoire, MapLogger* mapLogger);
};

}

#endif
