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
	CLightTarget();
	~CLightTarget();

	bool Lit(TiXmlElement* element, MapLogger* mapLogger);
	bool LitFichier(CIfstreamMap &fichier);
	bool SaveFichierMap(std::ofstream &fichier);
	bool Save(TiXmlElement* element);

	void Active();
	void Desactive();

	void SetDirection( float direction[3] );
	void getDirection(float direction[3]) const;
	void SetFallOff( float falloff );
	float getFallOff() const;

	void EchangeXY();										// Echange les coordonnées X et Y de l'objet
	void EchangeXZ();										// Echange les coordonnées X et Z de l'objet
	void EchangeYZ();										// Echange les coordonnées Y et Z de l'objet
	void Scale( float scaleX, float scaleY, float scaleZ );	// Homothétie pondérée selon X, Y et Z de l'objet
};

}	// JktMoteur

#endif
