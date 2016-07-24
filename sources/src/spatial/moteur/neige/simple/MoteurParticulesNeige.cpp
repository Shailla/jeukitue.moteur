
#include <vector>
#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/glu.h>
#include <cmath>
#include <iostream>

using namespace std;

#include "SDL.h"

#include "util/V3D.h"
#include "util/Trace.h"

#include <spatial/moteur/neige/simple/Particule.h>
#include <spatial/moteur/neige/simple/MoteurParticulesNeige.h>

namespace jkt
{

#define CSTE_K	1.0f
#define TAILLE_TEX 0.04f		// Dimension de la texture carrée de la particule

CMoteurParticulesNeige::CMoteurParticulesNeige(unsigned int nbrParticules, const CV3D& position, const CV3D& taille)
	:CMoteurParticules()
{
LOGDEBUG(("CMoteurParticulesNeige::CMoteurParticulesNeige(pos_centre,nbr=%u) begin%T", nbrParticules, this ));
	_centre = position;
	_taille = taille;
	_nbrParticules = nbrParticules;
	_particules = new CParticule[nbrParticules];	// Liste des particules du moteur

	_lastTempsNew = SDL_GetTicks();
	_vitesse.X = _vitesse.Y = _vitesse.Z = 0.0f;

	for(unsigned int i=0 ; i<nbrParticules ; i++)	{	// Initialisation des particules
		_particules[i].position = getRandomParticulePosition();
		_particules[i].vitesse = _vitesse;
		_particules[i].duree2vie = 10000;

		_particules[i].acceleration.X = -0.00003f + 0.00006f * ((float)rand() / (float)RAND_MAX);
		_particules[i].acceleration.Y = -0.0001f - 0.0005f * ((float)rand() / (float)RAND_MAX);
		_particules[i].acceleration.Z = -0.00003f + 0.00006f * ((float)rand() / (float)RAND_MAX);
		_particules[i].visible = true;
		_particules[i].date2naissance = _lastTempsNew - ((float)rand() / (float)RAND_MAX)*10000;
	}
LOGDEBUG(("CMoteurParticulesNeige::CMoteurParticulesNeige() end%T", this ));
}

CMoteurParticulesNeige::~CMoteurParticulesNeige() {
	delete[] _particules;
}

CV3D CMoteurParticulesNeige::getRandomParticulePosition() {
	CV3D randomPosition;
	randomPosition.X = _centre.X + (_taille.X * (((float)rand() / (float)RAND_MAX) - 0.5f));
	randomPosition.Y = _centre.Y + (_taille.Y / 2);
	randomPosition.Z = _centre.Z + (_taille.Z * (((float)rand() / (float)RAND_MAX) - 0.5f));

	return randomPosition;
}

void CMoteurParticulesNeige::initGL() {
	GenereTextureParticule();
}

void CMoteurParticulesNeige::freeGL() {
	glDeleteTextures(1, &_texName);
}

void CMoteurParticulesNeige::GenereTextureParticule() {
LOGDEBUG(("CMoteurParticulesNeige::GenereTextureParticule() begin%T", this ));
	unsigned char *pixels = new unsigned char[32*32*4*sizeof(unsigned char)];

	unsigned int i = 0;
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

LOGDEBUG(("CMoteurParticulesNeige::GenereTextureParticule() Texture neige : %d end%T", _texName, this ));
}

void CMoteurParticulesNeige::affiche() {
	CV3D Var, accRND;
	CParticule* particule;

	// Calcul du plan orthogonal à l'axe de la vue
	GLfloat mat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	CV3D v_droit(mat[0], mat[4], mat[8]);
	CV3D v_haut(mat[1], mat[5], mat[9]);
	CV3D a, b, c, d;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _texName);

	unsigned int now = SDL_GetTicks();

	glBegin(GL_QUADS);

	for(unsigned int i=0 ; i<_nbrParticules ; i++) {
		particule = &_particules[i];

		if( (particule->position.Y>-0.205f) && particule->visible ) {
			particule->Calcule();		// Calcule les paramètres

			if( particule->vitesse.norme()>0.01f )
				particule->vitesse = particule->vitesse.directeur()*0.01f;
		}
		else if( (!particule->Vie(now)) || (!particule->visible) )	// Si la particule est morte
		{
			particule->visible = false;

			if(now-_lastTempsNew > 5)	// Temps min séparant la renaissance de 2 particules
			{
				_lastTempsNew = now;	// Date la dernière renaissance de particule

				CV3D posRND;
				posRND.X = 3.0f*((float)rand()/(float)RAND_MAX-0.5f);
				posRND.Y = 0.5f*((float)rand()/(float)RAND_MAX-0.5f);
				posRND.Z = 3.0f*((float)rand()/(float)RAND_MAX-0.5f);

				particule->Reset(getRandomParticulePosition(), _vitesse, now);
			}
			else {
				continue;
			}
		}

		if( particule->visible ) {
				// Affichage de la particule
			a = particule->position - ((v_haut+v_droit)*TAILLE_TEX);
			b = particule->position + ((v_droit-v_haut)*TAILLE_TEX);
			c = particule->position + ((v_haut+v_droit)*TAILLE_TEX);
			d = particule->position + ((v_haut-v_droit)*TAILLE_TEX);

			glTexCoord2f( 0.0f, 0.0f );
			glVertex3f( a.X, a.Y, a.Z );

			glTexCoord2f( 0.0f, 1.0f );
			glVertex3f( b.X, b.Y, b.Z );

			glTexCoord2f( 1.0f, 1.0f );
			glVertex3f( c.X, c.Y, c.Z );

			glTexCoord2f( 1.0f, 0.0f );
			glVertex3f( d.X, d.Y, d.Z );
		}
	}

	glEnd();

	glDisable( GL_TEXTURE_2D );
}

}	// JktMoteur
