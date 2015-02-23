#ifndef __JKT__DIRIGEABLE_H
#define __JKT__DIRIGEABLE_H

#include "spatial/Mouve.h"

#include "data/ValeurFloat.h"

class CGame;
class CPlayer;

namespace JktMoteur
{

class CMap;
class CIstreamMap;

class Dirigeable : public CMouve {
	CMap* _map;

	// Vitesse de l'objet dans l'arbre de données
	ValeurFloat* _vitesseX;
	ValeurFloat* _vitesseY;
	ValeurFloat* _vitesseZ;

	// Position de l'objet dans l'arbre de données
	ValeurFloat* _positionX;
	ValeurFloat* _positionY;
	ValeurFloat* _positionZ;

	// Couleur de l'objet dans l'arbre de données
	ValeurFloat* _colorR;
	ValeurFloat* _colorG;
	ValeurFloat* _colorB;

public:
	Dirigeable(CMap *map);
	~Dirigeable();

	void Init();					// Initialisation de l'objet géométrique
	void Affiche();					// Fonction d'affichage de l'objet géométrique
	static void Affiche(Branche* branche);
	void Refresh(CGame *game);		// Rafraichissement des données, position, vitesse, ... de l'objet
};

}	// JktMoteur

#endif

