
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
public:
	// Constructeurs / destructeur
	Geometrical();
	Geometrical(const Geometrical& geo);
	virtual ~Geometrical();

	// Transformations 3D
	virtual void EchangeXY() = 0;										// Echange les coordonn�es X et Y de l'objet
	virtual void EchangeXZ() = 0;										// Echange les coordonn�es X et Z de l'objet
	virtual void EchangeYZ() = 0;										// Echange les coordonn�es Y et Z de l'objet
	virtual void Scale(float scaleX, float scaleY, float scaleZ) = 0;	// Homoth�tie pond�r�e selon X, Y et Z de l'objet
	virtual void translate(float x, float y, float z) = 0;				// Translation pond�r�e selon X, Y et Z de l'objet
};

}	// JktMoteur

#endif

