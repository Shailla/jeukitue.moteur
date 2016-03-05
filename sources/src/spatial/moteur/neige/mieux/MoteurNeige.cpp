
#include <vector>
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <iostream>

using namespace std;

#include "SDL.h"

#include "util/V3D.h"
#include "util/Trace.h"

#include "spatial/moteur/neige/mieux/Flocon.h"
#include "spatial/moteur/neige/mieux/MoteurNeige.h"

namespace JktMoteur
{

#define CSTE_K	1.0f
#define TAILLE_TEX 0.04f		// Dimension de la texture carrée de la particule

MoteurNeige::MoteurNeige(int nbrParticules, int nbrParticulesOnGround, const CV3D& centre, const CV3D& taille) : CMoteurParticules() {
	_centre = centre;
	_taille = taille;
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
}

MoteurNeige::~MoteurNeige() {
	delete[] _flocons;
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

void MoteurNeige::affiche() {
	Refresh(0);

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

void MoteurNeige::Refresh(CGame *game) {
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
