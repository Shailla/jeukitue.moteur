
#ifndef __JKT__CLIGHTOMNI_H
#define __JKT__CLIGHTOMNI_H

#include <iostream>

#include "tinyxml.h"

#include "spatial/light/Light.h"

namespace JktMoteur
{

class CLightOmni : public CLight
{
public:
	CLightOmni();
	~CLightOmni();

	bool Lit(TiXmlElement* element, MapLogger* mapLogger);
	bool LitFichier(CIfstreamMap &fichier);
	bool SaveFichierMap(std::ofstream &fichier);
	bool Save(TiXmlElement* element);

	void Active();
	void Desactive();
};

}	// JktMoteur

#endif
