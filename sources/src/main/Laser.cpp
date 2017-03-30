
#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/glu.h>

#include <fstream>
#include <cmath>
#include <map>

#include "SDL.h"

#include "util/Erreur.h"
#include "util/V3D.h"
#include "son/DemonSons.h"
#include "reseau/SPA.h"
#include "main/Player.h"
#include "main/Game.h"
#include "spatial/Map.h"

#include "Laser.h"

using namespace std;
using namespace jkt;

#define Pi 3.14159265f
#define DUREE_VIE	100			// Dur�e de vie du laser en millisecondes

extern CGame Game;

CLaser::CLaser( CPlayer *player ) : CProjectil( player ) {
	m_TimeStart = SDL_GetTicks();
	m_Delta = 0.0f;
}

CLaser::~CLaser() {
}

void CLaser::Affiche() {
	float che = 0.01f;
	float vect[3];

	glPushMatrix();

	m_Player->getPosition( vect );
	glTranslatef( vect[0], vect[1], -vect[2] );
	glRotated( -m_Player->Teta(), 0.0f, 1.0f, 0.0f ); //Rotation par rapport � l'axe verticale
	glRotated( -m_Player->Phi(), 1.0f, 0.0f, 0.0f );
	glBegin( GL_QUADS );
		glColor3f( 1.0f, 0.0f, 0.0f );

		glVertex3f( -che, -che, 0.0f );
		glVertex3f( che, -che, 0.0f );
		glVertex3f( che, -che, -m_Delta );
		glVertex3f( -che, -che, -m_Delta );

		glVertex3f( che, -che, 0.0f );
		glVertex3f( che, che, 0.0f );
		glVertex3f( che, che, -m_Delta );
		glVertex3f( che, -che, -m_Delta );

		glVertex3f( che, che, 0.0f );
		glVertex3f( -che, che, 0.0f );
		glVertex3f( -che, che, -m_Delta );
		glVertex3f( che, che, -m_Delta );

		glVertex3f( -che, che, 0.0f );
		glVertex3f( -che, -che, 0.0f );
		glVertex3f( -che, -che, -m_Delta );
		glVertex3f( -che, che, -m_Delta );

	glEnd();

	glTranslatef( 0.0f, 0.0f, -m_Delta );
	glColor3f( 0.0f, 0.0f, 1.0f );

	GLUquadric* quadric = gluNewQuadric();
	gluSphere(quadric, 0.05, 16, 16);
	gluDeleteQuadric(quadric);

	glPopMatrix();
}

bool CLaser::Refresh() {
	CV3D Dir, EH;			// Vecteur direction du laser du joueur concern�
	float pos[3], pos2[3];	// Position du joueur concern�
	float distance;
	CPlayer *playerTouche;	// Identifiant du joueur touch� par le laser

		// vecteur direction du personnage (donc aussi du laser !)
	Dir.X = /*FastSin0( player->Teta/180.0f*Pi )*FastCos0( player->Phi/180.0f*Pi );*/
		sinf( m_Player->Teta()/180.0f*Pi )*cosf( m_Player->Phi()/180.0f*Pi );
	Dir.Y = /*-FastSin0( player->Phi/180.0f*Pi ) ;*/
		-sinf( m_Player->Phi()/180.0f*Pi ) ;
	Dir.Z = /*-FastCos0( player->Teta/180.0f*Pi )*FastCos0( player->Phi/180.0f*Pi ) ;*/
		-cosf( m_Player->Teta()/180.0f*Pi )*cosf( m_Player->Phi()/180.0f*Pi ) ;

	m_Player->getPosition( pos );

	distance = 10000.0f;
	CV3D DirVerif;
	DirVerif = Dir;
	distance = Game.getMap()->GereLaserPlayer( pos, Dir, distance );	// G�re le laser de 'player' avec la map

	// V�rifie si un joueur a �t� touch�
	playerTouche = 0;	// Pas de joueur touch� par le laser trouv�
	int curseur = -1;
	CPlayer *player;

	while((player = Game.nextPlayer(curseur))) {
		if(player != m_Player) {
			player->getPosition( pos2 );
			EH.X = pos2[0] - pos[0];
			EH.Y = pos2[1] - pos[1];
			EH.Z = -pos2[2] + pos[2];

			float var = EH^Dir;

			if( var >=0.0f ) {	// Si le laser va dans le sens du joueur
				if( var < distance ) {	// Si le laser n'est pas arr�t� par un objet avant
										// d'atteindre le joueur
					float hcarre = (EH^EH) - (var*var);

					if( hcarre<0.007f ) {	// Si la distance entre le laser et l'autre joueur < 0.007
						distance = var;
						playerTouche = player;	// Se souvient quel joeur est touch�
					}
				}
			}
		}
	}

	if( playerTouche )
		playerTouche->tuer();	// Tue le joueur touch� par le laser

	m_Delta = distance;	// Pour l'affichage du laser, pas tr�s propre, A VOIR

	if( SDL_GetTicks() - m_TimeStart < DUREE_VIE )
		return true;	// Le laser n'a pas fini
	else
		return false;	// Le coup de laser est fini
}
