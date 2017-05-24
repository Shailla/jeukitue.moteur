
#include <vector>
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/glu.h>
#include <cmath>
#include <iostream>

#include "SDL.h"

#include "util/V3D.h"
#include "util/Trace.h"

#include "spatial/XmlVocabulaire.h"
#include "spatial/moteur/neige/mieux/Flocon.h"
#include "spatial/moteur/neige/mieux/MoteurNeige.h"

using namespace std;

namespace jkt
{

#define CSTE_K	1.0f
#define TAILLE_TEX 0.04f		// Dimension de la texture carrée de la particule

MoteurNeige::MoteurNeige(CMap* parent) : CMoteurParticules(parent) {
	_texName = -1;
	_flocons = 0;
	_nbrParticules = 0;
	_nbrParticulesOnGround = 0;
}

MoteurNeige::~MoteurNeige() {
	delete[] _flocons;
}

MapObject* MoteurNeige::clone() {
	return new MoteurNeige(*this);
}

void MoteurNeige::init() throw(CErreur) {
}

void MoteurNeige::randomPosition(Flocon& flocon) {
	flocon._position.X = _centre.X + (_taille.X * (((float)rand() / (float)RAND_MAX) - 0.5f));
	flocon._position.Y = _centre.Y + (_taille.Y / 2);
	flocon._position.Z = _centre.Z + (_taille.Z * (((float)rand() / (float)RAND_MAX) - 0.5f));
}

void MoteurNeige::randomVelocity(Flocon& flocon) {
	flocon._vitesse.X = -0.00003f + 0.00006f * ((float)rand() / (float)RAND_MAX);
	flocon._vitesse.Y = -0.0001f - 0.0005f * ((float)rand() / (float)RAND_MAX);
	flocon._vitesse.Z = -0.00003f + 0.00006f * ((float)rand() / (float)RAND_MAX);

	flocon._acceleration.X = -0.00003f + 0.00006f * ((float)rand() / (float)RAND_MAX);
	flocon._acceleration.Y = -0.0001f - 0.0005f * ((float)rand() / (float)RAND_MAX);
	flocon._acceleration.Z = -0.00003f + 0.00006f * ((float)rand() / (float)RAND_MAX);
}

bool MoteurNeige::Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger){
	if(strcmp(Xml::NEIGE, el->Value())) {
		Xml::throwCorruptedMapFileException(Xml::NEIGE, el->Value());
	}

	// Nom
	const char* nom = el->Attribute(Xml::NOM);
	if(nom) {
		setName(nom);
	}

	// Lecture du nombre de particules à afficher
	int nbrParticules, nbrParticulesOnGround;

	if(!el->Attribute(Xml::NBR_PARTICULES, &nbrParticules)) {		// Lecture nom de la Map à importer
		mapLogger->logError("Fichier Map corrompu : Nombre de particules du moteur manquant");
		throw CErreur("Fichier Map corrompu : Nombre de particules du moteur manquant");
	}

	if(!el->Attribute(Xml::NBR_PARTICULES_ON_GROUND, &nbrParticulesOnGround)) {		// Lecture nom de la Map à importer
		mapLogger->logInfo("Fichier Map : Nombre de particules du moteur sur le sol manquant absent, application valeur par défaut");
		nbrParticulesOnGround = nbrParticules / 3;
	}

	// Lecture de la position du moteur de neige
	float position[3];

	if(!Xml::Lit3fv(el, Xml::CENTRE, Xml::X, Xml::Y, Xml::Z, position)) {
		position[0] = 0.0;
		position[1] = 0.0;
		position[2] = 0.0;
	}

	// Lecture des dimensions du moteur de neige
	float dimension[3];

	if(!Xml::Lit3fv(el, Xml::DIMENSION, Xml::X, Xml::Y, Xml::Z, dimension)) {
		dimension[0] = 0.0;
		dimension[1] = 0.0;
		dimension[2] = 0.0;
	}

	_centre = position;
	_taille = dimension;


	_nbrParticules = nbrParticules;
	_nbrParticulesOnGround = nbrParticulesOnGround;

	_flocons = new Flocon[_nbrParticules];			// Liste des particules du moteur

	for(int i=0 ; i<_nbrParticules ; i++)	{	// Initialisation des particules
		Flocon& flocon = _flocons[i];

		if(i < _nbrParticulesOnGround) {
			// Crée les particules au sol
			randomPosition(flocon);
			flocon._position.Y = 0.0f;	// Met le flocon au sol
			flocon.changeState(Flocon::FloconState::ON_THE_GROUND);
			_floconsOnGround.push(&flocon);
		}
		else {
			// Crée les particules qui tombent
			randomPosition(flocon);
			flocon._position.Y = _centre.Y + (_taille.Y * (((float)rand() / (float)RAND_MAX) - 0.5f));
			randomVelocity(flocon);
			flocon.changeState(Flocon::FloconState::FALLING);
		}
	}

	return true;
}

bool MoteurNeige::Save(TiXmlElement* el) {

	TiXmlElement *elEngine = new TiXmlElement(Xml::NEIGE);
	el->LinkEndChild(elEngine);

	el->SetAttribute(Xml::NOM, getName());

	// Nombres de particules
	elEngine->SetAttribute(Xml::NBR_PARTICULES, _nbrParticules);
	elEngine->SetAttribute(Xml::NBR_PARTICULES_ON_GROUND, _nbrParticulesOnGround);

	// Position du centre du moteur de particules
	TiXmlElement* elPos = new TiXmlElement(Xml::CENTRE);
	Xml::SaveAttribut(elPos, Xml::X, _centre.X);
	Xml::SaveAttribut(elPos, Xml::Y, _centre.Y);
	Xml::SaveAttribut(elPos, Xml::Z, _centre.Z);
	elEngine->LinkEndChild(elPos);

	// Dimensions du moteur de particules
	TiXmlElement* elDim = new TiXmlElement(Xml::DIMENSION);
	Xml::SaveAttribut(elDim, Xml::X, _taille.X);
	Xml::SaveAttribut(elDim, Xml::Y, _taille.Y);
	Xml::SaveAttribut(elDim, Xml::Z, _taille.Z);
	elEngine->LinkEndChild(elPos);

	return true;
}

void MoteurNeige::initGL() {
	GenereTextureParticule();
}

void MoteurNeige::freeGL() {
	glDeleteTextures(1, &_texName);
}

void MoteurNeige::GenereTextureParticule() {
	unsigned char *pixels = new unsigned char[32*32*4*sizeof(unsigned char)];

	int i = 0;
	int intensite;
	float dx, dy, distance;

	for(int y=0 ; y<32 ; y++) {
		for(int x=0 ; x<32 ; x++) {
			dx = (float)x - 16.0f;
			dy = (float)y - 16.0f;
			distance = sqrtf( (dx*dx) + (dy*dy) )/22.6274f; // sqrtf()/sqrtf(16*16 + 16*16)
			intensite = (int)(expf(-distance*8)*512.0f);
			if( intensite>=255 )
				intensite = 255;

			pixels[i] = (unsigned char)(0.8f*intensite);
			pixels[i+1] = (unsigned char)(0.8f*intensite);
			pixels[i+2] = (unsigned char)intensite;
			pixels[i+3] = (unsigned char)intensite;
			i += 4;
		}
	}

	glGenTextures(1, &_texName);
	glBindTexture(GL_TEXTURE_2D, _texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	delete[] pixels;
}

void MoteurNeige::AfficheHighlighted(float r,float v,float b) {
	// TODO
}

void MoteurNeige::Affiche() {
	glEnable( GL_BLEND );
	glDepthMask( GL_FALSE );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );

	// Calcul du plan orthogonal à l'axe de la vue
	GLfloat mat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	CV3D v_droit(mat[0], mat[4], mat[8]);
	CV3D v_haut(mat[1], mat[5], mat[9]);
	CV3D a, b, c, d;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _texName);

	glBegin(GL_QUADS);

	for(int i=0 ; i<_nbrParticules ; i++) {
		Flocon& flocon = _flocons[i];

		// Affichage de la particule
		a = flocon._position - ((v_haut+v_droit)*TAILLE_TEX);
		b = flocon._position + ((v_droit-v_haut)*TAILLE_TEX);
		c = flocon._position + ((v_haut+v_droit)*TAILLE_TEX);
		d = flocon._position + ((v_haut-v_droit)*TAILLE_TEX);

		glTexCoord2f( 0.0f, 0.0f );
		glVertex3f( a.X, a.Y, a.Z );

		glTexCoord2f( 0.0f, 1.0f );
		glVertex3f( b.X, b.Y, b.Z );

		glTexCoord2f( 1.0f, 1.0f );
		glVertex3f( c.X, c.Y, c.Z );

		glTexCoord2f( 1.0f, 0.0f );
		glVertex3f( d.X, d.Y, d.Z );
	}

	glEnd();

	glDisable( GL_TEXTURE_2D );
}

void MoteurNeige::refresh(CGame *game) {
	CV3D Var, accRND;

	for(int i=0 ; i<_nbrParticules ; i++) {
		Flocon& flocon = _flocons[i];

		if(flocon._state == Flocon::FloconState::FALLING) {
			flocon._vitesse += flocon._acceleration;

			if( flocon._vitesse.norme()>0.003f )	// Limite la vitesse
				flocon._vitesse = flocon._vitesse.directeur()*0.003f;

			flocon._position += flocon._vitesse;		// Vitesse relative + vitesse absolue

			if(flocon._position.Y <= _centre.Y - (_taille.Y / 2)) {
				flocon._position.Y = _centre.Y - (_taille.Y / 2);
				flocon.changeState(Flocon::FloconState::ON_THE_GROUND);
				_floconsOnGround.push(&flocon);
			}
		}
	}

	// Quand le nombre de flocons au sol dépasse le seuil on refait tomber les premiers à être arriver au sol
	int newFallingFlocons = _floconsOnGround.size() - _nbrParticulesOnGround;

	for(int i=0 ; i<newFallingFlocons ; i++) {
		Flocon& flocon = *_floconsOnGround.front();
		_floconsOnGround.pop();

		flocon.changeState(Flocon::FloconState::FALLING);
		randomPosition(flocon);
		randomVelocity(flocon);
	}
}

}	// JktMoteur
