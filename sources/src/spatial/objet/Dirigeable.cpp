
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

namespace JktMoteur
{

Dirigeable::Dirigeable() {
}

Dirigeable::~Dirigeable(void) {
}

void Dirigeable::Init() {			// Initialisation de l'objet géométrique
}

void Dirigeable::Affiche() {		// Fonction d'affichage de la porte
}

void Dirigeable::Affiche(Branche* branche) {		// Fonction d'affichage de la porte
	glPushMatrix();

	float x = ((ValeurFloat*)branche->getValeurByIdOrTmpId(4))->getValeur();
	float y = ((ValeurFloat*)branche->getValeurByIdOrTmpId(5))->getValeur();
	float z = ((ValeurFloat*)branche->getValeurByIdOrTmpId(6))->getValeur();

	float red = ((ValeurFloat*)branche->getValeurByIdOrTmpId(7))->getValeur();
	float green = ((ValeurFloat*)branche->getValeurByIdOrTmpId(8))->getValeur();
	float blue = ((ValeurFloat*)branche->getValeurByIdOrTmpId(9))->getValeur();

	glTranslatef(x, y, z);	// Translation de la porte de la valeur de son ouverture
	glColor3f(red, green, blue);

	GLUquadric* quadric = gluNewQuadric();
	gluSphere(quadric, 0.05, 16, 16);

	glPopMatrix();
}

void Dirigeable::Refresh(CGame *game) {
}

}	// JktMoteur
