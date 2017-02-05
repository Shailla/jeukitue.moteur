#ifndef __JKT__DIRIGEABLE_H
#define __JKT__DIRIGEABLE_H

#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#include "spatial/basic/MapObject.h"

#include "data/ValeurFloat.h"

class CGame;
class CPlayer;

namespace jkt
{

class CMap;
class CIstreamMap;

class Dirigeable : public MapObject {

public:
	Dirigeable(CMap* map);
	Dirigeable(const Dirigeable& other);
	~Dirigeable();

	void Affiche() override;										// Fonction d'affichage de l'objet géométrique
	void AfficheHighlighted(float r, float g, float b) override;	// Fonction d'affichage de l'objet géométrique
	static void Affiche(Branche* branche);
	void refresh(CGame *game) override;								// Rafraichissement des données, position, vitesse, ... de l'objet
	MapObject* clone() override;
	void init() throw(CErreur);

	// Sérialisation
	bool Lit(TiXmlElement* el, MapLogger* mapLogger) override;
	bool Save(TiXmlElement* element) override;
};

}	// JktMoteur

#endif

