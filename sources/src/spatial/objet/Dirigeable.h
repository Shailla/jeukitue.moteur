#ifndef __JKT__DIRIGEABLE_H
#define __JKT__DIRIGEABLE_H

#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#include "spatial/Mouve.h"

#include "data/ValeurFloat.h"

class CGame;
class CPlayer;

namespace jkt
{

class CMap;
class CIstreamMap;

class Dirigeable : public CMouve {

public:
	Dirigeable();
	~Dirigeable();

	void Affiche();					// Fonction d'affichage de l'objet géométrique
	static void Affiche(Branche* branche);
	void Refresh(CGame *game);		// Rafraichissement des données, position, vitesse, ... de l'objet
};

}	// JktMoteur

#endif

