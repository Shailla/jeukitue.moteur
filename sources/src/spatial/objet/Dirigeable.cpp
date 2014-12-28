
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
#include <GL/glu.h>

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

extern CGame Game;

namespace JktMoteur
{

Dirigeable::Dirigeable(CMap *map) {
	_map = map;
	DataTree* dataTree = Game.getLocalDataTree();

	vector<int> brancheId;
	brancheId.push_back(1);
	brancheId.push_back(2);
	brancheId.push_back(1);
	brancheId.push_back(1);

	_vitesseX = (ValeurFloat*)dataTree->getValeur(brancheId, 1);
	_vitesseY = (ValeurFloat*)dataTree->getValeur(brancheId, 2);
	_vitesseZ = (ValeurFloat*)dataTree->getValeur(brancheId, 3);

	_positionX = (ValeurFloat*)dataTree->getValeur(brancheId, 4);
	_positionY = (ValeurFloat*)dataTree->getValeur(brancheId, 5);
	_positionZ = (ValeurFloat*)dataTree->getValeur(brancheId, 6);
}

Dirigeable::~Dirigeable(void) {
}

void Dirigeable::Init() {			// Initialisation de l'objet géométrique
//	_map->Init();
}

void Dirigeable::Affiche() {		// Fonction d'affichage de la porte
//	glPushMatrix();
//
//	float x = _positionX->getValeur();
//	float y = _positionY->getValeur();
//	float z = _positionZ->getValeur();
//
//	glTranslatef(x, y, z);	// Translation de la porte de la valeur de son ouverture
////	_map->Affiche();						// Affichage
//	glColor3f(0.0f, 1.0f, 1.0f);
//	GLUquadric* quadric = gluNewQuadric();
//	gluSphere(quadric, 0.05, 16, 16);
//
//	glPopMatrix();
}

void Dirigeable::Affiche(Branche* branche) {		// Fonction d'affichage de la porte
	glPushMatrix();

	float x = ((ValeurFloat*)branche->getValeurByIdOrTmpId(4))->getValeur();
	float y = ((ValeurFloat*)branche->getValeurByIdOrTmpId(5))->getValeur();
	float z = ((ValeurFloat*)branche->getValeurByIdOrTmpId(6))->getValeur();

	glTranslatef(x, y, z);	// Translation de la porte de la valeur de son ouverture
	glColor3f(0.0f, 1.0f, 1.0f);
	GLUquadric* quadric = gluNewQuadric();
	gluSphere(quadric, 0.05, 16, 16);

	glPopMatrix();
}

void Dirigeable::Refresh(CGame *game) {
}

}	// JktMoteur
