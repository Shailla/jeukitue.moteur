

/*//////////////////////////////////////////////////////////////////////////////////////////////
					CHOSES A FAIRE

Les fonctions de callback des players (gravité, contact, ...)ne sont pas
implémentées proprement, il ne faudrait pas avoir besoin de leur fournir le pointeur
sur le player (mais j'sais pas comment faire)

Formaliser les vecteurs position et vitesse de manière à ce qu'il ne soit plus
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
#include <time.h>

#ifdef WIN32
#include <windows.h>
#include <io.h>
#include <direct.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include <agar/config/have_opengl.h>
#include <agar/config/have_sdl.h>
#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/cursors.h>
#include <agar/gui/sdl.h>
#include <agar/gui/opengl.h>

using namespace std;

#define Pi				3.14159265f			//nombre pi pour les calculs
#define quantumVitesse	0.003f				//pas d'augmentation de la vitesse du joueur (accélération discrête)
#define DELAY_TIMER		10					// Temps entre 2 calculs de scènes (en ms)
#define TAILLEFONT		0.7f				// Taille des fontes

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_net.h"
#include "fmod.h"
#include "fmod_errors.h"	/* optional */
#include "main/glfont2.h"
#include "tinyxml.h"
#include "util/XmlHelper.h"
using namespace glfont;

#include "util/GenRef.h"
#include "util/IpUtils.h"
#include "util/math_vectoriel.h"		//Fonctions de calculs vectoriels
#include "util/V3D.h"
#include "util/mathFast.h"
#include "util/Trace.h"				// Gestion du fichier de traces
#include "util/Tableau.cpp"			//Liste chaînée
#include "util/TableauIndex.cpp"		//Liste Indexée
#include "util/FindFolder.h"
#include "util/Erreur.h"
#include "ressource/RessourcesLoader.h"

class CGame;

#include "spatial/widget/GraphicObject.h"
#include "main/Cfg.h"
#include "spatial/materiau/Material.h"
#include "spatial/IfstreamMap.h"
#include "spatial/geo/Geo.h"				//Paramètres des objets géométriques
#include "spatial/objet/Dirigeable.h"
#include "spatial/Mouve.h"
#include "spatial/geo/GeoObject.h"
#include "reseau/web/HtmlServer.h"
#include "Rocket.h"
#include "Laser.h"
#include "spatial/objet/Porte.h"
#include "spatial/objet/Navette.h"
#include "main/Clavier.h"		//Requêtes du clavier
#include "Photo.h"
#include "spatial/materiau/MaterialTexture.h"	//Paramètres des matériaux associés aux objets géométriques
#include "spatial/materiau/MaterialMulti.h"
#include "spatial/light/Light.h"			//Lumières de la map
#include "spatial/Map.h"
#include "son/DemonSons.h"
#include "son/Son.h"
#include "son/ReqSon.h"
#include "reseau/SPA.h"
#include "main/Player.h"
#include "spatial/contact.h"		//Fonctions utilisées pour le contact
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
#include "data/DataTreeUtils.h"

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

GLuint fonteTex;
GLFont myfont;

CCfg Config;		// Contient la configuration du jeu
CGame Game;			// Contient toutes les données vivantes du jeu

const char nomFichierJoueur[] = "@Joueur\\joueurTex";

int JKT_RenderMode = GL_TRIANGLES;	// Rendu normal ou en mode filaire
bool JKT_AfficheToutesTextures = false;

string nomFichierConfig = "config";

CFocus *pFocus;

int numMainPlayer = 0;	// Numéro du joueur principal dans la MAP (identifie le joueur principal dans la liste des joueurs de la MAP)

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

unsigned int frpsTimer = 0, frpTimer = 0;

CMachin *machin;	// Pour tester le son 3D

NetworkManager* _networkManager;

void gravitePlayer(CPlayer *player)	// Fonction implémentant la gravité
{									// aux objets qui doivent la subire
	float vect[3];
	player->getVitesse( vect );

	vect[1] -= 0.5*quantumVitesse; // Ajout de gravité au joueur

	player->setVitesse( vect );
}

/**
 * Initialise les menu du jeu, affiche le menu principal et place le focus dessus.
 */
void initMenu(void) {
	pFocus = new CFocus(play_handle_key_down, CDlg::menu_handle_key_down);
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

	// Position du point d'écoute (le joueur)
	FSOUND_3D_Listener_SetAttributes(pos_erwin, 0, fx,fy,fz,tx,ty,tz);

	// Position de l'émetteur du son
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

void afficheInfo( Uint32 tempsDisplay ) {
	myfont.Begin();

	// Affiche le mode de jeu (rien, local, client ou serveur)
	string str;

	str = "Mode de jeu : ";
	if( Game.isModeNull() ) {
		str += "aucun";
	}
	else if( Game.isModeLocal() ) {
		str += "local";
	}
	else if( Game.isModeClient() ) {
		str += "client";

		switch( Game.getStatutClient() )
		{
		case JKT_STATUT_CLIENT_INIT:
			str += " (initialisation)";
			break;
		case JKT_STATUT_CLIENT_READY:
			str += " (ready)";
			break;
		case JKT_STATUT_CLIENT_DEMJTG:
			str += " (demande de JTG envoyée)";
			break;
		case JKT_STATUT_CLIENT_OUV:
			str += " (ouverture de la MAP)";
			break;
		case JKT_STATUT_CLIENT_PLAY:
			str += " (jeu en cours)";
			break;
		default:
			break;
		}
	}
	else if( Game.isModeServer() ) {
		str += "serveur";
		switch( Game.getStatutServer() )
		{
		case JKT_STATUT_SERVER_INIT:
			str += " (initialisation)";
			break;
		case JKT_STATUT_SERVER_READY:
			str += " (ready)";
			break;
		case JKT_STATUT_SERVER_PLAY:
			str += " (jeu en cours)";
			break;
		default:
			break;
		}
	}

	int pos = 0;

	myfont.DrawString(str, TAILLEFONT, 20.0f, ((float)Config.Display.Y) - 20.0f - pos++*15.0f);



//	int surface = AG_WidgetMapSurface(0, AG_TextRender("Coucou ma grosse caille"));
//
//	AG_WidgetBlitSurface(NULL, surface, 200, 200);



	glBindTexture(GL_TEXTURE_2D, fonteTex);

	glEnable( GL_TEXTURE_2D );

	glColor3f( 1.0f, 1.0f, 1.0f );

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(100.0, 200.0, 0.0);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(200.0, 200.0, 0.0);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(200.0, 300.0, 0.0);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(100.0, 300.0, 0.0);
	glEnd();














	char cou[70];

	// Affiche le mode du jeu
	if(Game.getMap() && Game.getMap()->IsSelectionMode()) {
		sprintf( cou, "Selection : %s", Game.getMap()->getSelectedName());
		str = cou;
		myfont.DrawString( cou, TAILLEFONT, 20.0f, ((float)Config.Display.Y) - 20.0f - pos++*15.0f);
	}

	CPlayer *erwin = Game.Erwin();

	if(erwin) {
		// Affiche le Teta du joueur principal
		sprintf( cou, "Teta Phi : %.3d %.3d", (int)erwin->Teta(), (int)erwin->Phi());
		str = cou;
		myfont.DrawString( str, TAILLEFONT, 20.0f, ((float)Config.Display.Y) - 20.0f - pos++*15.0f);

		// Affiche la position du joueur principal
		float position[3];
		erwin->getPosition( position );

		sprintf( cou, "Position : %0.4f %0.4f %0.4f", position[0], position[1], position[2] );
		str = cou;
		myfont.DrawString( str, TAILLEFONT, 20.0f, ((float)Config.Display.Y) - 20.0f - pos++*15.0f);
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
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );					// Spécifie la couleur de vidage du tampon chromatique
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 90.0, Config.Display.X/Config.Display.Y, 0.01f, 50.0f );

	glMatrixMode( GL_MODELVIEW );
	glEnable( GL_DEPTH_TEST );
	glLoadIdentity();

	if(Game.getMap()) {		// Si il y a une map a afficher
		CPlayer* erwin = Game.Erwin();

		if(erwin) {		// S'il y a un joueur principal
			float vect[3];

			erwin->getPosVue(vect);
			glTranslatef(-vect[0], -vect[1], vect[2]);	// Placement du point de vue

			glRotated(erwin->Phi(), 1.0, 0.0, 0.0);		// Rotation par rapport au plan horizontal
			glRotated(erwin->Teta(), 0.0, 1.0, 0.0);	// Rotation par rapport à l'axe verticale

			erwin->getPosition(vect);
			glTranslatef(-vect[0], -vect[1], vect[2]);	// Placement du point de vue
		}

		Game.AfficheDirigeables();
		Game.AffichePlayers();		// Affiche tous les joueurs

		glEnable(GL_DEPTH_TEST);

		Game.getMap()->Affiche();	// Affichage de la map
		Game.AfficheProjectils();

		// Dessine les axes dans la map	(sert au repérage pour la conception du jeu)
		if(Config.Debug.axesMeterVisibility) {
			glLineWidth( 3 );
			glBegin(GL_LINES);	// Dessine les axes X Y Z
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

		// Dessine les axes dans la map	(sert au repérage pour la conception du jeu)
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

			balle->changeAction( gravitePlayer );	// associe au projectile une fonction de gravité
			balle->changeContact( contactSprite );	// associe une fonction pour les contacts avec la map

			//			Game._pTabIndexPlayer->Ajoute(balle);			// ajoute le projectile à la liste des joueurs
			Game.addPlayer(balle);			// ajoute le projectile à la liste des joueurs
		}
	}
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
			// Sélection arme suivante
			else if(mouseButtonDown == Config.Commandes.SelectWeaponUp.mouse) {
				erwin->ActiveArmeUp();
			}
			// Sélection arme précédente
			else if(mouseButtonDown == Config.Commandes.SelectWeaponDown.mouse) {
				erwin->ActiveArmeDown();
			}
		}
		break;

	case SDL_KEYDOWN: {
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

		case SDLK_F3 : {
			LOGINFO(("Derniere erreur FMOD : %s", FMOD_ErrorString(FSOUND_GetError())));
			LOGINFO(("Derniere erreur SDL : %s", SDL_GetError()));
			LOGINFO(("Derniere erreur SDL_Net : %s", SDLNet_GetError()));
			LOGINFO(("Derniere erreur openGL : %s", (const char*)gluErrorString( glGetError() )));
			LOGINFO(("Derniere erreur Agar : %s", AG_GetError()));
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
				LOGINFO(("Selection du suivant"));
				Game.getMap()->incrementeSelection();
			}
			break;

		case SDLK_KP_MINUS :
			if( Game.getMap() && Game.getMap()->IsSelectionMode() ) {
				LOGINFO(("Selection du precedent"));
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
			cout << endl << "Nombre de joeurs dans la partie : " << Game.getNbrPlayers();

			if(Game.getMap()) {
				CPlayer* erwin = Game.nextPlayer(numMainPlayer);	// Lit le numéro du joueur suivant

				if(!erwin) {				// Si on a atteint la fin de la liste alors prend à nouveau le premier de liste
					erwin = Game.nextPlayer(numMainPlayer);
				}

				// Sélectionne le joueur en tant que joueur principal
				Game.Erwin(erwin);
				((ConsoleView*)Fabrique::getAgarView()->getView(Viewer::CONSOLE_VIEW))->setActivePlayerName(erwin->nom());
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
	SDL_Event sdlevent;
	AG_DriverEvent dev;

	while( SDL_PollEvent( &sdlevent ) ) {

		/* ****************************************** */
		/* Gestion événements prioritaires            */
		/* ****************************************** */

		switch( sdlevent.type ) {
		case SDL_QUIT:
			quit_game("SDL quit event", 0 );
			break;

			// Bascule entre le menu Agar et le jeu
		case SDL_KEYDOWN:
			if(sdlevent.key.keysym.sym == SDLK_ESCAPE) {
				if(pFocus->isPlayFocus()) {
					AG_Event event;
					AG_EventArgs(&event, "%i", Controller::Action::ShowMenuAction);
					Controller::executeAction(&event);

					pFocus->SetMenuAgarFocus();
				}
				else {
					AG_Event event;
					AG_EventArgs(&event, "%i", Controller::Action::HideMenuAction);
					Controller::executeAction(&event);

					pFocus->SetPlayFocus();
				}
			}
			break;

		default:
			break;
		}


		/* ****************************************** */
		/* Gestion événements du jeu                  */
		/* ****************************************** */

		pFocus->ExecFocus( &sdlevent );


		/* ****************************************** */
		/* Gestion événements menu Agar               */
		/* ****************************************** */

		AG_SDL_TranslateEvent(agDriverSw, &sdlevent, &dev);
		AG_ProcessEvent(NULL, &dev);
	}

	AG_WindowProcessQueued();
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

	// Création joueurs
	Game.createPlayerList( 10 );	// Indique que la partie peut contenir jusqu'à 10 joueurs


	/**************************************
	 * Chargement de Map pour les joueurs
	 **************************************/

	// Lecture de map de joueurs
	string mapJoueurPrincipal;
	mapJoueurPrincipal.append("@Joueur\\").append(Config.Joueur.mapName);

	CMap *pMapJoueur = new CMap(0, mapJoueurPrincipal);
	pMapJoueur->EchangeXZ();					// Ajuste les coordonnées
	pMapJoueur->Scale( -0.06f, 0.06f, 0.06f );

	CMap *pMapJoueur2 = new CMap(0, nomFichierJoueur);
	pMapJoueur2->EchangeXZ();					// Ajuste les coordonnées
	pMapJoueur2->Scale( -0.06f, 0.06f, 0.06f );

	cout << endl;


	/**************************************
	 * Chargement de sons pour les joueurs
	 **************************************/

	// Récupération des ressources de cris des personnages
	string cri1 = "@Bruit\\cri_1.wav";
	JktUtils::RessourcesLoader::getFileRessource(cri1);

	string cri2 = "@Bruit\\cri_1.wav";
	JktUtils::RessourcesLoader::getFileRessource(cri2);


	/***********************************
	 * Création des joueurs dans la Map
	 ***********************************/

	// Création du joueur principal
	CPlayer *erwin = new CPlayer();				// Crée le joueur principal (celui géré par le clavier et l'écran)
	erwin->changeAction( gravitePlayer );		// Associe au joueur une fonction de gravité
	erwin->changeContact( contactPlayer );		// Associe une fonction de gestion des contacts avec la map
	erwin->Skin( pMapJoueur );
	erwin->setCri( cri1.c_str() );				// Cri du joueur
	erwin->nom( "ERWIN" );
	erwin->init();								// Initialise certaines données
	erwin->choiceOneEntryPoint();
	Game.addPlayer(erwin);	// Ajoute le joueur principal à la liste des joueurs
	Game.Erwin(erwin);						// Indique que 'erwin' est le joueur principal

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
	Game.addPlayer(julien);	// Ajoute le joueur à la liste des joueurs

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
	Game.addPlayer(sprite);	// Ajoute le joueur à la liste des joueurs

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
	//	 * Exécution de l'ouverture d'une MAP en mode multijoueurs
	//	 * ******************************************************/
	//
	//	if( Game.RequeteProcess.isOuvreMap() ) {		// S'il y a une demande d'ouvertue de MAP
	//		Aide = false;
	//		const string mapName = Game.RequeteProcess.getOuvreMap();
	//
	//		openMap( mapName );	// Ouvre la MAP voulue
	//
	//		Game.setStatutClient( JKT_STATUT_CLIENT_PLAY );		// Indique que la partie est lancée en mode client
	//		pFocus->SetPlayFocus();								// Met l'interception des commandes sur le mode jeu
	//
	//		Fabrique::getAgarView()->showView(Viewer::CONSOLE_VIEW);
	//	}


	/* *******************************************************
	 * Exécution du workflow d'ouverture d'une MAP locale
	 * ******************************************************/

	int etape = Game.RequeteProcess.getOuvreMapLocaleEtape();

	switch(etape) {
	case CRequeteProcess::OMLE_AUCUNE:				// Aucune ouverture de MAP locale en cours
		// Nothing to do
		break;

	case CRequeteProcess::OMLE_DEMANDE:
	{
		Fabrique::getAgarView()->showMenuView(Viewer::CONSOLE_VIEW);	// Affichage de la console


		/* *****************************************
		 * Fermeture Map courante
		 * ****************************************/

		// Fermeture de la MAP courante et destruction des joueurs
		Game.quitCurrentMap();

		Game.Erwin(0);
		Game.deletePlayers();


		/* *****************************************
		 * Ouverture Map serveur
		 * ****************************************/

		Game.setModeLocal();

		LocalDataTree* dataTree = new LocalDataTree();
		DataTreeUtils::formatGameServerDataTree(dataTree);
		Game.setLocalDataTree(dataTree);

		// Lancement ouverture MAP demandée
		const string mapName = Game.RequeteProcess.getOuvreMap();

		localeGameDto = new GameDto(mapName);

		Game.RequeteProcess.setOuvreMapLocaleEtape(CRequeteProcess::OMLE_OUVERTURE_EN_COURS);

		MapLoader::launchLocalGameLoading(localeGameDto);		// Lance l'ouverture de la MAP
	}
	break;

	case CRequeteProcess::OMLE_OUVERTURE_EN_COURS:	// Attente de la fin de l'ouverture de la nouvelle MAP dans la méthode "openMap"
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

		Dirigeable* dirigeable = new Dirigeable();
		map->add(dirigeable);

		// Définition des joueurs
		Game.createPlayerList(localeGameDto->getPlayersMaxNumber());

		CPlayer* erwin = localeGameDto->getErwin();

		if(erwin != NULL) {
			Game.addPlayer(erwin);		// Ajoute le joueur principal à la liste des joueurs
			Game.Erwin(erwin);			// Indique que 'erwin' est le joueur principal
		}

		for(vector<CPlayer*>::iterator iter = localeGameDto->getPlayers().begin() ; iter != localeGameDto->getPlayers().end() ; ++iter) {
			Game.addPlayer(*iter);				// Ajoute le joueur à la liste des joueurs
		}

		CPlayer *player;
		int playerIndex = -1;

		while((player = Game.nextPlayer(playerIndex))) {
			player->initGL();
			player->choiceOneEntryPoint();
		}

		delete localeGameDto;

		// Lancement du jeu en mode local
		Aide = false;
		pFocus->SetPlayFocus();						// Met l'interception des commandes sur le mode jeu
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
	case CRequeteProcess::OMCE_AUCUNE:				// Aucune ouverture de MAP client en cours
		// Nothing to do
		break;

	case CRequeteProcess::OMCE_DEMANDE:
	{
		Fabrique::getAgarView()->showMenuView(Viewer::CONSOLE_VIEW);	// Affichage de la console

		/* *****************************************
		 * Fermeture Map courante
		 * ****************************************/
		//		La fermeture de la Map courante est actuellement faite dans Client.cpp, voir SERVEUR_ACK / JKT_STATUT_CLIENT_DEMJTG
		//		// Fermeture de la MAP courante et destruction des joueurs
		//		Game.quitCurrentMap();
		//
		//		Game.Erwin(NULL);
		//		Game.deletePlayers();


		/* *****************************************
		 * Ouverture Map client
		 * ****************************************/

		// Connexion du client
		Game.setModeClient();

		Interlocutor2* clientInterlocutor = _networkManager->ouvreClient(Config.Reseau.getIpServer(), Config.Reseau.getServerPort(), Config.Reseau.getServerPortTree());

		if(!clientInterlocutor) {
			Game.setModeNull();
			Game.RequeteProcess.setOuvreMapClientEtape(CRequeteProcess::OMCE_AUCUNE);
			AG_TextMsg(AG_MSG_ERROR, "Echec de connexion du client au serveur");
		}
		else {
			// Création de l'arbre des données du client
			ClientDataTree* dataTree = new ClientDataTree(string("jkt"), clientInterlocutor);
			//			DataTreeUtils::formatGameDataTree(dataTree);
			Game.setClientDataTree(dataTree);

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

		while((player = Game.nextPlayer(playerIndex))) {
			player->initGL();
		}

		// Lancement du jeu en mode local
		Aide = false;
		pFocus->SetPlayFocus();						// Met l'interception des commandes sur le mode jeu

		JktNet::CClient *client = Game.getClient();
		client->nomMAP = clientGameDto->getMapName();			// Informe le serveur sur le nom de la MAP lancée
		client->setStatut( JktNet::JKT_STATUT_CLIENT_PLAY );

		delete clientGameDto;

		cout << "\nFINI";
		cout << flush;

		Game.RequeteProcess.setOuvreMapClientEtape(CRequeteProcess::OMCE_AUCUNE);
	}
	break;
	}

	/* **************************************************************
	 * Exécution du workflow d'ouverture d'une MAP en mode serveur
	 * *************************************************************/

	etape = Game.RequeteProcess.getOuvreMapServerEtape();

	switch(etape) {
	case CRequeteProcess::OMSE_AUCUNE:				// Aucune ouverture de MAP locale en cours
		// Nothing to do
		break;

	case CRequeteProcess::OMSE_DEMANDE:
	{
		Fabrique::getAgarView()->showMenuView(Viewer::CONSOLE_VIEW);	// Affichage de la console


		/* *****************************************
		 * Fermeture Map courante
		 * ****************************************/

		// Fermeture de la MAP courante et destruction des joueurs
		Game.quitCurrentMap();

		Game.Erwin(0);
		Game.deletePlayers();


		/* *****************************************
		 * Ouverture Map serveur
		 * ****************************************/

		// Création de l'arbre des données du serveur
		Game.setModeServer();

		_notConnectedServerInterlocutor = _networkManager->ouvreServer(Config.Reseau.getServerPort(), Config.Reseau.getServerPortTree());

		if(!_notConnectedServerInterlocutor) {
			Game.setModeNull();
			Game.RequeteProcess.setOuvreMapServerEtape(CRequeteProcess::OMSE_AUCUNE);
			AG_TextMsg(AG_MSG_ERROR, "Echec de connexion du serveur");
		}
		else {
			// Connexion du serveur
			ServeurDataTree* dataTree = new ServeurDataTree();
			DataTreeUtils::formatGameServerDataTree(dataTree);
			Game.setServerDataTree(dataTree);

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

		// Définition des joueurs
		Game.createPlayerList(serverGameDto->getPlayersMaxNumber());

		CPlayer* erwin = serverGameDto->getErwin();

		if(erwin) {
			Game.addPlayer(erwin);		// Ajoute Erwin à la liste des joueurs
			Game.Erwin(erwin);			// Indique qu'Erwin est le joueur principal
		}

		for(vector<CPlayer*>::iterator iter = serverGameDto->getPlayers().begin() ; iter != serverGameDto->getPlayers().end() ; ++iter) {
			Game.addPlayer(*iter);				// Ajoute le joueur à la liste des joueurs
		}

		CPlayer *player;
		int playerIndex = -1;

		// Initialisation de chacun des joueurs
		while((player = Game.nextPlayer(playerIndex))) {
			player->initGL();
			player->choiceOneEntryPoint();
		}


		// Lancement du jeu en mode local
		Aide = false;
		pFocus->SetPlayFocus();						// Met l'interception des commandes sur le mode jeu

		JktNet::CServer *server = Game.getServer();
		server->nomMAP = serverGameDto->getMapName();					// Informe le serveur sur le nom de la MAP lancée
		server->setStatut( JktNet::JKT_STATUT_SERVER_PLAY );
		server->bGame = true;						// Indique qu'une partie est en cours

		delete serverGameDto;

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

				// Réception des données des clients
				if(Game.getMap()) {			// Si une partie est en cours en mode de jeu client ou serveur
					_networkManager->recoitServer();	// Recoit les données des clients
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

			// Emet les données du joueur actif vers le serveur
			if(Game.getMap() && Game.Erwin() && Game.isModeClient() && (Game.getStatutClient() == JKT_STATUT_CLIENT_PLAY)) {
				client->emet( *Game.Erwin() );		// Emet les requetes et données du joueur actif;
			}
		}
	}
}

/* ***************************************
 * Boucle du jeu prinipale
 * **************************************/
void boucle() {
	Uint32 lastDataTreeUpdate = SDL_GetTicks();

	try {
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

			if(SDL_GetTicks() - lastDataTreeUpdate > 30) {	// Limitation (solution temporaire) des échanges de données pour éviter leur explosion
				// Traite les changements de données côté serveur
				ServeurDataTree* serverDataTree = Game.getServerDataTree();

				Uint32 time;
				saveTime(time);

				if(serverDataTree) {		// TODO serveurDataTree devrait être protégé par un mutex
					serverDataTree->receiveChangementsFromClients();		// Le serveur reçoit les changements des donnée du jeu de la part des clients
					serverDataTree->diffuseChangementsToClients();			// Diffuse les changements des données du jeu aux clients
				}

				// Traite les changements de données côté client
				ClientDataTree* clientDataTree = Game.getClientDataTree();

				if(clientDataTree) {		// TODO clientDataTree devrait être protégé par un mutex
					clientDataTree->receiveChangementsFromServer();		// Le client reçoit les changements des données du jeu de la part du serveur
					clientDataTree->diffuseChangementsToServer();		// Le client reçoit diffuse les changements des données du jeu vers le serveur
				}

				lastDataTreeUpdate = SDL_GetTicks();
			}


			/* **********************************************
			 * Lecture des événements claviers, souris, ...
			 * *********************************************/

			if( Game.getMap()) {	// Si une partie est en cours en mode de jeu local, client ou serveur
				if( 	Game.isModeLocal()
						|| 	(Game.isModeClient() && Game.getStatutClient()==JKT_STATUT_CLIENT_PLAY)
						|| 	(Game.isModeServer() && Game.getStatutServer()==JKT_STATUT_SERVER_PLAY) ) {
					chopeLesEvenements();	// Prends les requêtes du joueur pour permettre même au serveur de jouer
				}
			}


			/* ****************************************************************
			 * Communique
			 * (échange des données client -> serveur ou serveur -> clients)
			 * ****************************************************************/

			communique();


			/* ****************************************************
			 * Effectue les calculs physiques
			 * (gravité, gestion des contacts, déplacements, ...)
			 *
			 * ***************************************************/
			Uint32 beforeCompute = SDL_GetTicks();	// Temps au début du timer (->mesure du temps de calcul)
			Game.timer();
			Uint32 afterCompute = SDL_GetTicks();

			((ConsoleView*)Fabrique::getAgarView()->getView(Viewer::CONSOLE_VIEW))->setDureeCalcules(afterCompute - beforeCompute);


			/* ***********************************************************
			 * Affiche la scène
			 * **********************************************************/

			// Dessine la scène 3D et les menus
			display();

			process_events();	// vérifie les événements
		}
	}
	catch(NotExistingBrancheException& exception) {
		LOGERROR(("Unmanaged NotExistingBrancheException : %s", exception.what()));
	}
	catch(NotExistingValeurException& exception) {
		LOGERROR(("Unmanaged NotExistingValeurException : %s", exception.what()));
	}
}

int main(int argc, char** argv) {
	//	LOGDEBUG(("main(argc=%d,argv=%x)", argc, argv);

	if(argc > 1) {
		// Exécution des tests unitaires
		if(string("test") == argv[1]) {
			cout << endl << "MODE TEST" << endl;
			JktTest::TestSuite* testSuite = new JktTest::TestSuite();

			testSuite->init();
			testSuite->launchTests();
			cout << flush;
			cerr << flush;
			exit(0);
		}
	}

	atexit( quit_JKT );

	string fichier;

	Config.AfficheDateCompilation();		// Affiche la date de compilation du programme
	Config.NommeConfig( nomFichierConfig );	// Nomme le fichier de configuration
	Config.Lit();							// Lit le fichier de configuration

	LOGINFO(("RESUME DE L'INITIALISATION VIDEO"));
	Config.Display.Init();	// Initialisation SDL, OpenGL et Agar

	Config.Reseau.Init();	// Initialisation du réseau
	_networkManager = new NetworkManager();

	Config.Audio.Init();	// Initialisation audio
	Config.Ecrit();			// Enregistre les éventuelles modifications de la configuration

	// Purge les événements SDL, si on ne le fait pas deux fenêtres du jeu s'affichent et les événements sont mal gérés
	SDL_PumpEvents();

	srand(SDL_GetTicks());		// Initialisation de la fonction rand() pour les nombres aléatoires

	HtmlServer htmlServer(Config.Web.getHtmlServerPort());
	htmlServer.open();

	_grahicObjectsToInitializeMutex = SDL_CreateMutex();
	_grahicObjectsToDestructMutex = SDL_CreateMutex();

	LOGINFO(("INFO DIVERSES"));

	// Info réseau
	IPaddress ipaddress;
	if(!SDLNet_ResolveHost( &ipaddress, "localhost", 0 )) {
		const char *host = SDLNet_ResolveIP( &ipaddress );

		if(host)
			LOGINFO(("Nom et adresse de la machine locale : %s / %s", host, IpUtils::translateAddress(ipaddress).c_str()));
		else
			LOGWARN(("Nom de la machine locale : <Inconnu> / %s",IpUtils::translateAddress(ipaddress).c_str()));
	}
	else {
		LOGERROR(("Echec de resolution du nom de la machine locale"));
	}

	Fabrique::construct();

	// Création du démon de gestion des sons
	DemonSons = new CDemonSons();

	// Lancement de l'introduction du jeu
	if(Config.General._playIntro) {
		load_Intro( Config.Display.X, Config.Display.Y );	// Affiche l'introduction du jeu
	}













	FT_Library ft;

	if(FT_Init_FreeType(&ft)) {
		LOGERROR(("Librairie FreeType introuvable"));
	}
	else {
		LOGINFO(("Librairie FreeType initialisée"));
	}

	FT_Face face;

	string fonte = "@Fonte\\Fragmentcore.otf";		// Chargement de la fonte de caractères
	JktUtils::RessourcesLoader::getFileRessource(fonte);

	if(FT_New_Face(ft, fonte.c_str(), 0, &face)) {
		LOGERROR(("Echec d'ouverture de la fonte : %s", fonte.c_str()));
	}
	else {
		LOGINFO(("Fonte chargée : %s", fonte.c_str()));
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	if(FT_Load_Char(face, 'k', FT_LOAD_RENDER)) {
		LOGERROR(("Echec de lecture de X"));
	}
	else {
		LOGINFO(("X lu"));
	}

	glGenTextures(1, &fonteTex);
	glBindTexture(GL_TEXTURE_2D, fonteTex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	FT_GlyphSlot g = face->glyph;

	glTexImage2D(	GL_TEXTURE_2D,
			0,
			GL_RGB,
			g->bitmap.width,
			g->bitmap.rows,
			0,
			GL_LUMINANCE_ALPHA,
			GL_UNSIGNED_BYTE,
			g->bitmap.buffer);


















	// Initialisation de la classe CDlgBoite pour l'IHM
	if( !CDlgBoite::INIT_CLASSE() )
		return 1;	// Erreur fatale si la classe n'a pas pu être initialisée

	// Initialisation de la classe CRocket, réfléchir oû mettre cette initialisation
	if( !CRocket::INIT_CLASSE() )
		return 1;	// Erreur fatale si CRocket ne peut être initialisée

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

	return 1;	// On ne devrait normalement jamais exécuter cette ligne
}

