#ifndef __JKT__CLIGHTTARGET_H
#define __JKT__CLIGHTTARGET_H

#include <iostream>

#include "spatial/light/Light.h"

namespace jkt
{

class CLightTarget : public CLight
{
	float m_Direction[ 4 ];
	float m_fFallOff;

public:
	CLightTarget(CMap* map);
	~CLightTarget();

	MapObject* clone() override;
	void init() noexcept(false) override;

	bool Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger) noexcept(false) override;
	bool Save(TiXmlElement* element) noexcept(false) override;

	void Active() override;
	void Desactive() override;

	void SetDirection( float direction[3] );
	void getDirection(float direction[3]) const;
	void SetFallOff( float falloff );
	float getFallOff() const;

	void echangeXY();										// Echange les coordonn�es X et Y de l'objet
	void echangeXZ();										// Echange les coordonn�es X et Z de l'objet
	void echangeYZ();										// Echange les coordonn�es Y et Z de l'objet
	void scale(float scaleX, float scaleY, float scaleZ);	// Homoth�tie pond�r�e selon X, Y et Z de l'objet
};

}	// JktMoteur

#endif
