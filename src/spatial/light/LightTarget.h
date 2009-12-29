#ifndef __JKT__CLIGHTTARGET_H
#define __JKT__CLIGHTTARGET_H

#include <iostream>

#include "spatial/light/Light.h"

namespace JKT_PACKAGE_MOTEUR3D
{

class CLightTarget : public CLight
{
	float m_Direction[ 4 ];
	float m_fFallOff;

public:
	CLightTarget();
	~CLightTarget();

	bool Lit(TiXmlElement* element);
	bool LitFichier(CIfstreamMap &fichier);
	bool SaveFichierMap(std::ofstream &fichier);
	bool Save(TiXmlElement* element);

	void Active();
	void Desactive();

	void SetDirection( float direction[3] );
	void SetFallOff( float falloff );

	void EchangeXY();										// Echange les coordonnées X et Y de l'objet
	void EchangeXZ();										// Echange les coordonnées X et Z de l'objet
	void EchangeYZ();										// Echange les coordonnées Y et Z de l'objet
	void Scale( float scaleX, float scaleY, float scaleZ );	// Homothétie pondérée selon X, Y et Z de l'objet
};

}	// JKT_PACKAGE_MOTEUR3D

#endif
