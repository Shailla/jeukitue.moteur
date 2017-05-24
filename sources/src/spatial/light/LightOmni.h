
#ifndef __JKT__CLIGHTOMNI_H
#define __JKT__CLIGHTOMNI_H

#include <iostream>

#include "tinyxml.h"

#include "spatial/light/Light.h"

namespace jkt
{

class CLightOmni : public CLight
{
public:
	CLightOmni(CMap* map);
	~CLightOmni();

	MapObject* clone() override;
	void init() throw(CErreur) override;

	bool Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger);
	bool LitFichier(CIfstreamMap &fichier);
	bool SaveFichierMap(std::ofstream &fichier);
	bool Save(TiXmlElement* element);

	void Active();
	void Desactive();
};

}	// JktMoteur

#endif
