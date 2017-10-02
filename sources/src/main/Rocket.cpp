
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/glu.h>

#include <fstream>
#include <map>
#include <cmath>

#include "util/Erreur.h"
#include "util/Trace.h"
#include "util/V3D.h"
#include "main/Projectil.h"
#include "util/TableauIndex.h"
#include "util/Tableau.cpp"
#include "main/Game.h"
#include "spatial/Map.h"
#include "son/DemonSons.h"
#include "reseau/SPA.h"
#include "main/Player.h"
#include "spatial/materiau/Textures.h"
#include "spatial/Map.h"
#include "spatial/widget/Texture.h"

#include "Rocket.h"

using namespace std;
using namespace jkt;

extern CGame Game;

Texture* CRocket::_textureExplosion = NULL;		// Texture pour l'explosion de la rocket
bool CRocket::m_B_INIT_CLASSE = false;	// Par d�faut la classe n'a pas encore �t� initialis�e
jkt::CMap* CRocket::_mapRocket = NULL;			// Image 3D de la rocket

#define Pi 3.14159265f
#define ROCKET_RAYON			0.01f
#define DISTANCE_TOUCHE			0.1f
#define TAILLE_TEX_EXPLOSION	0.1f

bool CRocket::INIT_CLASSE() {
	LOGDEBUG(("CRocket::INIT_CLASSE()"));
	// Initialise la classe si elle ne l'a pas encore �t�
	if( !m_B_INIT_CLASSE ) {
		try {
			Uint8* pixels = jkt::litFichierImage("@Texture/Explosion.jpg", 0.75f);
			_textureExplosion = jkt::litFichierTexture("@Texture/Explosion.jpg", 0.75f , pixels);
		}
		catch(CErreur& erreur) {
			LOGWARN(("Echec lecture icone d'explosion du rocket"));
			_textureExplosion = NULL;
			return false;	// L'initialisation a �chou�
		}

		try {
			_mapRocket = new jkt::CMap(0);
			_mapRocket->Lit("@Arme\\Missile", 0);
			_mapRocket->init();
		}
		catch(CErreur& error) {
			stringstream msg;
			msg << "Echec de lecture du missile : " << error.what();
			LOGWARN((msg));
			delete _mapRocket;
			_mapRocket = 0;
		}

		m_B_INIT_CLASSE = true;	// Indique que l'initialisation a �t� faite
	}

	return true;	// L'initialisation a r�ussi
}

CRocket::CRocket(CPlayer *player) : CProjectil(player) {
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
	m_Phi = player->Phi();				//
	m_State = ROCKET_STATE_DEPL;		// Etat de d�part
	m_Taille = ROCKET_RAYON;
}

CRocket::~CRocket() {
}

void CRocket::afficheRocket() {
	glPushMatrix();
	glTranslatef(m_Pos.X, m_Pos.Y, -m_Pos.Z);
	//glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	glRotated(-m_Teta, 0.0f, 1.0f, 0.0f);	//Rotation par rapport � l'axe verticale
	glRotated(-m_Phi, 1.0, 0.0, 0.0);

	_mapRocket->Affiche();

	glPopMatrix();

}

void CRocket::afficheExplosion() {
	// Calcul du plan orthogonal � l'axe de la vue
	GLfloat mat[16];
	glGetFloatv( GL_MODELVIEW_MATRIX, mat );
	CV3D v_droit( mat[0], mat[4], mat[8] );
	CV3D v_haut( mat[1], mat[5], mat[9] );

	// Affichage de l'explosion
	CV3D a = - ( (v_haut+v_droit)*TAILLE_TEX_EXPLOSION );
	CV3D b = +( (v_droit-v_haut)*TAILLE_TEX_EXPLOSION );
	CV3D c = +( (v_haut+v_droit)*TAILLE_TEX_EXPLOSION );
	CV3D d = +( (v_haut-v_droit)*TAILLE_TEX_EXPLOSION );

	glPushMatrix();

	CV3D pos_explosion;
	pos_explosion = m_Pos - m_Dir*0.04f;	// Recule un peu l'explosion par rapport � la position o� elle se passe
	glTranslatef(pos_explosion.X, pos_explosion.Y, -pos_explosion.Z);

	_textureExplosion->afficheQuad(a, b, c, d);

	glPopMatrix();
}

void CRocket::Affiche() {
	switch(m_State) {
	case ROCKET_STATE_DEPL:
		afficheRocket();
		break;

	case ROCKET_STATE_EXPLOSION:
		afficheExplosion();
		break;

	default:
		break;
	}
}

void CRocket::Deplace() {
	CV3D EH;
	float pos[3], pos2[3];
	pos[0] = m_Pos.X;
	pos[1] = m_Pos.Y;
	pos[2] = m_Pos.Z;

	if(Game.getMap()->checkContact(pos, ROCKET_RAYON*1.02f))	// Si la rocket touche un objet
		m_State = ROCKET_STATE_CONTACT;	// La rocket doit exploser
	else
		m_Pos += m_Dir * 0.02f;	// Le mouvement de la rocket continue

	// V�rifie si un joueur passe assez pr�s de la rocket pour d�clencher son explosion
	CPlayer *playerTouche = 0;	// Eventuel touch� par la rocket
	CPlayer *player;
	int curseur = -1;

	while((player = Game.nextPlayer(curseur))) {
		if(player != m_Player) {
			player->getPosition( pos2 );
			EH.X = pos2[0] - pos[0];
			EH.Y = pos2[1] - pos[1];
			EH.Z = pos2[2] - pos[2];

			if(EH.norme() < DISTANCE_TOUCHE)
				playerTouche = player;	// Se souvient quel joeur est touch�
		}
	}

	if(playerTouche) {
		m_State = ROCKET_STATE_EXPLOSION;	// Passe la rocket en mode explosion
		playerTouche->tuer();	// Tue le joueur touch� par la rocket
	}
}

bool CRocket::Refresh() {
	bool vie = true;	// Demande � ce que l'objet CRocket survive (par d�faut)

	switch(m_State) {
	case ROCKET_STATE_DEPL:		// La rocket est en d�placement
		Deplace();
		break;

	case ROCKET_STATE_CONTACT:
		m_State = ROCKET_STATE_EXPLOSION;
		break;

	case ROCKET_STATE_EXPLOSION:		// La rocket est en train d'exploser
		m_Taille += 0.002f;				// La taille de la rocket augmente pour simuler l'explosion

		if( m_Taille > 0.1f )
			vie = false;				// Destruction de CRocket (fin de vie de la rocket)
		break;

	default:
		break;
	}

	return vie;
}
