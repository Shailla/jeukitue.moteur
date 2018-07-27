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

	void affiche() override;										// Fonction d'affichage de l'objet g�om�trique
	void afficheHighlighted(float r, float g, float b) override;	// Fonction d'affichage de l'objet g�om�trique
	static void Affiche(Branche* branche);
	void refresh(CGame *game) override;								// Rafraichissement des donn�es, position, vitesse, ... de l'objet
	MapObject* clone() override;
	void init() throw(CErreur);

	// S�rialisation
	bool Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger) throw(CErreur) override;
	bool Save(TiXmlElement* element) throw(CErreur) override;
};

}	// JktMoteur

#endif

