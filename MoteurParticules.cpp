
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

#include "Particule.h"
#include "MoteurParticules.h"

namespace JKT_PACKAGE_MOTEUR3D
{

#define CSTE_K	1.0f
#define TAILLE_TEX 0.04f		// Dimension de la texture carrée de la particule

CMoteurParticules::CMoteurParticules(CV3D pos_centre, unsigned int nbr, float m)
{
TRACE().p( TRACE_MOTEUR3D, "CMoteurParticules::CMoteurParticules(pos_centre,nbr=%u,m=%f) begin%T", nbr, m, this );
	CV3D posRND;

	centre = pos_centre;		// Centre attracteur
	masse = m;					// Masse du centre attracteur
	nbrParticules = nbr;		// Nombre de particules du moteur
	ListeParticules = new CParticule[nbr];	// Liste des particules du moteur
	GenereTextureParticule();	// Génère la texture affichée pour les particules
	lastTempsNew = SDL_GetTicks();
	srand( lastTempsNew );		// Initialisation de la fonction rand() pour les nombres aléatoires
	vitesse.X = vitesse.Y = vitesse.Z = 0.0f;

	for( unsigned int i=0 ; i<nbr ; i++)	// Initialisation des particules
	{
		posRND.X = 3.0f*((float)rand()/(float)RAND_MAX-0.5f);
		posRND.Y = -1.5f*((float)rand()/(float)RAND_MAX);
		posRND.Z = 3.0f*((float)rand()/(float)RAND_MAX-0.5f);

		ListeParticules[i].number = i;

		ListeParticules[i].position = pos_centre+posRND;
		ListeParticules[i].vitesse = vitesse;
		ListeParticules[i].masse = 1;
		ListeParticules[i].duree2vie = 10000;

		ListeParticules[i].acceleration.X = 0.0f;
		ListeParticules[i].acceleration.Y = -0.0005f;
		ListeParticules[i].acceleration.Z = 0.0f;
		ListeParticules[i].visible = true;
		ListeParticules[i].date2naissance -= (unsigned int)((posRND.Y/1.5f)*10000);
	}
TRACE().p( TRACE_MOTEUR3D, "CMoteurParticules::CMoteurParticules() end%T", this );
}

void CMoteurParticules::GenereTextureParticule()
{
TRACE().p( TRACE_MOTEUR3D, "CMoteurParticules::GenereTextureParticule() begin%T", this );
	unsigned char *pixels = new unsigned char[32*32*4*sizeof(unsigned char)];

	unsigned int i = 0;
	int intensite;
	float dx, dy, distance;

	for( int y=0 ; y<32 ; y++ )
	{
		for( int x=0 ; x<32 ; x++ )
		{
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

	glGenTextures( 1, &texName );
	glBindTexture( GL_TEXTURE_2D, texName );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

	delete[] pixels;

TRACE().p( TRACE_MOTEUR3D, "CMoteurParticules::GenereTextureParticule() Texture neige : %d end%T", texName, this );
}

void CMoteurParticules::Affiche()
{
	unsigned int tempsICI;
	CV3D Var, accRND;
	CParticule *particule;

		// Calcule les nouvelles coordonnées du moteur de particules
	centre += vitesse;

		// Calcul du plan orthogonal à l'axe de la vue
	GLfloat mat[16];
	glGetFloatv( GL_MODELVIEW_MATRIX, mat );
	CV3D v_droit( mat[0], mat[4], mat[8] );
	CV3D v_haut( mat[1], mat[5], mat[9] );
	CV3D a, b, c, d;

	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, texName );

	tempsICI = SDL_GetTicks();

	for( unsigned int i=0 ; i<nbrParticules ; i++ )
	{
		particule = &ListeParticules[i];

		if( (particule->position.Y>-0.205f) && particule->visible )
		{
			particule->Calcule();		// Calcule les paramètres

			if( particule->vitesse.norme()>0.01f )
				particule->vitesse = particule->vitesse.directeur()*0.01f;
		}
		else if( (!particule->Vie(tempsICI)) || (!particule->visible) )	// Si la particule est morte
		{
			particule->visible = false;

			if( tempsICI-lastTempsNew>5 )	// Temps min séparant la renaissance de 2 particules
			{
				lastTempsNew = tempsICI;	// Date la dernière renaissance de particule

				CV3D posRND;
				posRND.X = 3.0f*((float)rand()/(float)RAND_MAX-0.5f);
				posRND.Y = 0.5f*((float)rand()/(float)RAND_MAX-0.5f);
				posRND.Z = 3.0f*((float)rand()/(float)RAND_MAX-0.5f);

				particule->Reset( centre+posRND, vitesse );
			}
			else
			{
				continue;
			}
		}

		/*C PAS ICI NON PLUS */if( particule->visible )
		{
				// Affichage de la particule
			a = particule->position - ((v_haut+v_droit)*TAILLE_TEX);
			b = particule->position + ((v_droit-v_haut)*TAILLE_TEX);
			c = particule->position + ((v_haut+v_droit)*TAILLE_TEX);
			d = particule->position + ((v_haut-v_droit)*TAILLE_TEX);

		/*C PAS ICI	*/glBegin( GL_QUADS );
				glTexCoord2f( 0.0f, 0.0f );
				glVertex3f( a.X, a.Y, a.Z );

				glTexCoord2f( 0.0f, 1.0f );
				glVertex3f( b.X, b.Y, b.Z );

				glTexCoord2f( 1.0f, 1.0f );
				glVertex3f( c.X, c.Y, c.Z );

				glTexCoord2f( 1.0f, 0.0f );
				glVertex3f( d.X, d.Y, d.Z );
			glEnd();
		}
	}
	glDisable( GL_TEXTURE_2D );
}

}	// JKT_PACKAGE_MOTEUR3D
