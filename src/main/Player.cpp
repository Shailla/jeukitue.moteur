
#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <map>
#include <cmath>

using namespace std;

#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#include "fmod.h"
#include "fmod_errors.h"	/* optional */
#include "SDL.h"
#include "glfont2.h"

class CGame;

#include "util/math_vectoriel.h"
#include "util/Trace.h"
#include "util/V3D.h"
#include "util/Tableau.cpp"
#include "main/Laser.h"
#include "main/Rocket.h"
#include "main/Projectil.h"
#include "main/Clavier.h"
#include "spatial/Map.h"
#include "son/DemonSons.h"
#include "son/ReqSon.h"
#include "util/Erreur.h"
#include "spatial/materiau/Textures.h"
#include "main/Cfg.h"
#include "main/Game.h"
#include "reseau/SPA.h"

using namespace JktSon;
using namespace JktUtils;

extern CCfg Config;
extern CDemonSons *DemonSons;
extern CGame Game;

#include "main/Player.h"

#define DISTANCE_INTER_JOUEURS_ENTRY_POINT 0.8f
const float Pi = 3.14159265f;

int CPlayer::m_TexArmes = -1;

CPlayer::CPlayer()
{
	m_ArmeActif = 0;		// Pas d'arme active
	m_NbrArmes = 3;			// Nombre d'armes

	m_Position[0] = 0.0f;		// Position d'origine
	m_Position[1] = 0.0f;
	m_Position[2] = 0.0f;

	m_Vitesse[0] = 0.0f;		// Vitesse nulle
	m_Vitesse[1] = 0.0f;
	m_Vitesse[2] = 0.0f;

	m_Teta = -90.0f;			// Orientation d'origine
	m_Phi = 0.0;

	m_PosVue[0] = 0.0f;		// Position du point de vue par rapport au joueur
	m_PosVue[1] = 0.1f;
	m_PosVue[2] = 0.137f;

	m_TetaVue = 0.0f;			// Orientation du point de vue par rapport au joueur
	m_PhiVue = 0.0f;

	m_pClavier = 0;
	actionFunc = 0;			// Pas d'action périodique à réaliser
	contactFunc = 0;		// Pas d'action à réaliser lors d'un contact avec la map

	m_pSkin = 0;				// Pas de skin associé par défaut

	ID_Cri = 0;				// Le cri du joueur n'a pas encore été chargé
	ID_ReqCri = 0;			// Y'a pas encore la requête sur le cri non plus

	createClavier();		// Crée la classe qui gère les requêtes de mouvement, tir ...

	if( m_TexArmes==-1 )
	{
		try
		{
			m_TexArmes = JktMoteur::LitFichierTextureAlpha( "@Icone/Armes.bmp", 0.75f );
			TRACE().p( TRACE_OTHER, "CPlayer::CPlayer() Texture d'icone des armes : %d%T", m_TexArmes, this );
		}
		catch( CErreur erreur )
		{
			TRACE().p( TRACE_ERROR, "CPlayer::CPlayer() Echec de lecture de texture d'icone des armes%T", this );
			m_TexArmes = -1;
		}
	}
}

void CPlayer::AfficheIconesArmes()
{
	if( m_TexArmes!=-1 )
	{
		float X = (float)Config.Display.X;
		float Y = (float)Config.Display.Y/2;

			// Affichage des icones des armes
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
		glBindTexture( GL_TEXTURE_2D, m_TexArmes );
		glDepthMask( GL_FALSE );
		glEnable( GL_BLEND );
		glEnable( GL_TEXTURE_2D );
		glDisable( GL_DEPTH_TEST );

		glBegin( GL_QUADS );
			glTexCoord2f( 1.0f, 0.0f );
			glVertex2f( X-50.0f,	Y+(m_NbrArmes*50/2)	);

			glTexCoord2f( 1.0f, 1.0f );
			glVertex2f( X,			Y+(m_NbrArmes*50/2)	);

			glTexCoord2f( 0.0f, 1.0f );
			glVertex2f( X,			Y-(m_NbrArmes*50/2)	);

			glTexCoord2f( 0.0f, 0.0f );
			glVertex2f( X-50.0f,	Y-(m_NbrArmes*50/2)	);
		glEnd();

			// Affichage du focus sur l'arme active
		glDisable( GL_TEXTURE_2D );
		glDepthMask( GL_TRUE );
		glDisable( GL_BLEND );

		glLineWidth( 4 );
		glColor3f( 1.0f, 0.0f, 0.0f );

		glBegin( GL_LINE_LOOP );
			glVertex2f( X-50.0f,	Y/2 - (m_NbrArmes*50/2) + 50	+ m_ArmeActif*50.0f	);
			glVertex2f( X,			Y/2 - (m_NbrArmes*50/2) + 50	+ m_ArmeActif*50.0f	);
			glVertex2f( X,			Y/2 - (m_NbrArmes*50/2)	+ m_ArmeActif*50.0f	);
			glVertex2f( X-50.0f,	Y/2 - (m_NbrArmes*50/2)	+ m_ArmeActif*50.0f	);
		glEnd();
	}
}

void CPlayer::ActiveArmeUp()	// Rends l'arme suivante active
{
	m_ArmeActif++;
	if( m_ArmeActif >= m_NbrArmes )
		m_ArmeActif = 0;
}

void CPlayer::ActiveArmeDown()	// Rends l'arme précédente active
{
	m_ArmeActif--;
	if( m_ArmeActif < 0 )
		m_ArmeActif = m_NbrArmes-1;
}

int CPlayer::ArmeActif()
{
	return m_ArmeActif;
}

CPlayer::~CPlayer()
{
	if( m_pClavier )
	{
		delete m_pClavier;
		m_pClavier = 0;
	}

	if( ID_Cri )	// Destruction du cri du personnage
	{
		DemonSons->Delete( ID_Cri );
		ID_ReqCri = 0;
		ID_Cri = 0;
	}

	if( m_pSkin )
	{
		delete m_pSkin;
		m_pSkin = 0;
	}
}

void CPlayer::setCri( const char *nomFichier )
{
	ID_Cri = DemonSons->CreateSon( nomFichier );
	ID_ReqCri = DemonSons->PlayID( ID_Cri, true );
}

CClavier *CPlayer::getClavier()
{	return m_pClavier;	}

void CPlayer::setPosition( float x, float y, float z ) //change la position du joueur
{
	m_Position[0] = x;
	m_Position[1] = y;
	m_Position[2] = z;
}

void CPlayer::choiceOneEntryPoint()
{
	if( Game.getMap()->m_EntreeJoueurs.size() >= 1 )
	{
		CPlayer *player;
		bool valide;
		float pos[3];
		int curseur;
		CV3D distance;
		vector< vector<CV3D>::iterator > liste;
		vector<CV3D>::iterator iterEntry;
		vector<CPlayer*>::iterator iterPlayer;

			// Fait la liste des points d'entrée se trouvant à une distance supérieure
			// à DISTANCE_INTER_JOUEURS_ENTRY_POINT de tout autre joueur
		for( iterEntry=Game.getMap()->m_EntreeJoueurs.begin() ; iterEntry!=Game.getMap()->m_EntreeJoueurs.end() ; iterEntry++ )
		{
			curseur = -1;
			valide = false;

			while( Game.pTabIndexPlayer->bSuivant( curseur ) )
			{
				player = Game.pTabIndexPlayer->operator [](curseur);

				if( player != this )	// Si le joueur en question n'est pas le joueur actuel
				{
					distance = (*iterEntry);
					player->getPosition( pos );
					distance -= pos;
					if( distance.norme() < DISTANCE_INTER_JOUEURS_ENTRY_POINT )
					{
						valide = false;		// Le point d'entrée est trop proche d'un des joueurs
						break;				// On passe au point d'entrée suivant
					}
					else
					{
						valide = true;
					}
				}
			}

			if( valide )	// Si le point d'entrée suffisament distant de tout joueur, souviens-t'en
				liste.push_back( iterEntry );
		}

		int choice;
		size_t nbr;
		if( nbr = liste.size() )	// S'il y a des entrées à une distance convenable de tout joueur
		{							// Alors choisi l'une d'elles au hasard
			choice = (int)(nbr * ((float)rand()/(float)RAND_MAX));
			if( choice == nbr )
				choice--;
			setPosition( *(liste[choice]) );
		}
		else
		{							// Sinon prends en une au hasard dans la liste
			cout << "\nAu pif";
			nbr = Game.getMap()->m_EntreeJoueurs.size();
			choice = (int)(nbr * ((float)rand()/(float)RAND_MAX));
			if( choice == nbr )
				choice--;
			setPosition( Game.getMap()->m_EntreeJoueurs[choice] );
		}
	}
	else
	{
		CV3D pos( 1.0, 0.5, 0.0 );
		setPosition( pos );
	}
}

void CPlayer::setPosition( const CV3D& pos ) //change la position du joueur
{
	m_Position[0] = pos.X;
	m_Position[1] = pos.Y;
	m_Position[2] = pos.Z;
}

void CPlayer::setPosition( const float pos[3] ) //change la position du joueur
{
	m_Position[0] = pos[0];
	m_Position[1] = pos[1];
	m_Position[2] = pos[2];
}

void CPlayer::getPosition( float pos[3] ) const	//retourne le pointeur sur la position du joueur
{
	pos[0] = m_Position[0];
	pos[1] = m_Position[1];
	pos[2] = m_Position[2];
}

void CPlayer::changeVitesse( float vx, float vy, float vz ) //change la vitesse du joueur
{
	m_Vitesse[0] = vx;
	m_Vitesse[1] = vy;
	m_Vitesse[2] = vz;
}

void CPlayer::getVitesse( float vit[3] ) const	//retourne le pointeur sur la vitesse du joueur
{
	vit[0] = m_Vitesse[0];
	vit[1] = m_Vitesse[1];
	vit[2] = m_Vitesse[2];
}

void CPlayer::setVitesse( const float vit[3] )
{
	m_Vitesse[0] = vit[0];
	m_Vitesse[1] = vit[1];
	m_Vitesse[2] = vit[2];
}

void CPlayer::changeAction(void (*action)(CPlayer *player))
{
	actionFunc = action;	//définit l'action périodique à réaliser
}

void CPlayer::changeContact(void (*contact)(CPlayer *player, float *normal, float distanceW))
{
	contactFunc = contact;	//définit l'action à réaliser lors d'un contact avec la map
}

void CPlayer::Affiche()
{
	glPushMatrix();
	glTranslatef( m_Position[0], m_Position[1], -m_Position[2] );
	glRotated( 90.0f, 0.0f, 1.0f, 0.0f );
	glRotated( -m_Teta, 0.0f, 1.0f, 0.0f ); //Rotation par rapport à l'axe verticale

	if( m_pSkin )
	{
		m_pSkin->Affiche();

		const float taille = 0.01f;

		glBegin(GL_QUADS);
			glColor3f( 1.0, 0.0, 0.0);
			glVertex3f( -taille, -taille, -taille );
			glVertex3f( taille, -taille, -taille );
			glVertex3f( taille, taille, -taille );
			glVertex3f( -taille, taille, -taille );

			glColor3f( 0.0, 1.0, 0.0);
			glVertex3f( -taille, -taille, taille );
			glVertex3f( taille, -taille, taille );
			glVertex3f( taille, taille, taille );
			glVertex3f( -taille, taille, taille );

			glColor3f( 0.0, 0.0, 1.0);
			glVertex3f( -taille, -taille, -taille );
			glVertex3f( -taille, taille, -taille );
			glVertex3f( -taille, taille, taille );
			glVertex3f( -taille, -taille, taille );

			glColor3f( 0.0, 0.5, 0.5);
			glVertex3f( -taille, -taille, -taille );
			glVertex3f( taille, -taille, -taille );
			glVertex3f( taille, -taille, taille );
			glVertex3f( -taille, -taille, taille );

			glColor3f( 0.4f, 0.9f, 0.6f);
			glVertex3f( -taille, taille, -taille );
			glVertex3f( taille, taille, -taille );
			glVertex3f( taille, taille, taille );
			glVertex3f( -taille, taille, taille );
		glEnd();
	}

	glPopMatrix();
}

void CPlayer::tuer()
{
	if( !ID_ReqCri->IsPlaying() )
		DemonSons->Play( ID_ReqCri );	// (Re)Joue le cri
}

void CPlayer::AfficheProjectils()	// Affiche tous les projectils
{
	CProjectil *pro;

	Tableau<CProjectil>::Adr *adr = TabProjectil.BeginAdr();
	while( adr ) //tant que ce n'est pas le dernier objet géométrique de la liste
	{
		pro = adr->m_adrX;
		pro->Affiche();			// Affichage de l'objet géo
		adr = TabProjectil.Suivant( adr );	//Passe à l'objet géométrique suivant
	}
}

void CPlayer::Tir()
{
	CV3D Dir;

	if( ArmeActif()==0 )	// Si l'arme active de erwin est le rocket
	{}
	else if( ArmeActif()==1 )	// Si l'arme active de erwin est le rocket
	{
		TabProjectil.Ajoute( new CLaser( this ) );
	}
	else if( ArmeActif()==2 )	// Si l'arme active de erwin est le rocket
	{
		TabProjectil.Ajoute( new CRocket( this ) );
	}
}

void CPlayer::RefreshProjectils()
{
	CProjectil *pro;
	Tableau<CProjectil>::Adr *adrSuivant;

	Tableau<CProjectil>::Adr *adr = TabProjectil.BeginAdr();
	while( adr ) //tant que ce n'est pas le dernier objet géométrique de la liste
	{
		pro = adr->m_adrX;

		if( !pro->Refresh() )			// Affichage de l'objet géo
		{
				// L'objet 'pro' a signalé la fin de sa vie, il doit être détruit
			adrSuivant = TabProjectil.Suivant( adr );	//Passe à l'objet géométrique suivant

			TabProjectil.Enleve( adr );
			delete pro;
		}
		else
		{
				// L'objet 'pro' a signalé qu'il reste vivant
			adrSuivant = TabProjectil.Suivant( adr );	//Passe à l'objet géométrique suivant
		}

		adr = adrSuivant;
	}
}

void CPlayer::init()
{
}

void CPlayer::createClavier()
{	m_pClavier = new CClavier();	}

void CPlayer::exeActionFunc()	// Exécute l'action périodique associée au joueur
{	actionFunc( this );		}

void CPlayer::exeContactFunc( float *normal, float distanceW)	// Exécute fonction gestion contacts avec joueur
{	contactFunc( this, normal, distanceW );	}

float CPlayer::Phi() const
{
	return m_Phi;
}

float CPlayer::Teta() const
{
	return m_Teta;
}

float CPlayer::PhiVue() const
{
	return m_PhiVue;
}

float CPlayer::TetaVue() const
{
	return m_TetaVue;
}

void CPlayer::getPosVue( float vect[3] ) const
{
	vect[0] = m_PosVue[0];
	vect[1] = m_PosVue[1];
	vect[2] = m_PosVue[2];
}

void CPlayer::TetaVue( float tetaVue )
{
	m_TetaVue = tetaVue;
}

void CPlayer::PhiVue( float phiVue )
{
	m_PhiVue = phiVue;
}

void CPlayer::setPosVue( const float posVue[3] )
{
	m_PosVue[0] = posVue[0];
	m_PosVue[1] = posVue[1];
	m_PosVue[2] = posVue[2];
}

void CPlayer::Phi( float phi )
{
	m_Phi = phi;
}

void CPlayer::Teta( float teta )
{
	m_Teta = teta;
}

float CPlayer::Pente() const
{
	return m_Pente;
}

void CPlayer::Pente( float pente )
{
	m_Pente = pente;
}

void CPlayer::nom( const string &nom )
{
	m_Nom = nom;
}

string CPlayer::nom() const
{
	return m_Nom;
}

void CPlayer::Skin( JktMoteur::CMap *skin )
{
	m_pSkin = skin;
}

void CPlayer::deplace()
{
	const float quantumVitesse = 0.003f;
	float norm;
	float vect[3];

	if( fabsf(Pente())>0.707f )	// Si la pente est inférieure à 45°
	{
		if( (fabsf(getClavier()->m_fDroite)<quantumVitesse/(5.f)) &&
				(fabsf(getClavier()->m_fAvance)<quantumVitesse/(5.f)) )
				// S'il n'y a pas de requête de déplacement=>ralenti le joueur
		{
			getVitesse( vect );

			if( norme( vect )>quantumVitesse/2 )
			{
				float var = 1 - ( quantumVitesse/(2*norme( vect )) );

				vect[0] = vect[0]*var;
				vect[1] = vect[1]*var;
				vect[2] = vect[2]*var;

				setVitesse( vect );
			}
			else
			{
				vect[0] = 0;
				vect[1] = 0;
				vect[2] = 0;
				setVitesse( vect );
			}
		}
	}

	float position[3], vitesse[3];
	getPosition( position );
	getVitesse( vitesse );
	position[0] += vitesse[0];//incrémente la position du joueur
	position[1] += vitesse[1];//de sa vitesse
	position[2] += vitesse[2];
	setPosition( position );


	if( fabsf(Pente()) > 0.707f )	//Si la pente est inférieure à 45°=>limite la vitesse
	{
		norm = norme( vitesse );

		if( norm > 10.0*quantumVitesse )	//Limitation de la norme de la vitesse du joueur
		{									//à 10 fois quantumVitesse
			normalise( vitesse );
			vitesse[0] = vitesse[0]*10.0f*quantumVitesse;
			vitesse[1] = vitesse[1]*10.0f*quantumVitesse;
			vitesse[2] = vitesse[2]*10.0f*quantumVitesse;

			setVitesse( vitesse );
		}
	}
}

void CPlayer::faitRequeteClavier()
{
	const float quantumVitesse = 0.003f;
	float cosTeta = /*FastCos0(erwin->Teta/180.0f*Pi);*/	cosf(m_Teta/180.0f*Pi);
	float sinTeta = /*FastSin0(erwin->Teta/180.0f*Pi);*/	sinf(m_Teta/180.0f*Pi);
	float vect[3];

	getVitesse( vect );

	vect[1] += quantumVitesse*m_pClavier->m_fMonte;

	vect[0] += cosTeta*quantumVitesse*m_pClavier->m_fDroite;
	vect[2] -= sinTeta*quantumVitesse*m_pClavier->m_fDroite;

	vect[0] += sinTeta*quantumVitesse*m_pClavier->m_fAvance;
	vect[2] += cosTeta*quantumVitesse*m_pClavier->m_fAvance;

	setVitesse( vect );

	m_pClavier->reset();	// Réinitialise les requêtes clavier du joueur
}
