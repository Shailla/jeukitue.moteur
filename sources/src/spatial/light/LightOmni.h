
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

	bool Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger) throw(CErreur) override;
	bool Save(TiXmlElement* element) throw(CErreur) override;

	void Active() override;
	void Desactive() override;
};

}	// JktMoteur

#endif
