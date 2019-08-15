
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
	void init() noexcept(false) override;

	bool Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger) noexcept(false) override;
	bool Save(TiXmlElement* element) noexcept(false) override;

	void Active() override;
	void Desactive() override;
};

}	// JktMoteur

#endif
