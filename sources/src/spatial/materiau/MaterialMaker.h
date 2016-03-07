#ifndef __JKT__MATERIALMAKER_H
#define __JKT__MATERIALMAKER_H

#include <string>

using namespace std;

#include "tinyxml.h"

#include "spatial/materiau/Material.h"

namespace JktMoteur
{

class CMaterialMaker
{
public:
	static CMaterial* Lit(TiXmlElement* el, string &repertoire, MapLogger* mapLogger);
};

}

#endif
