
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <map>

using namespace std;

#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/gl.h>

class CGame;

#include "spatial/XmlVocabulaire.h"
#include "util/Erreur.h"
#include "util/V3D.h"
#include "spatial/Map.h"
#include "spatial/IfstreamMap.h"
#include "spatial/geo/GeoMaker.h"
#include "spatial/materiau/MaterialTexture.h"
#include "spatial/geo/GeoObject.h"
#include "son/DemonSons.h"
#include "main/Player.h"
#include "main/Game.h"

#include "spatial/objet/Dirigeable.h"

namespace JktMoteur
{

Dirigeable::Dirigeable(CMap *map) {
	_map = map;
	DataTree* dataTree = Game.getLocalDataTree();

	vector<int> brancheId;
	brancheId.push_back(1);
	brancheId.push_back(1);

	_vitesseX = dataTree->getValeur(brancheId, 1);
	_vitesseY = dataTree->getValeur(brancheId, 2);
	_vitesseZ = dataTree->getValeur(brancheId, 3);

	_positionX = dataTree->getValeur(brancheId, 4);
	_positionY = dataTree->getValeur(brancheId, 5);
	_positionZ = dataTree->getValeur(brancheId, 6);
}

Dirigeable::~Dirigeable(void) {
}

void Dirigeable::Init() {			// Initialisation de l'objet géométrique
	_map->Init();
}

void Dirigeable::Affiche() {		// Fonction d'affichage de la porte
	glPushMatrix();

	glTranslatef(_positionX->getValeur(), _positionY->getValeur(), _positionZ->getValeur());	// Translation de la porte de la valeur de son ouverture
//	_map->Affiche();						// Affichage
	GLUquadric* quadric = gluNewQuadric();
	gluSphere(quadric, 0.05, 16, 16);

	glPopMatrix();
}

void Dirigeable::Refresh(CGame *game) {
}

}	// JktMoteur
