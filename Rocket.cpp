
#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#include <fstream>
#include <map>
#include <cmath>

using namespace std;

#include "Erreur.h"
#include "Trace.h"
#include "V3D.h"
#include "Projectil.h"
#include "TableauIndex.h"
#include "Tableau.cpp"
#include "Game.h"
#include "Map.h"
#include "DemonSons.h"
#include "SPA.h"
#include "Player.h"
#include "Textures.h"
#include "Erreur.h"
#include "Map.h"

#include "Rocket.h"

using namespace JKT_PACKAGE_UTILS;

extern CGame Game;

int CRocket::m_TexExplosion = -1;		// Texture pour l'explosion de la rocket
bool CRocket::m_B_INIT_CLASSE = false;	// Par d�faut la classe n'a pas encore �t� initialis�e
JKT_PACKAGE_MOTEUR3D::CMap* CRocket::pMapRocket = 0;			// Image 3D de la rocket

#define Pi 3.14159265f
#define ROCKET_RAYON			0.01f
#define DISTANCE_TOUCHE			0.1f
#define TAILLE_TEX_EXPLOSION	0.1f

bool CRocket::INIT_CLASSE()
{
TRACE().p( TRACE_OTHER, "CRocket::INIT_CLASSE()" );
		// Initialise la classe si elle ne l'a pas encore �t�
	if( !m_B_INIT_CLASSE )
	{
		try
		{
			m_TexExplosion = JKT_PACKAGE_MOTEUR3D::LitFichierTextureAlpha( "@Texture/Explosion.jpg", 0.75f );
			TRACE().p( TRACE_OTHER, "loadSubIntro() Texture de fonte : %d", m_TexExplosion );
		}
		catch( CErreur erreur )
		{
			cerr << "\nEchec lecture icone d'explosion du rocket";
			m_TexExplosion = -1;
			return false;	// L'initialisation a �chou�
		}

		pMapRocket = new JKT_PACKAGE_MOTEUR3D::CMap("@Arme\\Missile");

		m_B_INIT_CLASSE = true;	// Indique que l'initialisation a �t� faite
	}

	return true;	// L'initialisation a r�ussi
}

CRocket::CRocket( CPlayer *player )
	:CProjectil( player )
{
	float pos[3];
	INIT_CLASSE();	// Initialise la classe (normalement �a a d�j� �t� fait !!!)

		// Calcul de la direction du tir
	m_Dir.X = /*FastSin0( player->Teta/180.0f*Pi )*FastCos0( player->Phi/180.0f*Pi );*/ 
		sinf( player->Teta()/180.0f*Pi )*cosf( player->Phi()/180.0f*Pi );
	m_Dir.Y = /*-FastSin0( player->Phi/180.0f*Pi ) ;*/										
		-sinf( player->Phi()/180.0f*Pi ) ;
	m_Dir.Z = /*-FastCos0( player->Teta/180.0f*Pi )*FastCos0( player->Phi/180.0f*Pi ) ;*/	
		cosf( player->Teta()/180.0f*Pi )*cosf( player->Phi()/180.0f*Pi ) ;
	
	player->getPosition( pos );
	m_Pos = pos;	// Position de d�part de la rocket
	m_Teta = player->Teta();			// Orientation de la rocket	
	m_Phi = player->Phi();			//
	m_State = ROCKET_STATE_DEPL;		// Etat de d�part
	m_Taille = ROCKET_RAYON;
}

CRocket::~CRocket()
{
}

void CRocket::Affiche_S1()
{
	glPushMatrix();
	glTranslatef( m_Pos.X, m_Pos.Y, -m_Pos.Z );		
	//glRotated( 90.0f, 0.0f, 1.0f, 0.0f );	
	glRotated( -m_Teta, 0.0f, 1.0f, 0.0f ); //Rotation par rapport � l'axe verticale
	glRotated( -m_Phi, 1.0, 0.0, 0.0 );
	
	pMapRocket->Affiche();

/*	glBegin(GL_QUADS);
		glColor3f( 1.0, 0.0, 0.0);
		glVertex3f( -m_Taille, -m_Taille, -m_Taille );
		glVertex3f( m_Taille, -m_Taille, -m_Taille );
		glVertex3f( m_Taille, m_Taille, -m_Taille );
		glVertex3f( -m_Taille, m_Taille, -m_Taille );

		glColor3f( 1.0, 0.0, 0.0);
		glVertex3f( -m_Taille, -m_Taille, m_Taille );
		glVertex3f( m_Taille, -m_Taille, m_Taille );
		glVertex3f( m_Taille, m_Taille, m_Taille );
		glVertex3f( -m_Taille, m_Taille, m_Taille );

		glColor3f( 0.0, 0.0, 1.0);
		glVertex3f( -m_Taille, -m_Taille, -m_Taille );
		glVertex3f( -m_Taille, m_Taille, -m_Taille );
		glVertex3f( -m_Taille, m_Taille, m_Taille );
		glVertex3f( -m_Taille, -m_Taille, m_Taille );

		glColor3f( 0.0, 0.0, 1.0);
		glVertex3f( m_Taille, -m_Taille, -m_Taille );
		glVertex3f( m_Taille, m_Taille, -m_Taille );
		glVertex3f( m_Taille, m_Taille, m_Taille );
		glVertex3f( m_Taille, -m_Taille, m_Taille );


		glColor3f( 0.0, 0.5, 0.5);
		glVertex3f( -m_Taille, -m_Taille, -m_Taille );
		glVertex3f( m_Taille, -m_Taille, -m_Taille );
		glVertex3f( m_Taille, -m_Taille, m_Taille );
		glVertex3f( -m_Taille, -m_Taille, m_Taille );

		glColor3f( 0.0, 0.5f, 0.5f);
		glVertex3f( -m_Taille, m_Taille, -m_Taille );
		glVertex3f( m_Taille, m_Taille, -m_Taille );
		glVertex3f( m_Taille, m_Taille, m_Taille );
		glVertex3f( -m_Taille, m_Taille, m_Taille );
	glEnd();
*/
	glPopMatrix();

}

void CRocket::Affiche_S2()
{
		// Calcul du plan orthogonal � l'axe de la vue
	GLfloat mat[16];
	glGetFloatv( GL_MODELVIEW_MATRIX, mat );
	CV3D v_droit( mat[0], mat[4], mat[8] );
	CV3D v_haut( mat[1], mat[5], mat[9] );
	CV3D a, b, c, d;

	glEnable( GL_TEXTURE_2D );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	glBindTexture( GL_TEXTURE_2D, m_TexExplosion );	
	glEnable( GL_BLEND );
	glDepthMask( GL_FALSE );

		// Affichage de l'explosion
	a = - ( (v_haut+v_droit)*TAILLE_TEX_EXPLOSION );
	b = +( (v_droit-v_haut)*TAILLE_TEX_EXPLOSION );
	c = +( (v_haut+v_droit)*TAILLE_TEX_EXPLOSION );
	d = +( (v_haut-v_droit)*TAILLE_TEX_EXPLOSION );

	glPushMatrix();

	CV3D pos_explosion;
	pos_explosion = m_Pos - m_Dir*0.04f;	// Recule un peu l'explosion par rapport � la position o� elle se passe
	glTranslatef( pos_explosion.X, pos_explosion.Y, -pos_explosion.Z );		

	glBegin( GL_QUADS );
		glTexCoord2f( 0.0f, 0.0f );
		glVertex3f( a.X, a.Y, a.Z );
		
		glTexCoord2f( 0.0f, 1.0f );
		glVertex3f( b.X, b.Y, b.Z );
		
		glTexCoord2f( 1.0f, 1.0f );
		glVertex3f( c.X, c.Y, c.Z );
		
		glTexCoord2f( 1.0f, 0.0f );
		glVertex3f( d.X, d.Y, d.Z );
	glEnd();
	
	glDepthMask( GL_TRUE );
	glDisable( GL_BLEND );
	glPopMatrix();
}
/*void CRocket::Affiche_S2()
{
	glEnable( GL_TEXTURE_2D );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	glBindTexture( GL_TEXTURE_2D, m_TexExplosion );	
	
	glPushMatrix();
	glTranslatef( m_Pos.X, m_Pos.Y, -m_Pos.Z );		

	glColor4f( 1.0f, 1.0f, 1.0f, 0.5f );
	glBegin(GL_QUADS);
		glTexCoord2f( 0.0f + m_Taille*15.0f, 0.0f + m_Taille*15.0f );
		glVertex3f( -m_Taille, -m_Taille, -m_Taille );
		glTexCoord2f( 1.0f + m_Taille*15.0f, 0.0f + m_Taille*15.0f );
		glVertex3f( m_Taille, -m_Taille, -m_Taille );
		glTexCoord2f( 1.0f + m_Taille*15.0f, 1.0f + m_Taille*15.0f );
		glVertex3f( m_Taille, m_Taille, -m_Taille );
		glTexCoord2f( 0.0f + m_Taille*15.0f, 1.0f + m_Taille*15.0f );
		glVertex3f( -m_Taille, m_Taille, -m_Taille );

		glTexCoord2f( 0.0f + m_Taille*15.0f, 0.0f + m_Taille*15.0f );
		glVertex3f( -m_Taille, -m_Taille, m_Taille );
		glTexCoord2f( 1.0f + m_Taille*15.0f, 0.0f + m_Taille*15.0f );
		glVertex3f( m_Taille, -m_Taille, m_Taille );
		glTexCoord2f( 1.0f + m_Taille*15.0f, 1.0f + m_Taille*15.0f );
		glVertex3f( m_Taille, m_Taille, m_Taille );
		glTexCoord2f( 0.0f + m_Taille*15.0f, 1.0f + m_Taille*15.0f );
		glVertex3f( -m_Taille, m_Taille, m_Taille );

		glTexCoord2f( 0.0f + m_Taille*15.0f, 0.0f + m_Taille*15.0f );
		glVertex3f( -m_Taille, -m_Taille, -m_Taille );
		glTexCoord2f( 1.0f + m_Taille*15.0f, 0.0f + m_Taille*15.0f );
		glVertex3f( -m_Taille, m_Taille, -m_Taille );
		glTexCoord2f( 1.0f + m_Taille*15.0f, 1.0f + m_Taille*15.0f );
		glVertex3f( -m_Taille, m_Taille, m_Taille );
		glTexCoord2f( 0.0f + m_Taille*15.0f, 1.0f + m_Taille*15.0f );
		glVertex3f( -m_Taille, -m_Taille, m_Taille );

		glTexCoord2f( 0.0f + m_Taille*15.0f, 0.0f + m_Taille*15.0f );
		glVertex3f( m_Taille, -m_Taille, -m_Taille );
		glTexCoord2f( 1.0f + m_Taille*15.0f, 0.0f + m_Taille*15.0f );
		glVertex3f( m_Taille, m_Taille, -m_Taille );
		glTexCoord2f( 1.0f + m_Taille*15.0f, 1.0f + m_Taille*15.0f );
		glVertex3f( m_Taille, m_Taille, m_Taille );
		glTexCoord2f( 0.0f + m_Taille*15.0f, 1.0f + m_Taille*15.0f );
		glVertex3f( m_Taille, -m_Taille, m_Taille );

		glTexCoord2f( 0.0f + m_Taille*15.0f, 0.0f + m_Taille*15.0f );
		glVertex3f( -m_Taille, -m_Taille, -m_Taille );
		glTexCoord2f( 1.0f + m_Taille*15.0f, 0.0f + m_Taille*15.0f );
		glVertex3f( m_Taille, -m_Taille, -m_Taille );
		glTexCoord2f( 1.0f + m_Taille*15.0f, 1.0f + m_Taille*15.0f );
		glVertex3f( m_Taille, -m_Taille, m_Taille );
		glTexCoord2f( 0.0f + m_Taille*15.0f, 1.0f + m_Taille*15.0f );
		glVertex3f( -m_Taille, -m_Taille, m_Taille );

		glTexCoord2f( 0.0f + m_Taille*15.0f, 0.0f + m_Taille*15.0f );
		glVertex3f( -m_Taille, m_Taille, -m_Taille );
		glTexCoord2f( 1.0f + m_Taille*15.0f, 0.0f + m_Taille*15.0f );
		glVertex3f( m_Taille, m_Taille, -m_Taille );
		glTexCoord2f( 1.0f + m_Taille*15.0f, 1.0f + m_Taille*15.0f );
		glVertex3f( m_Taille, m_Taille, m_Taille );
		glTexCoord2f( 0.0f + m_Taille*15.0f, 1.0f + m_Taille*15.0f );
		glVertex3f( -m_Taille, m_Taille, m_Taille );
	glEnd();

	glDisable( GL_BLEND );

	glPopMatrix();
}
*/

void CRocket::Affiche()
{
	switch( m_State )
	{
	case ROCKET_STATE_DEPL:
		Affiche_S1();
		break;

	case ROCKET_STATE_EXPLOSION:
		Affiche_S2();
		break;

	default:
		break;
	}
}

void CRocket::Deplace()
{
	CV3D EH;
	float pos[3], pos2[3];
	pos[0] = m_Pos.X;
	pos[1] = m_Pos.Y;
	pos[2] = m_Pos.Z;

	if( Game.getMap()->Contact( pos, ROCKET_RAYON*1.02f ) )	// Si la rocket touche un objet
		m_State = ROCKET_STATE_CONTACT;	// La rocket doit exploser
	else
		m_Pos += m_Dir * 0.02f;	// Le mouvement de la rocket continue

		// V�rifie si un joueur passe assez pr�s de la rocket pour d�clencher son explosion
	CPlayer *playerTouche = 0;	// Eventuel touch� par la rocket
	CPlayer *player;
	int curseur = -1;				
	while( Game.pTabIndexPlayer->bSuivant( curseur ) )
	{
		player = Game.pTabIndexPlayer->operator [](curseur);
		if( player!=m_Player )
		{
			player->getPosition( pos2 );
			EH.X = pos2[0] - pos[0];
			EH.Y = pos2[1] - pos[1];
			EH.Z = pos2[2] - pos[2];
		
			if( EH.norme() < DISTANCE_TOUCHE )
				playerTouche = player;	// Se souvient quel joeur est touch�
		}
	}

	if( playerTouche )
	{
		m_State = ROCKET_STATE_EXPLOSION;	// Passe la rocket en mode explosion
		playerTouche->tuer();	// Tue le joueur touch� par la rocket
	}
}

bool CRocket::Refresh()
{
	bool vie = true;	// Demande � ce que l'objet CRocket survive (par d�faut)

	switch( m_State )
	{
	case ROCKET_STATE_DEPL:		// La rocket est en d�placement	
		Deplace();
		break;

	case ROCKET_STATE_CONTACT:
		m_State = ROCKET_STATE_EXPLOSION;
		break;

	case ROCKET_STATE_EXPLOSION:		// La rocket est en train d'exploser
		m_Taille += 0.002f;	// La taille de la rocket augmente pour simuler l'explosion
		if( m_Taille > 0.1f )
			vie = false;	// Destruction de CRocket (fin de vie de la rocket)
	
	default:
		break;
	}

	return vie;
}