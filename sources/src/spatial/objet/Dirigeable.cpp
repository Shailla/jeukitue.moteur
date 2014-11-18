
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

Dirigeable::Dirigeable(CMap *map, const vector<int> vitesseDataId, const vector<int> positionDataId) {
	_map = map;
}

Dirigeable::~Dirigeable(void) {
}

void Dirigeable::Init() {			// Initialisation de l'objet géométrique
	_map->Init();
}

void Dirigeable::Affiche() {		// Fonction d'affichage de la porte
	glPushMatrix();

	glTranslatef(_positionX.getValeur(), _positionY.getValeur(), _positionZ.getValeur());	// Translation de la porte de la valeur de son ouverture
	_map->Affiche();						// Affichage

	glPopMatrix();
}

void Dirigeable::Refresh(CGame *game) {
}

}	// JktMoteur
