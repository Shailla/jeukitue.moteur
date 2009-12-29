

/*//////////////////////////////////////////////////////////////////////////////////////////////
					CHOSES A FAIRE

Les fonctions de callback des players (gravité, contact, ...)ne sont pas
implémentées proprement, il ne faudrait pas avoir besoin de leur fournir le pointeur
sur le player (mais j'sais pas comment faire)

Formaliser les vecteurs position et vitesse de manière à ce qu'il ne soit plus
indispensable d'inverser parfois certaines de leurs composantes selon l'utilisation

/*///////////////////////////////////////////////////////////////////////////////////////////////

// à virer
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>

#ifdef WIN32
	#include <windows.h>
	#include <io.h>
	#include <direct.h>
#endif
#include <agar/config/have_opengl.h>
#include <agar/core.h>
#include <agar/gui.h>
#include <GL/gl.h>
#include <GL/glu.h>

using namespace std;

#define Pi				3.14159265f			//nombre pi pour les calculs
#define quantumVitesse	0.003f				//pas d'augmentation de la vitesse du joueur (accélération discrète)
#define DELAY_TIMER		10					// Temps entre 2 calculs de scènes (en ms)
#define TAILLEFONT		0.7f				// Taille des fontes

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_net.h"
#include "fmod.h"
#include "fmod_errors.h"	/* optional */
#include "glfont2.h"
#include "tinyxml.h"
#include "util/XmlHelper.h"
using namespace glfont;

#include "util/GenRef.h"
#include "ressource/RessourcesLoader.h"
#include "util/math_vectoriel.h"		//Fonctions de calculs vectoriels
#include "util/V3D.h"
#include "util/mathFast.h"
#include "util/Trace.h"				// Gestion du fichier de traces
#include "util/Tableau.cpp"			//Liste chaînée
#include "util/TableauIndex.cpp"		//Liste Indexée
#include "util/FindFolder.h"
#include "util/Erreur.h"

class CGame;

#include "Cfg.h"
#include "Material.h"
#include "IfstreamMap.h"
#include "Geo.h"				//Paramètres des objets géométriques
#include "Mouve.h"
#include "GeoObject.h"
#include "Rocket.h"
#include "Laser.h"
#include "Porte.h"
#include "Navette.h"
#include "Clavier.h"		//Requêtes du clavier
#include "Photo.h"
#include "MaterialTexture.h"	//Paramètres des matériaux associés aux objets géométriques
#include "MaterialMulti.h"
#include "Light.h"			//Lumières de la map
#include "Map.h"
#include "son/DemonSons.h"
#include "son/Son.h"
#include "son/ReqSon.h"
#include "reseau/SPA.h"
#include "Player.h"			//Paramètres du joueur
#include "contact.h"		//Fonctions utilisées pour le contact
#include "divers.h"		//Initialisation d'SDL
#include "son/audio.h"
#include "DlgBoite.h"
#include "Portail.h"
#include "Menu.h"			// Fonctions de gestion du menu
#include "Focus.h"			// Gestion du focus
#include "Textures.h"
#include "util/TableauIndex.h"
#include "reseau/Client.h"
#include "reseau/Server.h"
#include "RequeteProcess.h"
#include "Game.h"
#include "util/V3D.h"
#include "Particule.h"
#include "MoteurParticules.h"
#include "reseau/Reseau.h"
#include "reseau/enumReseau.h"
#include "menu/Controller.h"
#include "menu/Viewer.h"
#include "Fabrique.h"

using namespace JKT_PACKAGE_MENU;
using namespace JKT_PACKAGE_RESEAU;
using namespace JKT_PACKAGE_MOTEUR3D;
using namespace JKT_PACKAGE_UTILS;
using namespace JKT_PACKAGE_SON;

#include "util/GenRef.h"

#include "jkt.h"

extern SDL_Surface *screen;

float GLIGHTX, GLIGHTY, GLIGHTZ;
GLFont myfont;
extern AG_Window *agarWindows;

class CMachin
{
public:
	CMachin();

	CV3D position;
	CV3D vitesse;
	CReqSon *req_son;
};

CMachin::CMachin()
{
	position.X = 2.0f;
	position.Y = 0.2f;
	position.Z = 0.0f;
	vitesse.X = 0.0f;
	vitesse.Y = 0.0f;
	vitesse.Z = 0.05f;
}

float delta = 0.0;

extern CFocus *pFocus;				// Gestion du focus
extern bool Aide;					// Indique que le menu d'aide est actif
Uint32 ecart,ecartTimer,ecartDisplay;

Uint32 ceTemps;
ofstream tempsFile("Temps.txt");

void initTemps() {
	ceTemps = SDL_GetTicks();
}

void setTemps(const string& note) {
	ecartDisplay = SDL_GetTicks() - ceTemps;
	tempsFile << endl << ecartDisplay << "\t" << note;
}

JKT_PACKAGE_MOTEUR3D::CMoteurParticules *moteurParticules;
float tempsMoteur;	// Pour le moteur de particules
unsigned int frpsTimer = 0, frpTimer = 0;

CMachin *machin;	// Pour tester le son 3D

CReseau Reseau;

void gravitePlayer(CPlayer *player)	// Fonction implémentant la gravité
{									// aux objets qui doivent la subire
	float vect[3];
	player->getVitesse( vect );

	vect[1] -= 0.5*quantumVitesse; // Ajout de gravité au joueur

	player->setVitesse( vect );
}

void init( void )		//INITIALISATION DU JEU
{
	pFocus = new CFocus(play_handle_key_down,
						CDlg::menu_handle_key_down,
						menu_agar_handle_key_down);
	pFocus->SetPlayFocus();
	Aide = false;
}

void updateSon3D()
{
	// TEST DU SON 3D
//	machin->vitesse.Z = 0.0f;
	if( machin->position.Z>1.5f )
		machin->vitesse.Z = -0.05f;
	else if( machin->position.Z<-1.5f )
		machin->vitesse.Z = 0.05f;

	machin->position += machin->vitesse;

	CPlayer *erwin = Game.Erwin();

	float cosP = /*FastCos0( erwin->Phi/180.0f*Pi );*/	cosf( erwin->Phi()/180.0f*Pi );
	float sinP = /*FastSin0( erwin->Phi/180.0f*Pi );*/	sinf( erwin->Phi()/180.0f*Pi );
	float cosT = /*FastCos0( erwin->Teta/180.0f*Pi );*/	cosf( erwin->Teta()/180.0f*Pi );
	float sinT = /*FastSin0( erwin->Teta/180.0f*Pi );*/	sinf( erwin->Teta()/180.0f*Pi );

	float fx = cosP*sinT;
	float fy = -sinP;
	float fz = -cosT*cosP;
	float tx = -fabsf(sinP)*sinT;
	float ty = cosP;
	float tz = fabsf(sinP)*cosT;

	float pos_erwin[3];
	erwin->getPosition( pos_erwin );
	pos_erwin[2] = -pos_erwin[2];

	float pos_machin[3];
	pos_machin[0] = machin->position.X;
	pos_machin[1] = machin->position.Y;
	pos_machin[2] = machin->position.Z;

		// Position du point d'écoute (le joueur)
	FSOUND_3D_Listener_SetAttributes(pos_erwin, 0, fx,fy,fz,tx,ty,tz);

		// Position de l'émetteur du son
	((CReqSon3D*)machin->req_son)->SetPosition( pos_machin );
	FSOUND_Update();

	glPushMatrix();
	glTranslatef( machin->position.X, machin->position.Y, machin->position.Z );
	glColor3f( 1.0f, 1.0f, 0.0f );
	gluSphere( gluNewQuadric(), 0.05, 16, 16 );
	glPopMatrix();
}

void afficheInfo( Uint32 tempsDisplay )
{
	myfont.Begin();

		// Affiche le mode de jeu (rien, local, client ou serveur)
	string str;
	string str2;

	str = "Jeu en mode : ";
	if( Game.isModeNull() )
	{
		str += "NULL";
		str2 = "";
	}
	else if( Game.isModeLocal() )
	{	str += "LOCAL";
		str2 = "";
	}
	else if( Game.isModeClient() )
	{
		str += "CLIENT";
		switch( Game.getStatutClient() )
		{
		case JKT_STATUT_CLIENT_INIT:
			str2 = "Etat : Initialisation en cours";
			break;
		case JKT_STATUT_CLIENT_READY:
			str2 = "Etat : Client ready";
			break;
		case JKT_STATUT_CLIENT_DEMJTG:
			str2 = "Etat : Demande de JTG envoyée";
			break;
		case JKT_STATUT_CLIENT_OUV:
			str2 = "Etat : Ouverture de la MAP en cours";
			break;
		case JKT_STATUT_CLIENT_PLAY:
			str2 = "Etat : Jeu en cours";
		default:
			break;
		}
	}
	else if( Game.isModeServer() )
	{
		str += "SERVEUR";
		switch( Game.getStatutServer() )
		{
		case JKT_STATUT_SERVER_INIT:
			str2 = "Etat : Initialisation en cours";
			break;
		case JKT_STATUT_SERVER_READY:
			str2 = "Etat : Serveur ready";
			break;
		case JKT_STATUT_SERVER_PLAY:
			str2 = "Etat : Partie en cours";
			break;
		default:
			break;
		}
	}

	myfont.DrawString( str, TAILLEFONT, 20.0f, ((float)Config.Display.Y) - 20.0f );
	myfont.DrawString( str2, TAILLEFONT, 20.0f, ((float)Config.Display.Y) - 35.0f );

	int pos = 0;
		// Affiche les temps total, de calcul et d'affichage
	char cou[70];
	sprintf( cou, "Ecarts : total=%.6u ms, timer=%.6u ms, display=%.6u ms", ecart, ecartTimer, ecartDisplay );
	str = cou;
	myfont.DrawString( str, TAILLEFONT, 20.0f, pos++*15.0f+20.0f );


		// Affiche le mode du jeu
	if( Game.getMap() && Game.getMap()->IsSelectionMode() )
	{
		sprintf( cou, "Selection : %s", Game.getMap()->getSelectedName());
		str = cou;
		myfont.DrawString( cou, TAILLEFONT, 20.0f, pos++*15.0f+20.0f );
	}
		// Affichage pour le debbugage du tir laser
	sprintf( cou, "Distance : %0.6f", delta );
	str = cou;
	myfont.DrawString( str, TAILLEFONT, 20.0f, pos++*15.0f+20.0f );

	if( Game.Erwin() )
	{
		CPlayer *erwin = Game.Erwin();

			// Affiche le Teta du joueur principal
		sprintf( cou, "Tete : %.5d", (int)erwin->Teta() );
		str = cou;
		myfont.DrawString( str, TAILLEFONT, 20.0f, pos++*15.0f+20.0f );

			// Affiche le Phi du joueur principal
		sprintf( cou, "Phi : %.5d", (int)erwin->Phi() );
		str = cou;
		myfont.DrawString( str, TAILLEFONT, 20.0f, pos++*15.0f+20.0f );

				// Affiche la position du joueur principal
		float position[3];
		erwin->getPosition( position );

		sprintf( cou, "Position : %0.4f %0.4f %0.4f", position[0], position[1], position[2] );
		str = cou;
		myfont.DrawString( str, TAILLEFONT, 20.0f, pos++*15.0f+20.0f );
	}

	if( Config.Debug.bSonPerformances )
	{
		unsigned int currentalloced, maxalloced;
		sprintf( cou, "Son, usage CPU : %.4f %%", FSOUND_GetCPUUsage() );
		str = cou;
		myfont.DrawString( str, TAILLEFONT, 20.0f, pos++*15.0f+20.0f );

		FSOUND_GetMemoryStats( &currentalloced, &maxalloced );

		sprintf( cou, "Son, memory allocated : %.5u ko", currentalloced/1024 );
		str = cou;
		myfont.DrawString( str, TAILLEFONT, 20.0f, pos++*15.0f+20.0f );

		sprintf( cou, "Son, max memory allocated : %.5u ko", maxalloced/1024 );
		str = cou;
		myfont.DrawString( str, TAILLEFONT, 20.0f, pos++*15.0f+20.0f );
	}

	if( Config.Debug.bReseauDebit )
	{
		float debit, taille;

		CSPA::getDebitRec( debit, taille );

		sprintf( cou, "Debit en reception : %.5f ko/s (%.5f octets)", debit, taille );
		str = cou;
		myfont.DrawString( str, TAILLEFONT, 20.0f, pos++*15.0f+20.0f );

		CSPA::getDebitEm( debit, taille );

		sprintf( cou, "Debit en emmission : %.5f ko/s (%f octets)",
		debit, taille );
		str = cou;
		myfont.DrawString( str, TAILLEFONT, 20.0f, pos++*15.0f+20.0f );
	}

	if( Config.Debug.bSonSpectre )
	{
		int NBR_RAIES_SPECTRE = 512;
		float *spectre = FSOUND_DSP_GetSpectrum();

		glDisable( GL_TEXTURE_2D );
		glDepthMask( GL_FALSE );
		glDisable( GL_DEPTH_TEST );
		glEnable( GL_BLEND );

		float gauche = (float)Config.Display.X - 130;
		float bas = (float)10;
		float largeur = 120;
		float hauteur = 120;
		float haut = bas + hauteur;
		float droite = gauche + largeur;

		glBegin( GL_QUADS );
			glColor4f( 1.0f, 1.0f, 0.0f, 0.35f );
			glVertex2f( gauche, bas );
			glVertex2f( gauche, haut );
			glVertex2f( droite, haut );
			glVertex2f( droite, bas );

		glDisable( GL_BLEND );

		for( int i=0 ; i < NBR_RAIES_SPECTRE ; i++ )
		{
			glColor3f( 0.0f, 1.0f, 0.0f );
			glVertex2f( gauche + i*(largeur/(float)NBR_RAIES_SPECTRE), bas );
			glVertex2f( gauche + (i+1)*(largeur/(float)NBR_RAIES_SPECTRE), bas );

			glColor3f( 1.0f, 0.0f, 0.0f );
			glVertex2f( gauche + (i+1)*(largeur/(float)NBR_RAIES_SPECTRE), bas + hauteur*spectre[i] );
			glVertex2f( gauche + i*(largeur/(float)NBR_RAIES_SPECTRE), bas + hauteur*spectre[i] );
		}

		glEnd();

		glEnable( GL_TEXTURE_2D );
		glDepthMask( GL_TRUE );
		glEnable( GL_DEPTH_TEST );
	}
}

void display()	// Fonction principale d'affichage
{
	Uint32 temps = SDL_GetTicks();	// Temps au début du réaffichage

		// AFFICHAGE DES ELEMENTS 3D
	glMatrixMode( GL_PROJECTION );	// Mise en place de la perspective joueur, de la profondeur, ...
    glLoadIdentity();
	gluPerspective( 60.0, Config.Display.X/Config.Display.Y, 0.01f, 100.0f );
	glEnable( GL_DEPTH_TEST );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// C PA LE K

	if( Game.getMap() )	// Si il y a une map a afficher
	{
		if( Game.Erwin() )	// S'il y a un joueur principal
		{
			float vect[3];
			CPlayer *erwin = Game.Erwin();

			erwin->getPosVue( vect );
			glTranslatef( -vect[0], -vect[1], vect[2] );	// Placement du point de vue
			glRotated( erwin->PhiVue(), 1.0, 0.0, 0.0 );	// Rotation par rapport au plan horizontal

			glRotated( erwin->Phi(), 1.0, 0.0, 0.0 );	// Rotation par rapport au plan horizontal
			glRotated( erwin->Teta(), 0.0,1.0, 0.0 );	// Rotation par rapport à l'axe verticale

			erwin->getPosition( vect );
			glTranslatef( -vect[0], -vect[1], vect[2] );	// Placement du point de vue

			glRotated( erwin->TetaVue(), 0.0,1.0, 0.0 );	// Rotation par rapport à l'axe verticale
		}

		Game.AffichePlayers();		// Affiche tous les joueurs

		// C PA LE K

			// Essai lumière
//		float light_position[] = { 0.6f, -0.1f, 0.0f, 1.0f };

//		float colorBlanc[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//		float colorRouge[] = { 1.0f, 0.0f, 0.0f, 1.0f };
//		float colorVert[] = { 0.0f, 1.0f, 0.0f, 1.0f };
//		float colorBleu[] = { 0.0f, 0.0f, 1.0f, 1.0f };
//		float colorNoir[] = { 0.0f, 0.0f, 0.0f, 1.0f };

/*		glPushMatrix();
		merde += 0.01f;
		if( merde > 1.0f )
			merde = 0.0f;
		glTranslatef( light_position[0], light_position[1]- merde, light_position[2] );
		glColor3f( 1.0f, 0.0f, 1.0f );
		gluSphere( gluNewQuadric(), 0.05, 64, 64 );
		glEnable( GL_LIGHTING );
		glEnable( GL_LIGHT0 );
		glLightfv( GL_LIGHT0, GL_POSITION, colorNoir );
		glLightfv( GL_LIGHT0, GL_AMBIENT, colorNoir );
		glLightfv( GL_LIGHT0, GL_DIFFUSE, colorNoir );
		glLightfv( GL_LIGHT0, GL_SPECULAR, colorBleu );
		glPopMatrix();


		glPushMatrix();
		glTranslatef( 0.0f, -0.4f, 0.0f );

		glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, colorNoir );
		glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, colorNoir );
		glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, colorBlanc );
		glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 50.0f );

		gluSphere( gluNewQuadric(), 0.15, 16, 16 );
		glPopMatrix();

		glDisable( GL_LIGHT0 );
		glDisable( GL_LIGHTING );
*/
		glEnable( GL_DEPTH_TEST );

		// C PA LE K
		Game.getMap()->Affiche();	// Affichage de la map
		// C LE K
		Game.AfficheProjectils();

			// Dessine les axes dans la map	(sert au repérage pour la conception du jeu)
		glLineWidth( 3 );
		glBegin( GL_LINES );	// Déssine les axes X Y Z
			glColor3f( 1.0f, 0.0f, 0.0f);	// Axe des X
			glVertex3f( 1.0f, 0.0f, 0.0f);
			glVertex3f( 0.0f, 0.0f, 0.0f);
			glColor3f( 0.0f, 1.0f, 0.0f);	// Axe des Y
			glVertex3f( 0.0f, 0.0f, 0.0f);
			glVertex3f( 0.0f, 1.0f, 0.0f);
			glColor3f( 0.0f, 0.0f, 1.0f);	// Axe des Z
			glVertex3f( 0.0f, 0.0f, 0.0f);
			glVertex3f( 0.0f, 0.0f, 1.0f);
		glEnd();

		glColor3f( 1.0f, 0.2f, 0.2f );
		gluSphere( gluNewQuadric(), 0.05, 16, 16 );

		// C LE K

		if( Game.Erwin() )
			updateSon3D();	// Positionne le joueur et les objets bruyants dans l'espace sonore

			// AFFICHAGE DES PARTICULES
		glEnable( GL_BLEND );
		glDepthMask( GL_FALSE );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE );

		// C LE K

		moteurParticules->Affiche();	// Le moteur de particules affiche toutes ses particules

		glDisable( GL_BLEND );
		glDepthMask( GL_TRUE );
	}

	// C LE K

			// AFFICHAGE DES ELEMENTS AVANTS (TEXTE, SCORE, ...)
	glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
	gluOrtho2D(0.0, Config.Display.X, 0.0, Config.Display.Y);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glEnable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );
	glDisable( GL_DEPTH_TEST );

	if( JKT_AfficheToutesTextures )
		Game.afficheToutesTextures( Config.Display.X, Config.Display.Y );

	if( Game.Erwin() )
	{
		Game.Erwin()->AfficheIconesArmes();
		Game.afficheViseur(Config.Display.X/2, Config.Display.Y/2);
		glDepthMask( GL_TRUE );
	}

	glEnable( GL_TEXTURE_2D );
	glEnable( GL_BLEND );
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glColor3f( 1.0, 1.0, 1.0 );

	afficheInfo( SDL_GetTicks()-temps );	// Affiche les infos texte en avant plan

	if( Aide )
		CDlg::afficheMenuActif();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    Viewer* agarView = Fabrique::getAgarView();
    if(agarView->IsVisible()) {
        glEnable( GL_TEXTURE_2D );
	    glDisable( GL_BLEND );
        glDisable( GL_DEPTH_TEST );

        glOrtho(0, agView->w, agView->h, 0, -1.0, 1.0);

        agarView->draw();

        glDisable( GL_TEXTURE_2D );
	    glDisable( GL_BLEND );
	}

	// C LE K

	SDL_GL_SwapBuffers();	// Echange des buffers graphique -> affiche à l'écran
}

void chopeLesEvenements()
{
	SDL_PumpEvents();							// Collecte les évênements
	Uint8 *keystate = SDL_GetKeyState( 0 );		//Récupère les touches clavier appuyées
	Uint8 mouse = SDL_GetMouseState( 0, 0 );

	if( Game.Erwin() && (!Game.isModeServer()) )
	{
		CPlayer *erwin = Game.Erwin();

		if( keystate[SDLK_a] )
		{
			erwin->TetaVue( erwin->TetaVue() + 0.2f );
			if( erwin->TetaVue() > 180.0f )
				erwin->TetaVue( erwin->TetaVue() - 360.0f );
		}

		if( keystate[SDLK_y] )
			GLIGHTX += 0.003f;

		if( keystate[SDLK_u] )
			GLIGHTY += 0.003f;

		if( keystate[SDLK_i] )
			GLIGHTZ += 0.003f;

		if( keystate[SDLK_h] )
			GLIGHTX -= 0.003f;

		if( keystate[SDLK_j] )
			GLIGHTY -= 0.003f;

		if( keystate[SDLK_k] )
			GLIGHTZ -= 0.003f;

		if( keystate[SDLK_q] )
		{
			erwin->TetaVue( erwin->TetaVue() - 0.2f );
			if( erwin->TetaVue() < -180.0f )
				erwin->TetaVue( erwin->TetaVue() + 360.0f );
		}

		if( keystate[SDLK_w] )
		{
			erwin->PhiVue( erwin->PhiVue() + 0.2f );
			if( erwin->PhiVue() > 180.0f )
				erwin->PhiVue( erwin->PhiVue() - 360.0f );
		}

		if( keystate[SDLK_s] )
		{
			erwin->PhiVue( erwin->PhiVue() - 0.2f );
			if( erwin->PhiVue() < -180.0f )
				erwin->PhiVue( erwin->PhiVue() + 360.0f );
		}

		if( keystate[SDLK_e] )
		{
			float posVue[3];
			erwin->getPosVue( posVue );
			posVue[0] += 0.003f;
			erwin->setPosVue( posVue );
		}

		if( keystate[SDLK_d] )
		{
			float posVue[3];
			erwin->getPosVue( posVue );
			posVue[0] -= 0.003f;
			erwin->setPosVue( posVue );
		}

		if( keystate[SDLK_r] )
		{
			float posVue[3];
			erwin->getPosVue( posVue );
			posVue[1] += 0.003f;
			erwin->setPosVue( posVue );
		}

		if( keystate[SDLK_f] )
		{
			float posVue[3];
			erwin->getPosVue( posVue );
			posVue[1] -= 0.003f;
			erwin->setPosVue( posVue );
		}

		if( keystate[SDLK_t] )
		{
			float posVue[3];
			erwin->getPosVue( posVue );
			posVue[2] += 0.003f;
			erwin->setPosVue( posVue );
		}

		if( keystate[SDLK_g] )
		{
			float posVue[3];
			erwin->getPosVue( posVue );
			posVue[2] -= 0.003f;
			erwin->setPosVue( posVue );
		}

		if ( keystate[Config.Commandes.Gauche.key]||(mouse&SDL_BUTTON(Config.Commandes.Gauche.mouse)) )	//gauche
		{
			erwin->getClavier()->m_fDroite = -1.0;
			erwin->getClavier()->m_bIndic = true;
		}

		if( keystate[Config.Commandes.Reculer.key]||(mouse&SDL_BUTTON(Config.Commandes.Reculer.mouse)) )	//reculer
		{
			erwin->getClavier()->m_fAvance = -1.0;
			erwin->getClavier()->m_bIndic = true;
		}

		if( keystate[Config.Commandes.Avancer.key]||(mouse&SDL_BUTTON(Config.Commandes.Avancer.mouse)) )	//avant
		{
			erwin->getClavier()->m_fAvance = 1.0;
			erwin->getClavier()->m_bIndic = true;
		}

		if( keystate[Config.Commandes.Droite.key]||(mouse&SDL_BUTTON(Config.Commandes.Droite.mouse)) )	//droite
		{
			erwin->getClavier()->m_fDroite = 1.0;
			erwin->getClavier()->m_bIndic = true;
		}

		if( keystate[Config.Commandes.Tir1.key]||(mouse&SDL_BUTTON(Config.Commandes.Tir1.mouse)) )	// Tire au laser
		{
			//erwin->Tir();		// Valide un tir
		}

		if( keystate[Config.Commandes.Monter.key]||(mouse&SDL_BUTTON(Config.Commandes.Monter.mouse)) )	//si bouton droit souris
		{													//alors monte
			erwin->getClavier()->m_fMonte = 1.0;
			erwin->getClavier()->m_bIndic = true;
		}

		if( keystate[Config.Commandes.Tir2.key]||(mouse&SDL_BUTTON(Config.Commandes.Tir2.mouse)) )	//si bouton gauche souris
		{													//alors tire un projectile qui rebondit
			CPlayer *balle;

			float cosTeta = /*FastCos0( erwin->Teta/180.0f*Pi );	//*/cosf( erwin->Teta()/180.0f*Pi );
			float sinTeta = /*FastSin0( erwin->Teta/180.0f*Pi );	//*/sinf( erwin->Teta()/180.0f*Pi );
			float cosPhi = /*FastCos0( erwin->Phi/180.0f*Pi );	//*/cosf( erwin->Phi()/180.0f*Pi );
			float sinPhi = /*FastSin0( erwin->Phi/180.0f*Pi );	//*/sinf( erwin->Phi()/180.0f*Pi );

			balle = new CPlayer(); //crée un autre joueur
			float vect[3];
			erwin->getPosition( vect );
			balle->setPosition( vect[0], vect[1], vect[2]);		//positionne le projectile sur la map

			erwin->getVitesse( vect );
			balle->changeVitesse( sinTeta*cosPhi*10*quantumVitesse + vect[0],
				sinPhi*10*quantumVitesse + vect[1],
				cosTeta*cosPhi*10*quantumVitesse + vect[2]);

			balle->changeAction( gravitePlayer );	//associe au projectile une fonction de gravité
			balle->changeContact( contactSprite );	//associe une fonction pour les contacts avec la map

			Game.pTabIndexPlayer->Ajoute(balle);			//ajoute le projectile à la liste des joueurs
		}
	}
}

void timer(Uint32 ecart)	//focntion qui s'exécute périodiquement et qui provoque l'affichage
{
	Uint32 temps = SDL_GetTicks();	// Temps au début du timer (->mesure du temps de calcul)

	frpTimer++;

		// Réception des packets réseau
	if( Reseau.getOn() && (Game.isModeClient()) &&(
		(Game.getStatutClient()==JKT_STATUT_CLIENT_READY)	||
		(Game.getStatutClient()==JKT_STATUT_CLIENT_DEMJTG)	||
		(Game.getStatutClient()==JKT_STATUT_CLIENT_PLAY) ) )	// Si c'est un client
	{
		Reseau.recoitClient();
	}

		// Si une partie est en cours (partie locale, client ou serveur)
	if( Game.getMap() && (
		(Game.isModeLocal()) ||
		((Game.isModeClient())&&(Game.getStatutClient()==JKT_STATUT_CLIENT_PLAY)) ||
		((Game.isModeServer())&&(Game.getStatutServer()==JKT_STATUT_SERVER_PLAY)) ))
	{
		if( Reseau.getOn() && (Game.getStatutServer()==JKT_STATUT_SERVER_PLAY) )	// Si c'est un serveur
			Reseau.recoitServer();	// Recoit les données des clients
		else
			chopeLesEvenements();	// Sinon prends les requêtes du joueur les données du joueur

		if( Game.Erwin() )
		{
			if( Reseau.getOn() && (Game.getStatutClient()==JKT_STATUT_CLIENT_PLAY) )
			{
				CClient *client = Game.getClient();
				client->emet( *Game.Erwin() );		// Emet les requetes et données du joueur actif
			}
		}
			Game.timer();

			// Transmission données par le réseau
		if( Reseau.getOn() && (Game.getStatutServer()==JKT_STATUT_SERVER_PLAY) ) // Si partie serveur en cours
		{
			CServer *server = Game.getServer();
			server->emet();	// Emet toutes les données
		}
	}

	tempsTimer = SDL_GetTicks() - temps; // Temps pris par la fonction 'timer' => affichage
}

void menu_agar_handle_key_down( SDL_Event *event )
{
    switch(event->type) {
        case SDL_KEYDOWN:
		    switch( event->key.keysym.sym )
            {
    	        case SDLK_ESCAPE:
                    {
                        Viewer* agarView = Fabrique::getAgarView();
                        pFocus->SetPlayFocus();
			            agarView->SetVisible(false);
                    }
                    break;
                default:
                    AG_ProcessEvent(event);
                    break;
            }
            break;
        default:
            AG_ProcessEvent(event);
            break;
    }
}

void play_handle_key_down( SDL_Event *event )
{
//	if( pMap && erwin )
//	{
//		float cosTeta =	/*FastCos0(erwin->Teta/180.0f*Pi);		//*/cosf(erwin->Teta/180.0f*Pi);
//		float sinTeta = /*FastSin0(erwin->Teta/180.0f*Pi);		//*/sinf(erwin->Teta/180.0f*Pi);
//		float cosPhi =	/*FastCos0(erwin->Phi/180.0f*Pi);		//*/cosf(erwin->Phi/180.0f*Pi);
//		float sinPhi =	/*FastSin0(erwin->Phi/180.0f*Pi);		//*/sinf(erwin->Phi/180.0f*Pi);
//	}

	switch( event->type )
	{
    case SDL_MOUSEMOTION:		// Si c'est un évênement 'souris'
		if( Game.Erwin() )
		{
			CPlayer *erwin = Game.Erwin();
				//rotation /rapport au plan horizontal (regarder en l'air)
			erwin->Phi( erwin->Phi() + event->motion.yrel );
			if(erwin->Phi()>90.0)
				erwin->Phi( 90.0f );
			if(erwin->Phi()<-90.0)
				erwin->Phi( -90.0f );

				//rotation /rapport à l'axe verticale
			erwin->Teta( erwin->Teta() + event->motion.xrel );
			if(erwin->Teta()>180.0f)
				erwin->Teta( erwin->Teta() - 360.0f );
			if(erwin->Teta()<-180.0f)
				erwin->Teta( erwin->Teta() + 360.0f );
		}
		break;

	case SDL_MOUSEBUTTONDOWN:
		if( Game.Erwin() )
		{
			if( event->button.button==SDL_BUTTON_LEFT )
				Game.Erwin()->Tir();	// Tir avec l'arme active
			else if( event->button.button==4 )
				Game.Erwin()->ActiveArmeUp();
			else if( event->button.button==5 )
				Game.Erwin()->ActiveArmeDown();
		}
		break;

	case SDL_KEYDOWN:
		switch( event->key.keysym.sym )
		{
		case SDLK_F1 :
			pFocus->SetMenuFocus();		// Place le focus sur le menu
			lanceMenuAide( 0 );
			Aide = true;
			break;

		case SDLK_F3 :
			{
				string trace1 = "Derniere erreur FMOD : ";
				trace1 += FMOD_ErrorString(FSOUND_GetError());

				string trace2 = "Derniere erreur SDL : ";
				trace2 += SDL_GetError();

				string trace3 = "Derniere erreur SDL_Net : ";
				trace3 += SDLNet_GetError();

				string trace4 = "Derniere erreur openGL : ";
				trace4 += (char*)gluErrorString( glGetError() );

				string trace5 = "Derniere erreur Agar : ";
				trace5 += AG_GetError();

TRACE().p( TRACE_OTHER, trace1.c_str() );
TRACE().p( TRACE_OTHER, trace2.c_str() );
TRACE().p( TRACE_OTHER, trace3.c_str() );
TRACE().p( TRACE_OTHER, trace4.c_str() );
TRACE().p( TRACE_OTHER, trace5.c_str() );

				cerr << endl;
				cerr << trace1 << endl;
				cerr << trace2 << endl;
				cerr << trace3 << endl;
				cerr << trace4 << endl;
				cerr << trace5 << endl;
			}
			break;

		case SDLK_F4 :
			lanceMenuMode( 0 );
			break;

		case SDLK_g :		// Désactive / active la gravité (et annule la vitesse)
			if( Game.getMap() )
				Game.Gravite( !Game.Gravite() );
			break;

		case SDLK_v :		// Désactive / active la gravité (et annule la vitesse)
			if( Game.getMap() )
				Game.Erwin()->changeVitesse(0.0f, 0.0f, 0.0f);
			break;

		case SDLK_RETURN :	// Sauve le fichier map actuel
			if( Game.getMap() )
				Game.getMap()->Save( "save" );
			break;

		case SDLK_KP_PLUS :
			if( Game.getMap() && Game.getMap()->IsSelectionMode() )
			{
				cout << endl << "Selection du suivant";
				Game.getMap()->incrementeSelection();
			}
			break;
		case SDLK_KP_MINUS :
			if( Game.getMap() && Game.getMap()->IsSelectionMode() )
			{
				cout << endl << "Selection du precedent";
				Game.getMap()->decrementeSelection();
			}
			break;
		case SDLK_s :
			Game.getMap()->ChangeSelectionMode();
			break;

		case SDLK_p :		// Demande la prise d'une photo au prochain display
			Game.RequeteProcess.setTakePicture();
			break;

		case SDLK_m :		// Change de mode de rendu (normal ou filaire)
			if( JKT_RenderMode==GL_TRIANGLES )
				JKT_RenderMode = GL_LINE_STRIP;
			else
				JKT_RenderMode = GL_TRIANGLES;
			break;
		case SDLK_t:
			JKT_AfficheToutesTextures = !JKT_AfficheToutesTextures;
			break;
		case SDLK_o :		// Retour du joueur à la position d'origine avec vitesse nulle
			if( Game.Erwin() )
			{
				Game.Erwin()->choiceOneEntryPoint();
				Game.Erwin()->changeVitesse( 0.0f, 0.0f, 0.0f );
				Game.Erwin()->Teta( 0.0 );
				Game.Erwin()->Phi( 0.0 );
			}
			break;

		case SDLK_n:
			{
				set<CSon*>::iterator p;
				cout << endl << "Bilan des sons :";
				cout << endl << "----------------";
				int i=1;
				for( p=DemonSons->m_TabSon.begin() ; p!=DemonSons->m_TabSon.end() ; p++ )
					cout << endl << i++ << "\t" << (*p)->nom << "\t" << (unsigned int)(*p)->m_TabReq.size();
				cout << endl;
			}
			break;

		case SDLK_c :	// Change le joueur principal (=> change le point de vue et l'interraction clavier)
			cout << endl << "Nombre de joeurs dans la partie : " << Game.pTabIndexPlayer->getNbr();
			if( Game.getMap() )
			{
				nbrMainPlayer = Game.pTabIndexPlayer->Suivant( nbrMainPlayer ); //prends le nbrMainPlayer°ième joueur
				if( nbrMainPlayer>=Game.pTabIndexPlayer->getMax() )	// si on a atteint la fin de la liste
				{
					nbrMainPlayer = -1;		// Redonne la main au premier joueur de la liste
					nbrMainPlayer = Game.pTabIndexPlayer->Suivant( nbrMainPlayer );
				}
				Game.Erwin( Game.pTabIndexPlayer->operator [](nbrMainPlayer) );
			}
			break;

		case SDLK_ESCAPE:
			lanceMenuPrinc( 0 );
			break;

		case SDLK_F2:
            {
                Viewer* agarView = Fabrique::getAgarView();
                pFocus->SetMenuAgarFocus();		// Place le focus sur le menu
			    agarView->SetVisible(true);
            }
		    break;
		default:
			break;
		}
	}
}

static void process_events( void )
{
    SDL_Event event;

    while( SDL_PollEvent( &event ) )
	{
        switch( event.type )
		{
        case SDL_QUIT:
			quit_tutorial( 0 );
            break;

		default:
			pFocus->ExecFocus( &event );
			break;
        }
    }
}

bool openMAP( void *nomFichier )
{
	string nomFichierMap = (char*)nomFichier;

	if( !Game.openMap( nomFichierMap ) ) {
		cerr << endl << "Erreur à l'ouvertur du fichier Map : " << nomFichierMap;
		return false;
	}

		// Lecture du map du joueur
	CMap *pMapJoueur = new CMap( nomFichierJoueur );
	pMapJoueur->EchangeXZ();					// Ajuste les coordonnées
	pMapJoueur->Scale( -0.15f, 0.15f, 0.15f );

	CMap *pMapJoueur2 = new CMap( "GrosBonhomme" );
	pMapJoueur2->EchangeXZ();					// Ajuste les coordonnées
	pMapJoueur2->Scale( -0.80f, 0.80f, 0.80f );

	cout << endl;

		// Création joueurs
	Game.setPlayerList( 10 );	// Indique que la partie peut contenir jusqu'à 10 joueurs

	// Récupération des ressources de cris des personnages
	string cri1 = "@Bruit\\cri_1.wav";		// Chargement de la fonte de caractères
	JKT_PACKAGE_UTILS::RessourcesLoader::getFileRessource(cri1);

	string cri2 = "@Bruit\\cri_1.wav";		// Chargement de la fonte de caractères
	JKT_PACKAGE_UTILS::RessourcesLoader::getFileRessource(cri2);

		// Création du joueur principal
	CPlayer *erwin = new CPlayer();				// Crée le joueur principal (celui géré par le clavier et l'écran)
	erwin->changeAction( gravitePlayer );		// Associe au joueur une fonction de gravité
	erwin->changeContact( contactPlayer );		// Associe une fonction de gestion des contacts avec la map
	erwin->Skin( pMapJoueur );
	erwin->setCri( cri1.c_str() );							// Cri du joueur
	erwin->nom( "ERWIN" );
	erwin->init();				// Initialise certaines données
	erwin->choiceOneEntryPoint();
	Game.Erwin( erwin );		// Indique que 'erwin' est le joueur principal
	Game.pTabIndexPlayer->Ajoute( 0, erwin );		// Ajoute le joueur principal à la liste des joueurs

		// Création d'un second joueur
	CPlayer *julien;
	julien = new CPlayer();						// Crée un autre joueur
	julien->changeAction( gravitePlayer );		// Associe au joueur une fonction de gravité
	julien->changeContact( contactPlayer );		// Associe une fonction pour les contacts avec la map
	julien->Skin( pMapJoueur2 );
	julien->setCri( cri1.c_str() );
	julien->nom( "JULIEN" );
	julien->init();
	julien->choiceOneEntryPoint();
	Game.pTabIndexPlayer->Ajoute( 1, julien );				// Ajoute le joueur à la liste des joueurs

		// Création d'un troisième joueur
	CPlayer *sprite;
	sprite = new CPlayer();						// Crée un autre joueur
	sprite->changeAction( gravitePlayer );		// Associe au joueur une fonction de gravité
	sprite->changeContact( contactSprite );		// Associe une fonction pour les contacts avec la map
	sprite->Skin( pMapJoueur );
	sprite->setCri( cri2.c_str() );
	sprite->nom( "SPRITE" );
	sprite->init();
	sprite->choiceOneEntryPoint();
	Game.pTabIndexPlayer->Ajoute( 2, sprite );				// Ajoute le joueur à la liste des joueurs

	return true;
}

void openMAP2( const string &nomFichierMap )
{
	if( !Game.openMap( nomFichierMap ) )
		cerr << endl << "Erreur à l'ouverture du fichier Map : " << nomFichierMap;

		// Lecture du map du joueur
	CMap *pMapJoueur = new CMap( nomFichierJoueur );

		// Ajuste les coordonnées
	pMapJoueur->EchangeXZ();
	pMapJoueur->Scale( -0.15f, 0.15f, 0.15f );
}

void boucle()
{
	Uint32 temps=0;
	Uint32 oldTemps=0;
	ecart=0;

	for( ;; )	// Boucle principale du jeu
	{
		if( Game.RequeteProcess.isOuvreMap() )	// S'il y a une demande d'ouvertue de MAP
		{
			Aide = false;
			openMAP2( Game.RequeteProcess.getOuvreMap() );	// Ouvre la MAP voulue
			Game.setStatutClient( JKT_STATUT_CLIENT_PLAY );	// Indique que la partie est lancée
			pFocus->SetPlayFocus();
		}

		if( Game.RequeteProcess.isTakePicture() )	// S'il y a une demande de prise de photo de la scène
		{
			CPhoto photo( Config.Display.X, Config.Display.Y );

			string fichier_photo("./Images/photo.bmp");
			if( photo.Save( fichier_photo ) )
				cout << "\nPhoto prise.";
			else
				cerr << "\nEchec a la prise de la photo";
		}

			// Mesure du temps écoulé depuis les derniers calculs
		if(temps == 0)
		{
			temps = SDL_GetTicks();
			oldTemps = temps;
		}
		else
		{
			oldTemps = temps;
			temps = SDL_GetTicks();
		}

		ecart = temps - oldTemps;

			// Effectue tous les calculs du jeu
		timer( ecart );

		ecartTimer = SDL_GetTicks() - temps;

			// Dessine la scène 3D et les menus
		display();

		//ecartDisplay = SDL_GetTicks() - temps - ecartTimer;

		process_events();	// vérifie les évênements
	}
}

int main(int argc, char** argv)
{
TRACE().p( TRACE_OTHER, "main(argc=%d,argv=%x)", argc, argv );

	atexit( quit_JKT );

	string fichier;

	Config.AfficheDateCompilation();		// Affiche le n° de version du programme
	Config.NommeConfig( nomFichierConfig );	// Nomme le fichier de configuration
	Config.Lit();							// Lit le fichier de configuration

	cout << "\n\tRESUME DE L'INITIALISATION VIDEO";
	Config.Display.Init();	// Initialisation vidéo (SDL + OpenGL)

	Config.Reseau.Init();	// Initialisation du réseau

	Config.Audio.Init();	// Initialisation audio
	Config.Ecrit();			// Enregistre les éventuelles modifications de la configuration

	cout << "\n\tINFO DIVERSES";

		// Info réseau
	IPaddress ipaddress;
	if( !SDLNet_ResolveHost( &ipaddress, "localhost", 0 ) )
	{
		const char *host = SDLNet_ResolveIP( &ipaddress );
		if( host )
			cout << "\nNom de la machine locale : " << host;
		else
			cout << "\nNom de la machine locale : " << "Inconnu";
	}
	else
    {
		cout << "\nNom de la machine locale : " << "Inconnu";
    }

    Fabrique::construct();

		// Création du démon de gestion des sons
	DemonSons = new CDemonSons();

		// Lancement de l'introduction du jeu
	load_Intro( Config.Display.X, Config.Display.Y );	// Affiche l'introduction du jeu

		// Initialisation pour les menus et boîtes de dialogue
	{
		string fonte = "@Fonte\\Fonte.glf";		// Chargement de la fonte de caractères
		JKT_PACKAGE_UTILS::RessourcesLoader::getFileRessource(fonte);
		unsigned int texFonte;
		glGenTextures(1, &texFonte);

		if( !myfont.Create(fonte.c_str(), texFonte) )
		{
			TRACE().p( TRACE_ERROR, "main() Echec ouverture texture de fonte (%s) : %d", fonte, texFonte );
			cerr << "\nErreur : Echec d'ouverture de la fonte : " << fonte << endl;
		}
		else
		{
			TRACE().p( TRACE_INFO, "main() Texture de fonte (%s) : %d", fonte.c_str(), texFonte );
		}
	}

		// Initialisation de la classe CDlgBoite pour l'IHM
	if( !CDlgBoite::INIT_CLASSE() )
		return 1;	// Erreur fatale si la classe n'a pas pu être initialisée

		// Initialisation de la classe CRocket, réfléchir où mettre cette initialisation
	if( !CRocket::INIT_CLASSE() )
		return 1;	// Erreur fatale si CRocket ne peut être initialisée

		// Mise en place du moteur de particules pour la neige, réfléchir où mettre ça
	CV3D posMoteurParticules( -2.35f, 1.5f, 0.0f );
	moteurParticules = new CMoteurParticules( posMoteurParticules, 1000, 0.05f );

		// Délai d'attente après l'intro du jeu, je sais plus à quoi il sert
	SDL_Delay( 1000 );

		// Création d'un haut parleur qui se déplace pour tester le son 3D
	machin = new CMachin();
	string fichierSon3D = "@Musique\\drumloop.wav";		// Chargement de la fonte de caractères
	JKT_PACKAGE_UTILS::RessourcesLoader::getFileRessource(fichierSon3D);
	CSon3D *son3D = DemonSons->CreateSon3D( fichierSon3D.c_str() );
	CReqSon *reqSon = DemonSons->PlayID( (CSon*)son3D, true );
	reqSon->Boucle( true );
	machin->req_son = reqSon;
	DemonSons->Play( machin->req_son );

	init();

		// Lancement de l'IHM, on entre dans le menu principal du jeu
	lanceMenuPrinc( 0 );

		// Entrée dans la boucle principale du jeu
	boucle();

	return 1;	// On ne devrait normalement jamais exécuter cette ligne
}

