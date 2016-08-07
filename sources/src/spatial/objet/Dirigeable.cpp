
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <map>

using namespace std;

#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/glew.h>
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

namespace jkt
{

Dirigeable::Dirigeable(CMap* map) : MapObject(map) {
}

Dirigeable::Dirigeable(const Dirigeable& other) : MapObject(other) {
}

Dirigeable::~Dirigeable(void) {
}

void Dirigeable::AfficheSelection(float r, float g, float b) {		// Fonction d'affichage de la porte
	Affiche();
}

void Dirigeable::Affiche() {		// Fonction d'affichage de la porte
}

void Dirigeable::Affiche(Branche* branche) {		// Fonction d'affichage de la porte
	glPushMatrix();

	float x = ((ValeurFloat*)branche->getValeurByIdOrTmpId(0, 1))->getValeur();
	float y = ((ValeurFloat*)branche->getValeurByIdOrTmpId(0, 2))->getValeur();
	float z = ((ValeurFloat*)branche->getValeurByIdOrTmpId(0, 3))->getValeur();

	float red = ((ValeurFloat*)branche->getValeurByIdOrTmpId(0, 4))->getValeur();
	float green = ((ValeurFloat*)branche->getValeurByIdOrTmpId(0, 5))->getValeur();
	float blue = ((ValeurFloat*)branche->getValeurByIdOrTmpId(0, 6))->getValeur();

	glTranslatef(x, y, z);	// Translation de la porte de la valeur de son ouverture
	glColor3f(red, green, blue);

	GLUquadric* quadric = gluNewQuadric();
	gluSphere(quadric, 0.05, 16, 16);

	glPopMatrix();
}

MapObject* Dirigeable::clone() {
	return new Dirigeable(*this);
}

void Dirigeable::init() throw(CErreur) {
}

void Dirigeable::refresh(CGame *game) {
}

bool Dirigeable::Lit(TiXmlElement* el, MapLogger* mapLogger) {
	return true;
}

bool Dirigeable::Save(TiXmlElement* element) {
	return true;
}

}	// JktMoteur
