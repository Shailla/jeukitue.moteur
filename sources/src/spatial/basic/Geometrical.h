
#ifndef __JKT__GEO_H
#define __JKT__GEO_H

/************************************************************************/
/*																		*/
/*			Cette classe est une interface,								*/
/*			elle est la base de la plupart des objets d'une Map         */
/*																		*/
/************************************************************************/

#include "spatial/basic/Object.h"

class CPlayer;

using namespace jkt;

namespace jkt
{

class CMap;
class CIfstreamMap;

class Geometrical {

protected:
	float _minX, _minY, _minZ, _maxX, _maxY, _maxZ;	// Coordonnées du pavé englobant l'objet géo
	float _centre[3];			// Centre de la sphère englobant l'objet
	float _rayon;				// Rayon de la sphère englobant l'objet

public:
	// Constructeurs / destructeur
	Geometrical();
	Geometrical(const Geometrical& geo);
	virtual ~Geometrical();

	// Transformations 3D
	virtual void echangeXY() = 0;										// Echange les coordonnées X et Y de l'objet
	virtual void echangeXZ() = 0;										// Echange les coordonnées X et Z de l'objet
	virtual void echangeYZ() = 0;										// Echange les coordonnées Y et Z de l'objet
	virtual void scale(float scaleX, float scaleY, float scaleZ) = 0;	// Homothétie pondérée selon X, Y et Z de l'objet
	virtual void translate(float x, float y, float z) = 0;				// Translation pondérée selon X, Y et Z de l'objet
};

}	// JktMoteur

#endif

