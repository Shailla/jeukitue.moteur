

/*//////////////////////////////////////////////////////////////////////////////////////////////
					CHOSES A FAIRE

Les fonctions de callback des players (gravitï¿½, contact, ...)ne sont pas
implï¿½mentï¿½es proprement, il ne faudrait pas avoir besoin de leur fournir le pointeur
sur le player (mais j'sais pas comment faire)

Formaliser les vecteurs position et vitesse de maniï¿½re ï¿½ ce qu'il ne soit plus
indispensable d'inverser parfois certaines de leurs composantes selon l'utilisation

/*///////////////////////////////////////////////////////////////////////////////////////////////

// A virer
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include <vector>

#ifdef WIN32
#include <windows.h>
#include <io.h>
#include <direct.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <agar/config/have_opengl.h>
#include <agar/config/have_sdl.h>
#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/cursors.h>
#include <agar/gui/sdl.h>
#include <agar/gui/opengl.h>

using namespace std;

#define Pi				3.14159265f			//nombre pi pour les calculs
#define quantumVitesse	0.003f				//pas d'augmentation de la vitesse du joueur (accï¿½lï¿½ration discrï¿½te)
#define DELAY_TIMER		10					// Temps entre 2 calculs de scï¿½nes (en ms)
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
#include "util/IpUtils.h"
#include "util/math_vectoriel.h"		//Fonctions de calculs vectoriels
#include "util/V3D.h"
#include "util/mathFast.h"
#include "util/Trace.h"				// Gestion du fichier de traces
#include "util/Tableau.cpp"			//Liste chaï¿½nï¿½e
#include "util/TableauIndex.cpp"		//Liste Indexï¿½e
#include "util/FindFolder.h"
#include "util/Erreur.h"
#include "ressource/RessourcesLoader.h"

class CGame;

#include "spatial/widget/GraphicObject.h"
#include "main/Cfg.h"
#include "spatial/materiau/Material.h"
#include "spatial/IfstreamMap.h"
#include "spatial/geo/Geo.h"				//Paramï¿½tres des objets gï¿½omï¿½triques
#include "spatial/objet/Dirigeable.h"
#include "spatial/Mouve.h"
#include "spatial/geo/GeoObject.h"
#include "Rocket.h"
#include "Laser.h"
#include "spatial/objet/Porte.h"
#include "spatial/objet/Navette.h"
#include "main/Clavier.h"		//Requï¿½tes du clavier
#include "Photo.h"
#include "spatial/materiau/MaterialTexture.h"	//Paramï¿½tres des matï¿½riaux associï¿½s aux objets gï¿½omï¿½triques
#include "spatial/materiau/MaterialMulti.h"
#include "spatial/light/Light.h"			//Lumiï¿½res de la map
#include "spatial/Map.h"
#include "son/DemonSons.h"
#include "son/Son.h"
#include "son/ReqSon.h"
#include "reseau/SPA.h"
#include "main/Player.h"
#include "spatial/contact.h"		//Fonctions utilisï¿½es pour le contact
#include "main/divers.h"		//Initialisation d'SDL
#include "son/audio.h"
#include "ihm/DlgBoite.h"
#include "ihm/Portail.h"
#include "ihm/Menu.h"			// Fonctions de gestion du menu
#include "main/Focus.h"			// Gestion du focus
#include "spatial/materiau/Textures.h"
#include "util/TableauIndex.h"
#include "reseau/Client.h"
#include "reseau/Server.h"
#include "main/RequeteProcess.h"
#include "main/Game.h"
#include "util/V3D.h"
#include "spatial/Particule.h"
#include "spatial/MoteurParticules.h"
#include "spatial/MoteurParticulesNeige.h"
#include "reseau/NetworkManager.h"
#include "reseau/enumReseau.h"
#include "menu/Controller.h"
#include "menu/Viewer.h"
#include "main/Fabrique.h"
#include "menu/ConsoleView.h"
#include "game/GameDto.h"
#include "game/MapLoader.h"
#include "spatial/contact.h"
#include "data/ServeurDataTree.h"
#include "data/ClientDataTree.h"
#include "data/DistantTreeProxy.h"

#include "reseau/new/ClientUdpInterlocutor.h"
#include "reseau/new/ServerUdpInterlocutor.h"

#include "test/TestSuite.h"

using namespace JktMenu;
using namespace JktNet;
using namespace JktMoteur;
using namespace JktUtils;
using namespace JktSon;

#include "util/GenRef.h"

#include "jkt.h"

NotConnectedInterlocutor2* _notConnectedServerInterlocutor = 0;

float GLIGHTX, GLIGHTY, GLIGHTZ;
GLFont myfont;

CCfg Config;		// Contient la configuration du jeu
CGame Game;			// Contient toutes les données vivantes du jeu

const char nomFichierJoueur[] = "@Joueur\\joueurTex";

int JKT_RenderMode = GL_TRIANGLES;	// Rendu normal ou en mode filaire
bool JKT_AfficheToutesTextures = false;

string nomFichierConfig = "config";

CFocus *pFocus;

int numMainPlayer = 0;	// Numï¿½ro du joueur principal dans la MAP (identifie le joueur principal dans la liste des joueurs de la MAP)

bool Aide = false;

extern JktSon::CDemonSons *DemonSons;	// Requêtes des sons

Uint32 tempsTimer = 0;		// Temps pris par la fonction 'timer'
Uint32 tempsDisplay = 0;	// Temps pris par la fonction 'display'

SDL_mutex* _grahicObjectsToInitializeMutex;
vector<GraphicObject*> _grahicObjectsToInitialize;

SDL_mutex* _grahicObjectsToDestructMutex;
vector<GraphicObject*> _grahicObjectsToDestruct;

class CMachin {
public:
	CMachin();

	CV3D position;
	CV3D vitesse;
	CReqSon *req_son;
};

CMachin::CMachin() {
	position.X = 2.0f;
	position.Y = 0.2f;
	position.Z = 0.0f;
	vitesse.X = 0.0f;
	vitesse.Y = 0.0f;
	vitesse.Z = 0.05f;
	req_son = NULL;
}

float delta = 0.0;

extern CFocus *pFocus;				// Gestion du focus
extern bool Aide;					// Indique que le menu d'aide est actif

CMoteurParticules* moteurParticulesNeige;
unsigned int frpsTimer = 0, frpTimer = 0;

CMachin *machin;	// Pour tester le son 3D

NetworkManager* _networkManager;

void gravitePlayer(CPlayer *player)	// Fonction implï¿½mentant la gravitï¿½
{									// aux objets qui doivent la subire
	float vect[3];
	player->getVitesse( vect );

	vect[1] -= 0.5*quantumVitesse; // Ajout de gravitï¿½ au joueur

	player->setVitesse( vect );
}

/**
 * Initialise les menu du jeu, affiche le menu principal et place le focus dessus.
 */
void initMenu(void) {
	pFocus = new CFocus(play_handle_key_down,
			CDlg::menu_handle_key_down,
			menu_agar_handle_key_down);
	pFocus->SetPlayFocus();
	Aide = false;

	// Lancement de l'IHM, on entre dans le menu principal du jeu
	pFocus->SetMenuAgarFocus();		// Place le focus sur le menu

	AG_Event event;
	AG_EventArgs(&event, "%i", Controller::Action::ShowMenuAction);
	Controller::executeAction(&event);
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

	// Position du point d'ï¿½coute (le joueur)
	FSOUND_3D_Listener_SetAttributes(pos_erwin, 0, fx,fy,fz,tx,ty,tz);

	// Position de l'ï¿½metteur du son
	((CReqSon3D*)machin->req_son)->SetPosition( pos_machin );
	FSOUND_Update();

	glPushMatrix();
	glTranslatef( machin->position.X, machin->position.Y, machin->position.Z );
	glColor3f( 1.0f, 1.0f, 0.0f );

	GLUquadric* quadric = gluNewQuadric();
	gluSphere(quadric, 0.05, 16, 16);
	gluDeleteQuadric(quadric);

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
			str2 = "Etat : Demande de JTG envoyï¿½e";
			break;
		case JKT_STATUT_CLIENT_OUV:
			str2 = "Etat : Ouverture de la MAP en cours";
			break;
		case JKT_STATUT_CLIENT_PLAY:
			str2 = "Etat : Jeu en cours";
			break;
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
	char cou[70];

	// Affiche le mode du jeu
	if(Game.getMap() && Game.getMap()->IsSelectionMode()) {
		sprintf( cou, "Selection : %s", Game.getMap()->getSelectedName());
		str = cou;
		myfont.DrawString( cou, TAILLEFONT, 20.0f, pos++*15.0f+20.0f );
	}

	if(Game.Erwin()) {
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

	if(Config.Debug.bSonPerformances) {
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

	if(Config.Debug.bSonSpectre) {
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

		glEnd();

		glDisable( GL_BLEND );

		glBegin( GL_QUADS );

		for(int i=0 ; i < NBR_RAIES_SPECTRE ; i++) {
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

void drawSmallQuad(const int x, const int y) {
	glBegin(GL_QUADS);

	if(((abs(x%2)==0) && (abs(y%2)==0)) || ((abs(x%2)==1) && (abs(y%2)==1))) {
		glColor3f(1.0f, 1.0f, 1.0f);
	}
	else {
		glColor3f(0.0f, 0.0f, 0.0f);
	}

	glVertex3f(-0.0f,  -0.0f, 0.0f);
	glVertex3f(-0.0f,  +0.25f, 0.0f);
	glVertex3f(+0.25f, +0.25f, 0.0f);
	glVertex3f(+0.25f, -0.0f, 0.0f);

	glEnd();
}

void drawQuad() {
	for(int x=-2 ; x<2 ; x++) {
		for(int y=-2 ; y<2 ; y++) {
			glPushMatrix();
			glTranslatef(float(x) * 0.25f, float(y) * 0.25f, 0.0f);
			drawSmallQuad(x, y);
			glPopMatrix();
		}
	}
}

void addGraphicObjectToInitialize(GraphicObject* graphicObject) {
	SDL_LockMutex(_grahicObjectsToInitializeMutex);

	_grahicObjectsToInitialize.push_back(graphicObject);

	SDL_UnlockMutex(_grahicObjectsToInitializeMutex);
}

void addGraphicObjectToDestruct(GraphicObject* graphicObject) {
	SDL_LockMutex(_grahicObjectsToDestructMutex);

	_grahicObjectsToDestruct.push_back(graphicObject);

	SDL_UnlockMutex(_grahicObjectsToDestructMutex);
}

void display() {		// Fonction principale d'affichage
	Uint32 temps = SDL_GetTicks();	// Temps au début du réaffichage

	CSPA::computeDebits(temps);

	/* **********************************************************
	 * Mise en place de la perspective
	 * *********************************************************/

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 60.0, Config.Display.X/Config.Display.Y, 0.01f, 100.0f );
	glEnable( GL_DEPTH_TEST );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	if(Game.getMap()) {		// Si il y a une map a afficher
		CPlayer* erwin = Game.Erwin();

		if(erwin) {		// S'il y a un joueur principal
			float vect[3];

			erwin->getPosVue(vect);
			glTranslatef(-vect[0], -vect[1], vect[2]);	// Placement du point de vue
			glRotated(erwin->PhiVue(), 1.0, 0.0, 0.0);	// Rotation par rapport au plan horizontal

			glRotated(erwin->Phi(), 1.0, 0.0, 0.0);	// Rotation par rapport au plan horizontal
			glRotated(erwin->Teta(), 0.0,1.0, 0.0);	// Rotation par rapport ï¿½ l'axe verticale

			erwin->getPosition(vect);
			glTranslatef(-vect[0], -vect[1], vect[2]);	// Placement du point de vue

			glRotated(erwin->TetaVue(), 0.0,1.0, 0.0);	// Rotation par rapport ï¿½ l'axe verticale
		}

		Game.AffichePlayers();		// Affiche tous les joueurs

		glEnable(GL_DEPTH_TEST);

		Game.getMap()->Affiche();	// Affichage de la map
		Game.AfficheProjectils();

		// Dessine les axes dans la map	(sert au repï¿½rage pour la conception du jeu)
		if(Config.Debug.axesMeterVisibility) {
			glLineWidth( 3 );
			glBegin(GL_LINES);	// Dï¿½ssine les axes X Y Z
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
		}

		// Dessine les axes dans la map	(sert au repï¿½rage pour la conception du jeu)
		if(Config.Debug.cubicMeterVisibility) {
			glPushMatrix();
			glTranslatef(0.0f, 0.0f, -0.5f);
			drawQuad();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0.0f, 0.0f, +0.5f);
			drawQuad();
			glPopMatrix();

			glPushMatrix();
			glRotated(90.0f, 1.0, 0.0, 0.0);
			glTranslatef(0.0f, 0.0f, -0.5f);
			drawQuad();
			glPopMatrix();

			glPushMatrix();
			glRotated(90.0f, 1.0, 0.0, 0.0);
			glTranslatef(0.0f, 0.0f, +0.5f);
			drawQuad();
			glPopMatrix();

			glPushMatrix();
			glRotated(90.0f, 0.0, 1.0, 0.0);
			glTranslatef(0.0f, 0.0f, -0.5f);
			drawQuad();
			glPopMatrix();

			glPushMatrix();
			glRotated(90.0f, 0.0, 1.0, 0.0);
			glTranslatef(0.0f, 0.0f, +0.5f);
			drawQuad();
			glPopMatrix();
		}

		glColor3f(1.0f, 0.2f, 0.2f);

		GLUquadric* quadric = gluNewQuadric();
		gluSphere(quadric, 0.05, 16, 16);
		gluDeleteQuadric(quadric);

		if( Game.Erwin() )
			updateSon3D();	// Positionne le joueur et les objets bruyants dans l'espace sonore

		// AFFICHAGE DES PARTICULES
		glEnable( GL_BLEND );
		glDepthMask( GL_FALSE );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE );

		moteurParticulesNeige->Affiche();	// Le moteur de particules affiche toutes ses particules

		glDisable( GL_BLEND );
		glDepthMask( GL_TRUE );
	}


	/* *******************************************************
	 * Affichage des elements avants (menu, score, ...)
	 * ******************************************************/

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluOrtho2D(0.0, Config.Display.X, 0.0, Config.Display.Y);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glEnable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );
	glDisable( GL_DEPTH_TEST );

	if(JKT_AfficheToutesTextures)
		Game.afficheToutesTextures(0, 0, Config.Display.X, Config.Display.Y);

	if(Game.Erwin()) {
		Game.Erwin()->AfficheIconesArmes();
		Game.afficheViseur(Config.Display.X/2, Config.Display.Y/2);
		glDepthMask( GL_TRUE );
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor3f(1.0, 1.0, 1.0 );

	afficheInfo(SDL_GetTicks()-temps);	// Affiche les infos texte en avant plan

	if( Aide )
		CDlg::afficheMenuActif();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();


	/******************************************
	   Dessin des menus Agar
	 *****************************************/

	glEnable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );
	glDisable( GL_DEPTH_TEST );

	glOrtho(0, (double)agDriverSw->w, (double)agDriverSw->h, 0.0, -1.0, 1.0);

	Fabrique::getAgarView()->draw();

	glDisable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );


	/*******************************************************
	  Echange des buffers graphique -> affiche à l'écran.
	 ******************************************************/

	SDL_GL_SwapBuffers();


	/* *****************************************************
	 * Initialisation et destruction des objets graphiques
	 * ****************************************************/

	GraphicObject* object;

	// Initialisation des objets graphiques à initialiser
	SDL_LockMutex(_grahicObjectsToInitializeMutex);

	while (!_grahicObjectsToInitialize.empty()) {
		object = _grahicObjectsToInitialize.back();
		object->initializeGraphicObject();

		_grahicObjectsToInitialize.pop_back();
	}
	SDL_UnlockMutex(_grahicObjectsToInitializeMutex);

	// Descruction des objets graphiques à détruire
	SDL_LockMutex(_grahicObjectsToDestructMutex);

	while (!_grahicObjectsToDestruct.empty()) {
		object = _grahicObjectsToDestruct.back();
		object->destructGraphicObject();

		_grahicObjectsToDestruct.pop_back();
	}
	SDL_UnlockMutex(_grahicObjectsToDestructMutex);

	((ConsoleView*)Fabrique::getAgarView()->getView(Viewer::CONSOLE_VIEW))->setDureeDisplay(SDL_GetTicks() - temps);
}

void chopeLesEvenements() {
	Uint8 *keystate = SDL_GetKeyState(0);		// Récupère les touches clavier appuyées
	Uint8 mouse = SDL_GetMouseState(0, 0);

	if(Game.Erwin()) {
		CPlayer *erwin = Game.Erwin();

		if( keystate[SDLK_a] ) {
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

		if( keystate[SDLK_q] ) {
			erwin->TetaVue( erwin->TetaVue() - 0.2f );
			if( erwin->TetaVue() < -180.0f )
				erwin->TetaVue( erwin->TetaVue() + 360.0f );
		}

		if( keystate[SDLK_w] ) {
			erwin->PhiVue( erwin->PhiVue() + 0.2f );

			if( erwin->PhiVue() > 180.0f )
				erwin->PhiVue( erwin->PhiVue() - 360.0f );
		}

		if( keystate[SDLK_s] ) {
			erwin->PhiVue( erwin->PhiVue() - 0.2f );

			if( erwin->PhiVue() < -180.0f )
				erwin->PhiVue( erwin->PhiVue() + 360.0f );
		}

		if( keystate[SDLK_e] ) {
			float posVue[3];
			erwin->getPosVue(posVue);
			posVue[0] += 0.003f;
			erwin->setPosVue(posVue);
		}

		if( keystate[SDLK_d] ) {
			float posVue[3];
			erwin->getPosVue(posVue);
			posVue[0] -= 0.003f;
			erwin->setPosVue(posVue);
		}

		if( keystate[SDLK_r] ) {
			float posVue[3];
			erwin->getPosVue(posVue);
			posVue[1] += 0.003f;
			erwin->setPosVue(posVue);
		}

		if( keystate[SDLK_f] ) {
			float posVue[3];
			erwin->getPosVue(posVue);
			posVue[1] -= 0.003f;
			erwin->setPosVue(posVue);
		}

		if( keystate[SDLK_t] ) {
			float posVue[3];
			erwin->getPosVue(posVue);
			posVue[2] += 0.003f;
			erwin->setPosVue(posVue);
		}

		if( keystate[SDLK_g] ) {
			float posVue[3];
			erwin->getPosVue(posVue);
			posVue[2] -= 0.003f;
			erwin->setPosVue(posVue);
		}

		if ( keystate[Config.Commandes.Gauche.key]||(mouse&SDL_BUTTON(Config.Commandes.Gauche.mouse)) )	{	//gauche
			erwin->getClavier()->m_fDroite = -1.0;
			erwin->getClavier()->m_bIndic = true;
		}

		if( keystate[Config.Commandes.Reculer.key]||(mouse&SDL_BUTTON(Config.Commandes.Reculer.mouse)) ) {	// Reculer
			erwin->getClavier()->m_fAvance = -1.0;
			erwin->getClavier()->m_bIndic = true;
		}

		if( keystate[Config.Commandes.Avancer.key]||(mouse&SDL_BUTTON(Config.Commandes.Avancer.mouse)) ) {	// Avant
			erwin->getClavier()->m_fAvance = 1.0;
			erwin->getClavier()->m_bIndic = true;
		}

		if( keystate[Config.Commandes.Droite.key]||(mouse&SDL_BUTTON(Config.Commandes.Droite.mouse)) ) {	// Droite
			erwin->getClavier()->m_fDroite = 1.0;
			erwin->getClavier()->m_bIndic = true;
		}

		if( keystate[Config.Commandes.Tir1.key]||(mouse&SDL_BUTTON(Config.Commandes.Tir1.mouse)) ) {	// Tire avec l'arme sélectionnée
			erwin->Tir();		// Valide un tir
		}

		if( keystate[Config.Commandes.Monter.key]||(mouse&SDL_BUTTON(Config.Commandes.Monter.mouse)) ) {	// Monter
			erwin->getClavier()->m_fMonte = 1.0;
			erwin->getClavier()->m_bIndic = true;
		}

		if( keystate[Config.Commandes.Tir2.key]||(mouse&SDL_BUTTON(Config.Commandes.Tir2.mouse)) ) {	// Tire de projectile qui rebondit
			CPlayer *balle;

			float cosTeta = /*FastCos0( erwin->Teta/180.0f*Pi );	//*/cosf( erwin->Teta()/180.0f*Pi );
			float sinTeta = /*FastSin0( erwin->Teta/180.0f*Pi );	//*/sinf( erwin->Teta()/180.0f*Pi );
			float cosPhi = /*FastCos0( erwin->Phi/180.0f*Pi );	//*/cosf( erwin->Phi()/180.0f*Pi );
			float sinPhi = /*FastSin0( erwin->Phi/180.0f*Pi );	//*/sinf( erwin->Phi()/180.0f*Pi );

			balle = new CPlayer(); //crée un autre joueur
			float vect[3];
			erwin->getPosition( vect );
			balle->setPosition( vect[0], vect[1], vect[2]);		// positionne le projectile sur la map

			erwin->getVitesse( vect );
			balle->changeVitesse( sinTeta*cosPhi*10*quantumVitesse + vect[0],
					sinPhi*10*quantumVitesse + vect[1],
					cosTeta*cosPhi*10*quantumVitesse + vect[2]);

			balle->changeAction( gravitePlayer );	// associe au projectile une fonction de gravitï¿½
			balle->changeContact( contactSprite );	// associe une fonction pour les contacts avec la map

			Game._pTabIndexPlayer->Ajoute(balle);			// ajoute le projectile à la liste des joueurs
		}
	}
}

void menu_agar_handle_key_down(SDL_Event *sdlEvent) {
//	cout << endl << " -> AGAR";
//	string evDesc;
//	CCfg::resolve(sdlEvent, evDesc);
//	cout << " -> {" << evDesc << "}";

	if(sdlEvent->type == SDL_KEYDOWN && sdlEvent->key.keysym.sym == SDLK_ESCAPE) {
		pFocus->SetPlayFocus();

		AG_Event event;
		AG_EventArgs(&event, "%i", Controller::Action::HideMenuAction);
		Controller::executeAction(&event);
	}
	else {
		switch (sdlEvent->type) {

		case SDL_MOUSEBUTTONDOWN:
		{
			AG_DriverEvent agarEvent;
			AG_SDL_TranslateEvent(agDriverSw, sdlEvent, &agarEvent);
			AG_ProcessEvent((AG_Driver *)agDriverSw, &agarEvent);
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			AG_DriverEvent agarEvent;
			AG_SDL_TranslateEvent(agDriverSw, sdlEvent, &agarEvent);
			AG_ProcessEvent((AG_Driver *)agDriverSw, &agarEvent);
			break;
		}
		case SDL_MOUSEMOTION:
		case SDL_KEYDOWN:
		case SDL_KEYUP:
		case SDL_VIDEORESIZE:
		case SDL_VIDEOEXPOSE:
		case SDL_QUIT:
		{
			AG_DriverEvent agarEvent;
			AG_SDL_TranslateEvent(agDriverSw, sdlEvent, &agarEvent);
			AG_ProcessEvent((AG_Driver *)agDriverSw, &agarEvent);
			break;
		}
		default:
			// Event ignored by Agar
			//			cout << endl << " Ignored by agar";
			break;
		}
	}

//	AG_DriverEvent dev;
//
//	while(AG_GetNextEvent(NULL, &dev) > 0) {
//	    AG_ProcessEvent(NULL, &dev);
//	}
}

void play_handle_key_down( SDL_Event *event ) {
	//	if( pMap && erwin )
	//	{
	//		float cosTeta =	/*FastCos0(erwin->Teta/180.0f*Pi);		//*/cosf(erwin->Teta/180.0f*Pi);
	//		float sinTeta = /*FastSin0(erwin->Teta/180.0f*Pi);		//*/sinf(erwin->Teta/180.0f*Pi);
	//		float cosPhi =	/*FastCos0(erwin->Phi/180.0f*Pi);		//*/cosf(erwin->Phi/180.0f*Pi);
	//		float sinPhi =	/*FastSin0(erwin->Phi/180.0f*Pi);		//*/sinf(erwin->Phi/180.0f*Pi);
	//	}

	CPlayer *erwin = Game.Erwin();

	switch( event->type ) {
	case SDL_MOUSEMOTION:		// Si c'est un évênement 'souris'
		if( erwin ) {
			// Rotation par rapport au plan horizontal (regarder en l'air ou vers le bas)
			erwin->Phi( erwin->Phi() + event->motion.yrel );

			if(erwin->Phi()>90.0)
				erwin->Phi( 90.0f );

			if(erwin->Phi()<-90.0)
				erwin->Phi( -90.0f );

			// Rotation par rapport à l'axe verticale (regarder à droite ou à gauche)
			erwin->Teta( erwin->Teta() + event->motion.xrel );
			if(erwin->Teta()>180.0f)
				erwin->Teta( erwin->Teta() - 360.0f );

			if(erwin->Teta()<-180.0f)
				erwin->Teta( erwin->Teta() + 360.0f );
		}
		break;

	case SDL_MOUSEBUTTONDOWN:
		if( erwin ) {
			unsigned char mouseButtonDown = event->button.button;

			// Tir de l'arme active
			if(mouseButtonDown == Config.Commandes.Tir1.mouse) {
				erwin->Tir();	// Tir avec l'arme active
			}
			// Sï¿½lection arme suivante
			else if(mouseButtonDown == Config.Commandes.SelectWeaponUp.mouse) {
				erwin->ActiveArmeUp();
			}
			// Sï¿½lection arme prï¿½cédente
			else if(mouseButtonDown == Config.Commandes.SelectWeaponDown.mouse) {
				erwin->ActiveArmeDown();
			}
		}
		break;

	case SDL_KEYDOWN:
	{
		SDLKey keyDown = event->key.keysym.sym;

		if( erwin ) {
			// Tir de l'arme active
			if(keyDown == Config.Commandes.Tir1.key) {
				erwin->Tir();
			}
			// Sélection arme suivante
			else if(keyDown == Config.Commandes.SelectWeaponUp.key) {
				erwin->ActiveArmeUp();
			}
			// Sélection arme précédente
			else if(keyDown == Config.Commandes.SelectWeaponDown.key) {
				erwin->ActiveArmeDown();
			}
		}

		switch(keyDown) {
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

			cerr << endl << __FILE__ << ":" << __LINE__;
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

		case SDLK_g :		// Désactive / active la gravité
			if( Game.getMap() )
				Game.Gravite( !Game.Gravite() );
			break;

		case SDLK_v :		// Annule la vitesse du joueur actif
			if( Game.getMap() )
				Game.Erwin()->changeVitesse(0.0f, 0.0f, 0.0f);
			break;

		case SDLK_RETURN :	// Sauve le fichier map actuel
			if(Game.getMap()) {
				ConsoleView* console = ((ConsoleView*)Fabrique::getAgarView()->getView(Viewer::CONSOLE_VIEW));

				time_t currentTime;
				time(&currentTime);
				ostringstream saveMapName;
				saveMapName << "save-" << currentTime;
				console->println(ConsoleView::COT_INFO, string("Enregistrement de la Map : ") + saveMapName.str());

				Game.getMap()->Save(saveMapName.str());
			}
			break;

		case SDLK_KP_PLUS :
			if( Game.getMap() && Game.getMap()->IsSelectionMode() ) {
				cout << endl << "Selection du suivant";
				Game.getMap()->incrementeSelection();
			}
			break;

		case SDLK_KP_MINUS :
			if( Game.getMap() && Game.getMap()->IsSelectionMode() ) {
				cout << endl << "Selection du precedent";
				Game.getMap()->decrementeSelection();
			}
			break;

		case SDLK_s :
			if(Game.getMap()) {
				Game.getMap()->ChangeSelectionMode();
			}
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

		case SDLK_t:		// Affiche les textures actives dans un damier
			JKT_AfficheToutesTextures = !JKT_AfficheToutesTextures;
			break;

		case SDLK_o :		// Retour du joueur à la position d'origine avec vitesse nulle
			if(Game.Erwin()) {
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
			cout << endl << "Nombre de joeurs dans la partie : " << Game._pTabIndexPlayer->getNbr();

			if(Game.getMap()) {
				Game._pTabIndexPlayer->Suivant(numMainPlayer);	// Lit le numï¿½ro du joueur suivant

				if(numMainPlayer < 0) {				// Si on a atteint la fin de la liste alors prend ï¿½ nouveau le premier de liste
					Game._pTabIndexPlayer->Suivant(numMainPlayer);
				}

				// Sï¿½lectionne le joueur en tant que joueur principal
				Game.Erwin(Game._pTabIndexPlayer->operator [](numMainPlayer));
			}
			break;

		case SDLK_ESCAPE:
		{
			pFocus->SetMenuAgarFocus();		// Place le focus sur le menu

			AG_Event event;
			AG_EventArgs(&event, "%i", Controller::Action::ShowMenuAction);
			Controller::executeAction(&event);
		}
		break;

		case SDLK_F2:
			lanceMenuPrinc( 0 );
			break;

		case SDLK_F5:
			pFocus->SwitchPlayOrConsoleFocus();		// Place le focus sur le menu
			break;

		default:
			break;
		}

		break;
	}
	}
}

static void process_events(void) {
	SDL_Event event;

	while( SDL_PollEvent( &event ) ) {
		switch( event.type ) {
		case SDL_QUIT:
			quit_game( 0 );
			break;

		default:
			pFocus->ExecFocus( &event );
			break;
		}
	}
}

bool deprecatedOpenMAP(const void *nomFichier) {
	/**************************************
	 * Ouverture de la Map proprement dite
	 **************************************/

	string nomFichierMap = (char*)nomFichier;

	if( !Game.openMap( nomFichierMap ) ) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur à l'ouverture du fichier Map : " << nomFichierMap;
		return false;
	}

	// Crï¿½ation joueurs
	Game.setPlayerList( 10 );	// Indique que la partie peut contenir jusqu'ï¿½ 10 joueurs


	/**************************************
	 * Chargement de Map pour les joueurs
	 **************************************/

	// Lecture de map de joueurs
	string mapJoueurPrincipal;
	mapJoueurPrincipal.append("@Joueur\\").append(Config.Joueur.mapName);

	CMap *pMapJoueur = new CMap(0, mapJoueurPrincipal);
	pMapJoueur->EchangeXZ();					// Ajuste les coordonnï¿½es
	pMapJoueur->Scale( -0.03f, 0.03f, 0.03f );

	CMap *pMapJoueur2 = new CMap(0, nomFichierJoueur);
	pMapJoueur2->EchangeXZ();					// Ajuste les coordonnï¿½es
	pMapJoueur2->Scale( -0.10f, 0.10f, 0.10f );

	cout << endl;


	/**************************************
	 * Chargement de sons pour les joueurs
	 **************************************/

	// Rï¿½cupï¿½ration des ressources de cris des personnages
	string cri1 = "@Bruit\\cri_1.wav";
	JktUtils::RessourcesLoader::getFileRessource(cri1);

	string cri2 = "@Bruit\\cri_1.wav";
	JktUtils::RessourcesLoader::getFileRessource(cri2);


	/***********************************
	 * Crï¿½ation des joueurs dans la Map
	 ***********************************/

	// Crï¿½ation du joueur principal
	CPlayer *erwin = new CPlayer();				// Crï¿½e le joueur principal (celui gï¿½rï¿½ par le clavier et l'ï¿½cran)
	erwin->changeAction( gravitePlayer );		// Associe au joueur une fonction de gravitï¿½
	erwin->changeContact( contactPlayer );		// Associe une fonction de gestion des contacts avec la map
	erwin->Skin( pMapJoueur );
	erwin->setCri( cri1.c_str() );				// Cri du joueur
	erwin->nom( "ERWIN" );
	erwin->init();								// Initialise certaines donnï¿½es
	erwin->choiceOneEntryPoint();
	Game.Erwin( erwin );						// Indique que 'erwin' est le joueur principal
	Game._pTabIndexPlayer->Ajoute( 0, erwin );	// Ajoute le joueur principal ï¿½ la liste des joueurs

	// Crï¿½ation d'un second joueur
	CPlayer *julien;
	julien = new CPlayer();						// Crï¿½e un autre joueur
	julien->changeAction( gravitePlayer );		// Associe au joueur une fonction de gravitï¿½
	julien->changeContact( contactPlayer );		// Associe une fonction pour les contacts avec la map
	julien->Skin( pMapJoueur2 );
	julien->setCri( cri1.c_str() );
	julien->nom( "JULIEN" );
	julien->init();
	julien->choiceOneEntryPoint();
	Game._pTabIndexPlayer->Ajoute( 1, julien );	// Ajoute le joueur ï¿½ la liste des joueurs

	// Crï¿½ation d'un troisiï¿½me joueur
	CPlayer *sprite;
	sprite = new CPlayer();						// Crï¿½e un autre joueur
	sprite->changeAction( gravitePlayer );		// Associe au joueur une fonction de gravitï¿½
	sprite->changeContact( contactSprite );		// Associe une fonction pour les contacts avec la map
	sprite->Skin( pMapJoueur );
	sprite->setCri( cri2.c_str() );
	sprite->nom( "SPRITE" );
	sprite->init();
	sprite->choiceOneEntryPoint();
	Game._pTabIndexPlayer->Ajoute( 2, sprite );	// Ajoute le joueur ï¿½ la liste des joueurs

	return true;
}

void openMap(const string& nomFichierMap) {
	((ConsoleView*)Fabrique::getAgarView()->getView(Viewer::CONSOLE_VIEW))->setMapOuverteName(nomFichierMap);

	if( !Game.openMap(nomFichierMap) ) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur a l'ouverture du fichier Map : " << nomFichierMap;
	}
}

GameDto* localeGameDto;
GameDto* clientGameDto;
GameDto* serverGameDto;


void executeJktRequests() {

	//	/* *******************************************************
	//	 * Exï¿½cution de l'ouverture d'une MAP en mode multijoueurs
	//	 * ******************************************************/
	//
	//	if( Game.RequeteProcess.isOuvreMap() ) {		// S'il y a une demande d'ouvertue de MAP
	//		Aide = false;
	//		const string mapName = Game.RequeteProcess.getOuvreMap();
	//
	//		openMap( mapName );	// Ouvre la MAP voulue
	//
	//		Game.setStatutClient( JKT_STATUT_CLIENT_PLAY );		// Indique que la partie est lancï¿½e en mode client
	//		pFocus->SetPlayFocus();								// Met l'interception des commandes sur le mode jeu
	//
	//		Fabrique::getAgarView()->showView(Viewer::CONSOLE_VIEW);
	//	}


	/* *******************************************************
	 * Exï¿½cution du workflow d'ouverture d'une MAP locale
	 * ******************************************************/

	int etape = Game.RequeteProcess.getOuvreMapLocaleEtape();

	switch(etape) {
	case CRequeteProcess::OMLE_AUCUNE:				// Aucune ouverture de MAP locale en cours
		// Nothing to do
		break;

	case CRequeteProcess::OMLE_DEMANDE:
	{
		Fabrique::getAgarView()->showMenuView(Viewer::CONSOLE_VIEW);	// Affichage de la console

		// Fermeture de la MAP courante et destruction des joueurs
		CMap* currentMap = Game.getMap();
		if(currentMap) {
			currentMap->freePlugins();
			currentMap->freeGL();
			Game.changeActiveMap(NULL);
		}

		Game.Erwin(NULL);

		if(Game._pTabIndexPlayer) {
			CPlayer *player;
			int playerIndex = -1;
			while(Game._pTabIndexPlayer->Suivant(playerIndex)) {
				player = Game._pTabIndexPlayer->operator [](playerIndex);
				player->freeGL();
			}

			delete Game._pTabIndexPlayer;
			Game._pTabIndexPlayer = NULL;
		}

		Game.setLocalDataTree(new LocalDataTree());

		// Lancement ouverture MAP demandée
		const string mapName = Game.RequeteProcess.getOuvreMap();

		localeGameDto = new GameDto(mapName);

		Game.RequeteProcess.setOuvreMapLocaleEtape(CRequeteProcess::OMLE_OUVERTURE_EN_COURS);

		MapLoader::launchLocalGameLoading(localeGameDto);		// Lance l'ouverture de la MAP
	}
	break;

	case CRequeteProcess::OMLE_OUVERTURE_EN_COURS:	// Attente de la fin de l'ouverture de la nouvelle MAP dans la mï¿½thode "openMap"
		// Nothing to do
		break;

	case CRequeteProcess::OMLE_OUVERTURE:
	{
		ConsoleView* console = ((ConsoleView*)Fabrique::getAgarView()->getView(Viewer::CONSOLE_VIEW));
		console->setMapOuverteName(localeGameDto->getMapName());

		// Activation de la MAP ouverte
		CMap* map = localeGameDto->getMap();
		map->initGL();
		map->initPlugins();
		Game.changeActiveMap(map);

		Dirigeable* dirigeable = new Dirigeable(0);
		map->add(dirigeable);

		// Définition des joueurs
		Game.setPlayerList(localeGameDto->getPlayersMaxNumber());

		CPlayer* erwin = localeGameDto->getErwin();


		int i = 0;

		if(erwin != NULL) {
			Game.Erwin(erwin);									// Indique que 'erwin' est le joueur principal
			Game._pTabIndexPlayer->Ajoute( i++, erwin );		// Ajoute le joueur principal à la liste des joueurs
		}

		for(vector<CPlayer*>::iterator iter = localeGameDto->getPlayers().begin() ; iter != localeGameDto->getPlayers().end() ; ++iter) {
			Game._pTabIndexPlayer->Ajoute( i++, *iter );				// Ajoute le joueur à la liste des joueurs
		}

		CPlayer *player;
		int playerIndex = -1;

		while(Game._pTabIndexPlayer->Suivant(playerIndex)) {
			player = (*Game._pTabIndexPlayer)[playerIndex];
			player->initGL();
			player->choiceOneEntryPoint();
		}

		delete localeGameDto;

		// Lancement du jeu en mode local
		Aide = false;
		pFocus->SetPlayFocus();						// Met l'interception des commandes sur le mode jeu
		Game.setModeLocal();						// Jeu en mode jeu local
		Game.RequeteProcess.setOuvreMapLocaleEtape(CRequeteProcess::OMLE_AUCUNE);

		cout << "\nFINI";
		cout << flush;
	}
	break;
	}


	/* **************************************************************
	 * Exécution du workflow d'ouverture d'une MAP en mode client
	 * *************************************************************/

	etape = Game.RequeteProcess.getOuvreMapClientEtape();

	switch(etape) {
	case CRequeteProcess::OMCE_AUCUNE:				// Aucune ouverture de MAP locale en cours
		// Nothing to do
		break;

	case CRequeteProcess::OMCE_DEMANDE:
	{
		Fabrique::getAgarView()->showMenuView(Viewer::CONSOLE_VIEW);	// Affichage de la console

		// Connexion du client
		Interlocutor2* clientInterlocutor = _networkManager->ouvreClient(Config.Reseau.getIpServer(), Config.Reseau.getServerPort(), Config.Reseau.getServerPortTree());

		if(!clientInterlocutor) {
			Game.RequeteProcess.setOuvreMapClientEtape(CRequeteProcess::OMCE_AUCUNE);
			AG_TextMsg(AG_MSG_ERROR, "Echec de connexion du client au serveur");
		}
		else {
			// Création de l'arbre des données du client
			Game.setClientDataTree(new ClientDataTree(string("jkt"), clientInterlocutor));

			// Lancement ouverture MAP demandée
			const string mapName = Game.RequeteProcess.getOuvreMap();

			clientGameDto = new GameDto(mapName);

			Game.RequeteProcess.setOuvreMapClientEtape(CRequeteProcess::OMCE_OUVERTURE_EN_COURS);

			MapLoader::launchClientGameLoading(clientGameDto);		// Lance l'ouverture de la MAP
		}
	}
	break;

	case CRequeteProcess::OMCE_OUVERTURE:
	{
		// Activation de la MAP ouverte
		CMap* map = clientGameDto->getMap();
		map->initGL();
		Game.changeActiveMap(map);

		// Activation de chacun des joueurs
		CPlayer *player;
		int playerIndex = -1;

		while(Game._pTabIndexPlayer->Suivant(playerIndex)) {
			player = (*Game._pTabIndexPlayer)[playerIndex];
			player->initGL();
		}

		// Lancement du jeu en mode local
		Aide = false;
		pFocus->SetPlayFocus();						// Met l'interception des commandes sur le mode jeu

		JktNet::CClient *client = Game.getClient();
		client->nomMAP = clientGameDto->getMapName();			// Informe le serveur sur le nom de la MAP lancï¿½e
		client->setStatut( JktNet::JKT_STATUT_CLIENT_PLAY );
		Game.setModeClient();						// Jeu en mode jeu local

		delete clientGameDto;

		cout << "\nFINI";
		cout << flush;

		Game.RequeteProcess.setOuvreMapClientEtape(CRequeteProcess::OMCE_AUCUNE);
	}
	break;
	}

	/* **************************************************************
	 * Exï¿½cution du workflow d'ouverture d'une MAP en mode serveur
	 * *************************************************************/

	etape = Game.RequeteProcess.getOuvreMapServerEtape();

	switch(etape) {
	case CRequeteProcess::OMSE_AUCUNE:				// Aucune ouverture de MAP locale en cours
		// Nothing to do
		break;

	case CRequeteProcess::OMSE_DEMANDE:
	{
		Fabrique::getAgarView()->showMenuView(Viewer::CONSOLE_VIEW);	// Affichage de la console

		// Fermeture de la MAP courante et destruction des joueurs
		CMap* currentMap = Game.getMap();
		if(currentMap) {
			currentMap->freeGL();
			Game.changeActiveMap(NULL);
		}

		Game.Erwin(NULL);

		if(Game._pTabIndexPlayer) {
			CPlayer *player;
			int playerIndex = -1;
			while(Game._pTabIndexPlayer->Suivant(playerIndex)) {
				player = Game._pTabIndexPlayer->operator [](playerIndex);
				player->freeGL();
			}

			delete Game._pTabIndexPlayer;
			Game._pTabIndexPlayer = NULL;
		}

		// Création de l'arbre des données du serveur
		_notConnectedServerInterlocutor = _networkManager->ouvreServer(Config.Reseau.getServerPort(), Config.Reseau.getServerPortTree());

		if(!_notConnectedServerInterlocutor) {
			Game.RequeteProcess.setOuvreMapServerEtape(CRequeteProcess::OMSE_AUCUNE);
			AG_TextMsg(AG_MSG_ERROR, "Echec de connexion du serveur");
		}
		else {
			// Connexion du serveur
			Game.setServerDataTree(new ServeurDataTree());

			// Lancement ouverture MAP demandée
			const string mapName = Game.RequeteProcess.getOuvreMap();

			serverGameDto = new GameDto(mapName);

			Game.RequeteProcess.setOuvreMapServerEtape(CRequeteProcess::OMSE_OUVERTURE_EN_COURS);

			MapLoader::launchServerGameLoading(serverGameDto);		// Lance l'ouverture de la MAP
		}
	}
	break;

	case CRequeteProcess::OMSE_OUVERTURE_EN_COURS:	// Attente de la fin de l'ouverture de la nouvelle MAP dans la méthode "openMap"
		// Nothing to do
		break;

	case CRequeteProcess::OMSE_OUVERTURE:
	{
		// Activation de la MAP ouverte
		CMap* map = serverGameDto->getMap();
		map->initGL();
		Game.changeActiveMap(map);

		// Dï¿½finition des joueurs
		Game.setPlayerList(serverGameDto->getPlayersMaxNumber());

		CPlayer* erwin = serverGameDto->getErwin();


		int i = 0;

		if(erwin != NULL) {
			Game.Erwin(erwin);									// Indique que 'erwin' est le joueur principal
			Game._pTabIndexPlayer->Ajoute( i++, erwin );		// Ajoute le joueur principal ï¿½ la liste des joueurs
		}

		for(vector<CPlayer*>::iterator iter = serverGameDto->getPlayers().begin() ; iter != serverGameDto->getPlayers().end() ; ++iter) {
			Game._pTabIndexPlayer->Ajoute( i++, *iter );				// Ajoute le joueur ï¿½ la liste des joueurs
		}

		CPlayer *player;
		int playerIndex = -1;

		// Initialisation de chacun des joueurs
		while(Game._pTabIndexPlayer->Suivant(playerIndex)) {
			player = (*Game._pTabIndexPlayer)[playerIndex];
			player->initGL();
			player->choiceOneEntryPoint();
		}


		// Lancement du jeu en mode local
		Aide = false;
		pFocus->SetPlayFocus();						// Met l'interception des commandes sur le mode jeu

		JktNet::CServer *server = Game.getServer();
		server->nomMAP = serverGameDto->getMapName();					// Informe le serveur sur le nom de la MAP lancï¿½e
		Game.setPlayerList( server->maxPlayers );
		server->setStatut( JktNet::JKT_STATUT_SERVER_PLAY );
		Game.setModeServer();						// Jeu en mode jeu local
		server->bGame = true;						// Indique qu'une partie est en cours

		delete serverGameDto;

		cout << "\nFINI";
		cout << flush;

		Game.RequeteProcess.setOuvreMapServerEtape(CRequeteProcess::OMSE_AUCUNE);
	}
	break;
	}


	/* *******************************************
	 * Exécution d'une capture d'écran
	 * ******************************************/

	if( Game.RequeteProcess.isTakePicture() ) {	// S'il y a une demande de prise de photo de la scène
		CPhoto photo( Config.Display.X, Config.Display.Y );

		string fichier_photo("./Images/photo.bmp");

		if( photo.Save( fichier_photo ) )
			cout << "\nPhoto prise.";
		else
			cerr << endl << __FILE__ << ":" << __LINE__ << " Echec a la prise de la photo";
	}
}

void communique() {
	frpTimer++;

	if(_networkManager->getOn()) {	// Si le réseau est actif

		/* *******************************************
		 * Le serveur émet et réceptionne des données
		 * ******************************************/

		// Envoie des données aux clients
		if(Game.isModeServer()) {
			if(Game.getStatutServer() == JKT_STATUT_SERVER_PLAY) {

				// Emet les données vers les clients
				Game.getServer()->emet();

				// Rï¿½ception des donnï¿½es des clients
				if(Game.getMap()) {			// Si une partie est en cours en mode de jeu client ou serveur
					_networkManager->recoitServer();	// Recoit les donnï¿½es des clients
				}
			}
		}


		/* *******************************************
		 * Le client émet et réceptionne des données
		 * ******************************************/

		// Le client réceptionne les données du serveur
		CClient* client = _networkManager->getClient();

		if(client) {
			// Recoit les données du serveur en mode déconnecté, ainsi qu'en mode connecté si on l'est
			client->recoit();

			// Emet les données ddu joueur actif vers le serveur
			if(Game.getMap() && Game.Erwin() && Game.isModeClient() && (Game.getStatutClient() == JKT_STATUT_CLIENT_PLAY)) {
				client->emet( *Game.Erwin() );		// Emet les requetes et donnï¿½es du joueur actif;
			}
		}
	}
}

/* ***************************************
 * Boucle du jeu prinipale
 * **************************************/
void boucle() {

	for( ;; ) {
		executeJktRequests();


		/* ***********************************************************
		 * Traites les connexions à l'arbre de données
		 * **********************************************************/

		if(_notConnectedServerInterlocutor) {
			Interlocutor2* newInterlocutor;

			while((newInterlocutor = _notConnectedServerInterlocutor->popNewInterlocutor())) {
				ServeurDataTree* serverDataTree = Game.getServerDataTree();

				if(serverDataTree) {		// TODO serveurDataTree devrait être protégé par un mutex
					serverDataTree->addDistant(newInterlocutor);
				}
			}
		}


		/* ***********************************************************
		 * Traites les changements des données du jeu
		 * **********************************************************/

		// Traite les changements de données côté serveur
		ServeurDataTree* serverDataTree = Game.getServerDataTree();

		if(serverDataTree) {		// TODO serveurDataTree devrait être protégé par un mutex
			serverDataTree->receiveChangementsFromClients();		// Le serveur reçoit les changements des donnée du jeu de la part des clients
			serverDataTree->diffuseChangementsToClients();			// Diffuse les changements des données du jeu aux clients
		}

		// Traite les changements de données côté client
		ClientDataTree* clientDataTree = Game.getClientDataTree();

		if(clientDataTree) {		// TODO clientDataTree devrait être protégé par un mutex
			clientDataTree->receiveChangementsFromServer();		// Le client reçoit les changements des donnï¿½es du jeu de la part du serveur
			clientDataTree->diffuseChangementsToServer();		// Le client reçoit diffuse les changements des donnï¿½es du jeu vers le serveur
		}


		/* **********************************************
		 * Lecture des événements claviers, souris, ...
		 * *********************************************/

		if( Game.getMap()) {	// Si une partie est en cours en mode de jeu local, client ou serveur
			if( 	Game.isModeLocal()
					|| 	(Game.isModeClient() && Game.getStatutClient()==JKT_STATUT_CLIENT_PLAY)
					|| 	(Game.isModeServer() && Game.getStatutServer()==JKT_STATUT_SERVER_PLAY) ) {
				chopeLesEvenements();	// Prends les requï¿½tes du joueur pour permettre mï¿½me au serveur de jouer
			}
		}


		/* ****************************************************************
		 * Communique
		 * (échange des données client -> serveur ou serveur -> clients)
		 * ****************************************************************/

		communique();


		/* ****************************************************
		 * Effectue les calculs physiques
		 * (gravitï¿½, gestion des contacts, dï¿½placements, ...)
		 * ***************************************************/
		Uint32 temps = SDL_GetTicks();	// Temps au dï¿½but du timer (->mesure du temps de calcul)

		Game.timer();

		((ConsoleView*)Fabrique::getAgarView()->getView(Viewer::CONSOLE_VIEW))->setDureeCalcules(SDL_GetTicks() - temps);


		/* ***********************************************************
		 * Affiche la scï¿½ne
		 * **********************************************************/

		// Dessine la scène 3D et les menus
		display();

		SDL_PumpEvents();	// Collecte les événements
		process_events();	// vérifie les événements
	}
}

int main(int argc, char** argv) {
	TRACE().p( TRACE_OTHER, "main(argc=%d,argv=%x)", argc, argv );

	if(argc > 1) {
		// Exécution des tests unitaires
		if(string("test") == argv[1]) {
			cout << endl << "MODE TEST" << endl;
			JktTest::TestSuite* testSuite = new JktTest::TestSuite();

			testSuite->init();
			testSuite->launchTests();
			exit(0);
		}
	}


	atexit( quit_JKT );

	string fichier;

	Config.AfficheDateCompilation();		// Affiche la date de compilation du programme
	Config.NommeConfig( nomFichierConfig );	// Nomme le fichier de configuration
	Config.Lit();							// Lit le fichier de configuration

	cout << "\n\tRESUME DE L'INITIALISATION VIDEO";
	Config.Display.Init();	// Initialisation SDL, OpenGL et Agar

	Config.Reseau.Init();	// Initialisation du rï¿½seau
	_networkManager = new NetworkManager();

	Config.Audio.Init();	// Initialisation audio
	Config.Ecrit();			// Enregistre les ï¿½ventuelles modifications de la configuration

	srand(SDL_GetTicks());		// Initialisation de la fonction rand() pour les nombres alï¿½atoires

	_grahicObjectsToInitializeMutex = SDL_CreateMutex();
	_grahicObjectsToDestructMutex = SDL_CreateMutex();

	cout << "\n\tINFO DIVERSES";

	// Info réseau
	IPaddress ipaddress;
	if(!SDLNet_ResolveHost( &ipaddress, "localhost", 0 )) {
		const char *host = SDLNet_ResolveIP( &ipaddress );

		if(host)
			cout << endl << "Nom et adresse de la machine locale : " << host << " / " << IpUtils::translateAddress(ipaddress);
		else
			cout << endl << "Nom de la machine locale : <Inconnu>" << " / " << IpUtils::translateAddress(ipaddress);
	}
	else {
		cout << endl << "Echec de resolution du nom de la machine locale";
	}

	Fabrique::construct();

	// Création du démon de gestion des sons
	DemonSons = new CDemonSons();

	// Lancement de l'introduction du jeu
	if(Config.General._playIntro) {
		load_Intro( Config.Display.X, Config.Display.Y );	// Affiche l'introduction du jeu
	}

	// Initialisation pour les menus et boîtes de dialogue
	{
		string fonte = "@Fonte\\Fonte.glf";		// Chargement de la fonte de caractï¿½res
		JktUtils::RessourcesLoader::getFileRessource(fonte);
		unsigned int texFonte;
		glGenTextures(1, &texFonte);

		if( !myfont.Create(fonte.c_str(), texFonte) ) {
			TRACE().p( TRACE_ERROR, "main() Echec ouverture texture de fonte (%s) : %d", fonte.c_str(), texFonte );
			cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur : Echec d'ouverture de la fonte : " << fonte << endl;
		}
		else {
			TRACE().p( TRACE_INFO, "main() Texture de fonte (%s) : %d", fonte.c_str(), texFonte );
		}
	}

	// Initialisation de la classe CDlgBoite pour l'IHM
	if( !CDlgBoite::INIT_CLASSE() )
		return 1;	// Erreur fatale si la classe n'a pas pu être initialisée

	// Initialisation de la classe CRocket, réfléchir oû mettre cette initialisation
	if( !CRocket::INIT_CLASSE() )
		return 1;	// Erreur fatale si CRocket ne peut être initialisée



	// Mise en place du moteur de particules pour la neige, réfléchir où mettre ça
	CV3D posMoteurParticulesNeige( -2.35f, 1.5f, 0.0f );
	CV3D tailleMoteurParticulesNeige(3.0f, 3.0f, 3.0f);
	moteurParticulesNeige = new CMoteurParticulesNeige(10000, posMoteurParticulesNeige, tailleMoteurParticulesNeige);

	// Délai d'attente aprés l'intro du jeu, je sais plus à quoi il sert
	SDL_Delay( 1000 );

	// Création d'un haut parleur qui se déplace pour tester le son 3D
	machin = new CMachin();
	string fichierSon3D = "@Musique\\drumloop.wav";
	JktUtils::RessourcesLoader::getFileRessource(fichierSon3D);
	CSon3D *son3D = DemonSons->CreateSon3D( fichierSon3D.c_str() );
	CReqSon *reqSon = DemonSons->PlayID( (CSon*)son3D, true );
	reqSon->Boucle( true );
	machin->req_son = reqSon;
	DemonSons->Play( machin->req_son );

	// Initialisation des plugins démarrés par défaut
	Fabrique::getPluginEngine()->activateDefaultGlobalPlugins();

	initMenu();

	// Entrée dans la boucle principale du jeu
	Fabrique::getAgarView()->showView(Viewer::CONSOLE_VIEW);
	boucle();

	return 1;	// On ne devrait normalement jamais exï¿½cuter cette ligne
}

