#ifndef __JKT__MATERIALMAKER_H
#define __JKT__MATERIALMAKER_H

#include <string>

#include "tinyxml.h"

#include "spatial/materiau/Material.h"

namespace JKT_PACKAGE_MOTEUR3D
{

class CMaterialMaker
{
public:
	static CMaterial* Lit(TiXmlElement* el, string &repertoire);
};

}

#endif
