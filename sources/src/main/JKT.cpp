

/*//////////////////////////////////////////////////////////////////////////////////////////////
					CHOSES A FAIRE

Formaliser les vecteurs position et vitesse de mani�re � ce qu'il ne soit plus
indispensable d'inverser parfois certaines de leurs composantes selon l'utilisation

/*///////////////////////////////////////////////////////////////////////////////////////////////

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
#include <GL/glew.h>
#include <GL/glu.h>
#include <agar/config/have_opengl.h>
#include <agar/config/have_sdl.h>
#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/cursors.h>
#include <agar/gui/sdl.h>
#include <agar/gui/opengl.h>


#include "boost/program_options.hpp"
namespace bpo = boost::program_options;

#include "Constantes.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_net.h"
#include "fmod.h"
#include "fmod_errors.h"	/* optional */
#include "tinyxml.h"
#include "util/XmlHelper.h"

#include "util/GenRef.h"
#include "util/IpUtils.h"
#include "util/math_vectoriel.h"		//Fonctions de calculs vectoriels
#include "util/V3D.h"
#include "util/mathFast.h"
#include "util/Trace.h"					// Gestion du fichier de traces
#include "util/Tableau.cpp"				//Liste cha�n�e
#include "util/TableauIndex.cpp"		//Liste Index�e
#include "util/FindFolder.h"
#include "util/FileUtils.h"
#include "util/Erreur.h"
#include "util/fonte/Fonte.h"
#include "util/TableauIndex.h"
#include "util/V3D.h"
#include "ressource/RessourcesLoader.h"

class CGame;

#include "spatial/widget/GraphicObject.h"
#include "main/Cfg.h"
#include "spatial/materiau/Material.h"
#include "spatial/IfstreamMap.h"
#include "spatial/basic/Geometrical.h"				//Paramêtres des objets g�om�triques
#include "spatial/objet/Dirigeable.h"
#include "spatial/basic/Refreshable.h"
#include "spatial/geo/GeoObject.h"
#include "reseau/web/HttpServer.h"
#include "Rocket.h"
#include "Laser.h"
#include "spatial/objet/Porte.h"
#include "spatial/objet/Navette.h"
#include "main/Clavier.h"							//Requ�tes du clavier
#include "Photo.h"
#include "spatial/materiau/MaterialTexture.h"		//Paramêtres des mat�riaux associ�s aux objets g�om�triques
#include "spatial/materiau/MaterialMulti.h"
#include "spatial/light/Light.h"					//Lumi�res de la map
#include "spatial/Map.h"
#include "son/DemonSons.h"
#include "son/Son.h"
#include "son/ReqSon.h"
#include "reseau/SPA.h"
#include "main/Player.h"
#include "spatial/contact.h"						//Fonctions utilis�es pour le contact
#include "main/divers.h"							//Initialisation d'SDL
#include "son/audio.h"
#include "ihm/DlgBoite.h"
#include "ihm/Portail.h"
#include "ihm/Menu.h"								// Fonctions de gestion du menu
#include "main/Focus.h"								// Gestion du focus
#include "spatial/materiau/Textures.h"
#include "reseau/Client.h"
#include "reseau/Server.h"
#include "main/RequeteProcess.h"
#include "main/Game.h"

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
#include "data/LocalDataTree.h"
#include "data/ServeurDataTree.h"
#include "data/ClientDataTree.h"
#include "data/exception/NotExistingValeurException.h"
#include "data/exception/NotExistingBrancheException.h"

#include "reseau/new/ClientUdpInterlocutor.h"
#include "reseau/new/ServerUdpInterlocutor.h"

#include "test/TestSuite.h"

#include "util/GenRef.h"

#include "jkt.h"

using namespace std;
using namespace jkt;

NotConnectedInterlocutor2* _notConnectedServerInterlocutor = 0;

Fonte fonte;

CCfg Config;		// Contient la configuration du jeu
CGame Game;			// Contient toutes les données vivantes du jeu

const char nomFichierJoueur[] = "@Joueur\\joueurTex";

int JKT_RenderMode = GL_TRIANGLES;		// Rendu normal ou en mode filaire

int damierTextures_nbr_horiz = 5;	// Nombre de textures par page horizontalement
int damierTextures_nbr_vert = 5;	// Nombre de textures par page horizontalement
int damierTextures_page = 0;		// 0=on les affiche pas ; N=affichage la page N des textures

string nomFichierConfig = "config";

CFocus *pFocus;

int numMainPlayer = 0;	// Num�ro du joueur principal dans la MAP (identifie le joueur principal dans la liste des joueurs de la MAP)

bool Aide = false;

extern CDemonSons* demonSons;	// Requ�tes des sons

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

extern bool Aide;					// Indique que le menu d'aide est actif

unsigned int frpsTimer = 0, frpTimer = 0;

CMachin *machin;	// Pour tester le son 3D

NetworkManager* _networkManager;

void gravitePlayer(Uint32 now, CPlayer *player, float deltaTime) {	// Fonction implémentant la gravité aux objets qui doivent la subire
	float vitesse[3];
	player->getVitesse( vitesse );

	vitesse[1] -= GRAVITY_ACCELERATION * deltaTime; // Ajout de gravité au joueur

	player->setVitesse( vitesse );
}

/**
 * Initialise les menu du jeu, affiche le menu principal et place le focus dessus.
 */
void initMenu(void) {
	Aide = false;

	pFocus = new CFocus(play_handle_key_down, CDlg::menu_handle_key_down);

	// Lancement de l'IHM, on entre dans le menu principal du jeu
	pFocus->SetMenuAgarFocus();

	Controller::executeAction(Controller::Action::ShowMenuAction);
}

void updateSon3D() {
	// TEST DU SON 3D
	//	machin->vitesse.Z = 0.0f;
	if( machin->position.Z>1.5f )
		machin->vitesse.Z = -0.05f;
	else if( machin->position.Z<-1.5f )
		machin->vitesse.Z = 0.05f;

	machin->position += machin->vitesse;

	CPlayer *erwin = Game.Erwin();

	float cosP = /*FastCos0( erwin->Phi/180.0f*Pi );*/	cosf( erwin->phi()/180.0f*Pi );
	float sinP = /*FastSin0( erwin->Phi/180.0f*Pi );*/	sinf( erwin->phi()/180.0f*Pi );
	float cosT = /*FastCos0( erwin->Teta/180.0f*Pi );*/	cosf( erwin->teta()/180.0f*Pi );
	float sinT = /*FastSin0( erwin->Teta/180.0f*Pi );*/	sinf( erwin->teta()/180.0f*Pi );

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

	// Position du point d'�coute (le joueur)
	FSOUND_3D_Listener_SetAttributes(pos_erwin, 0, fx,fy,fz,tx,ty,tz);

	// Position de l'�metteur du son
	((CReqSon3D*)machin->req_son)->setPosition( pos_machin );
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

		switch( Game.getStatutClient() ) {
		case JKT_STATUT_CLIENT_INIT:
			str += " (initialisation)";
			break;
		case JKT_STATUT_CLIENT_READY:
			str += " (ready)";
			break;
		case JKT_STATUT_CLIENT_DEMJTG:
			str += " (demande de JTG envoy�e)";
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

		switch( Game.getStatutServer() ) {
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

	float pos = 0;

	fonte.drawString(str, 20.0f, ((float)Config.Display.Y) - 20.0f - pos++*15.0f, INFOFONTESCALAR);

	char cou[70];

	CPlayer *erwin = Game.Erwin();

	// Affiche l'orientation d'Erwin
	if(erwin) {
		// Affiche le Teta d'Erwin
		sprintf( cou, "Teta Phi : %.3d %.3d", (int)erwin->teta(), (int)erwin->phi());
		str = cou;
		fonte.drawString(str, 20.0f, ((float)Config.Display.Y) - 20.0f - pos++*15.0f, INFOFONTESCALAR);

		// Affiche la position d'Erwin
		float position[3];
		erwin->getPosition( position );

		sprintf( cou, "Position : %0.4f %0.4f %0.4f", position[0], position[1], position[2] );
		str = cou;
		fonte.drawString(str, 20.0f, ((float)Config.Display.Y) - 20.0f - pos++*15.0f, INFOFONTESCALAR);
	}

	// Affiche les performances de la gestion du son
	if(Config.Debug.bSonPerformances) {
		unsigned int currentalloced, maxalloced;
		sprintf( cou, "Son, usage CPU : %.4f %%", FSOUND_GetCPUUsage() );
		str = cou;
		fonte.drawString(str, 20.0f, pos++*15.0f+20.0f, INFOFONTESCALAR);

		FSOUND_GetMemoryStats( &currentalloced, &maxalloced );

		sprintf( cou, "Son, memory allocated : %.5u ko", currentalloced/1024);
		str = cou;
		fonte.drawString( str, 20.0f, pos++*15.0f+20.0f, INFOFONTESCALAR);

		sprintf( cou, "Son, max memory allocated : %.5u ko", maxalloced/1024 );
		str = cou;
		fonte.drawString(str, 20.0f, pos++*15.0f+20.0f, INFOFONTESCALAR);
	}

	// Affiche le spectre du son
	if(Config.Debug.bSonSpectre) {
		int NBR_RAIES_SPECTRE = 512;
		float *spectre = FSOUND_DSP_GetSpectrum();

		glDisable( GL_TEXTURE_2D );
		glDepthMask( GL_FALSE );
		glDisable( GL_DEPTH_TEST );
		glEnable( GL_BLEND );

		float largeur = 120;
		float hauteur = 120;
		float gauche = (float)Config.Display.X - largeur - 10;
		float bas = (float)Config.Display.X - hauteur - 10;
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

	if(Game.getMap()) {		// Si il y a une Map a afficher
		CPlayer* erwin = Game.Erwin();

		if(erwin) {		// S'il y a un joueur principal
			float posCamera[3], posPlayer[3];

			erwin->getPosVue(posCamera);
			erwin->getPosition(posPlayer);

			// Recul de la vue du joueur
			glTranslatef(0.0f, 0.0f, erwin->getReculVue());	// Placement du point de vue

			// Orientation de la vue du joueur
			glRotated(erwin->phi(), 1.0, 0.0, 0.0);		// Rotation par rapport au plan horizontal
			glRotated(erwin->teta(), 0.0, 1.0, 0.0);	// Rotation par rapport à l'axe verticale

			// Placement des yeux du joueur
			glTranslatef(-posCamera[0], -posCamera[1], posCamera[2]);

			// Placement du joueur
			glTranslatef(-posPlayer[0], -posPlayer[1], posPlayer[2]);
		}

		glEnable(GL_DEPTH_TEST);

		//		Game.getMap()->AfficheSelection(1.0f, 0.0f, 0.0f);		// Affichage de la map
		Game.getMap()->affiche();		// Affichage de la Map
		Game.afficheProjectils();		// Affichage des projectiles
		Game.afficheDirigeables();		// Affichage des dirigeables
		Game.affichePlayers();			// Affiche des joueurs

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

		// Dessine une cube de 1 m�tre de c�t� dans la Map (sert au rep�rage pour la conception du jeu)
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

		if( Game.Erwin() ) {
			updateSon3D();	// Positionne le joueur et les objets bruyants dans l'espace sonore
		}
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

	// Affichage du damier des textures
	if(damierTextures_page > 0) {
		damierTextures_page = Game.afficheDamierTextures(0, 0, Config.Display.X, Config.Display.Y, damierTextures_page, damierTextures_nbr_horiz, damierTextures_nbr_vert);
	}

	// Affichage du viseur
	if(Game.Erwin()) {
		Game.Erwin()->afficheIconesArmes();
		Game.afficheViseur(Config.Display.X / 2, Config.Display.Y / 2);
		glDepthMask( GL_TRUE );
	}

	// Affichage des infos (mode de jeu, FPS, ...)
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor3f(1.0, 1.0, 1.0 );

	afficheInfo(SDL_GetTicks() - temps);

	// Affichage du vieux système de menus
	if( Aide ) {
		CDlg::afficheMenuActif();
	}

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();


	/*****************************************
	   Affichage des menus Agar
	 *****************************************/

	glEnable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );
	glDisable( GL_DEPTH_TEST );

	glOrtho(0, (double)agDriverSw->w, (double)agDriverSw->h, 0.0, -1.0, 1.0);

	Fabrique::getAgarView()->draw();

	glDisable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );


	/******************************************************
	  Echange des buffers graphique -> affiche � l'�cran.
	 ******************************************************/

	SDL_GL_SwapBuffers();


	/* ****************************************************
	 * Initialisation et destruction des objets graphiques
	 * ****************************************************/

	GraphicObject* object;

	// Initialisation des objets graphiques � initialiser
	SDL_LockMutex(_grahicObjectsToInitializeMutex);

	while (!_grahicObjectsToInitialize.empty()) {
		object = _grahicObjectsToInitialize.back();
		object->initializeGraphicObject();

		_grahicObjectsToInitialize.pop_back();
	}
	SDL_UnlockMutex(_grahicObjectsToInitializeMutex);

	// Descruction des objets graphiques � d�truire
	SDL_LockMutex(_grahicObjectsToDestructMutex);

	while (!_grahicObjectsToDestruct.empty()) {
		object = _grahicObjectsToDestruct.back();
		object->destructGraphicObject();

		_grahicObjectsToDestruct.pop_back();
	}
	SDL_UnlockMutex(_grahicObjectsToDestructMutex);

	((ConsoleView*)Fabrique::getAgarView()->getView(Viewer::CONSOLE_VIEW))->setDureeDisplay(SDL_GetTicks() - temps);
}

void chopeLesEvenements(Uint32 now, float deltaTime) {
	Uint8 *keystate = SDL_GetKeyState(0);		// Récupère les touches clavier appuyées
	Uint8 mouse = SDL_GetMouseState(0, 0);

	if(Game.Erwin()) {
		CPlayer *erwin = Game.Erwin();

		if ( keystate[Config.Commandes.Gauche.key]||(mouse&SDL_BUTTON(Config.Commandes.Gauche.mouse)) )	{	//gauche
			erwin->getClavier()->m_fDroite = -1.0f;
			erwin->getClavier()->m_bIndic = true;
		}

		if( keystate[Config.Commandes.Reculer.key]||(mouse&SDL_BUTTON(Config.Commandes.Reculer.mouse)) ) {	// Reculer
			erwin->getClavier()->m_fAvance = -1.0f;
			erwin->getClavier()->m_bIndic = true;
		}

		if( keystate[Config.Commandes.Avancer.key]||(mouse&SDL_BUTTON(Config.Commandes.Avancer.mouse)) ) {	// Avant
			erwin->getClavier()->m_fAvance = 1.0f;
			erwin->getClavier()->m_bIndic = true;
		}

		if( keystate[Config.Commandes.Droite.key]||(mouse&SDL_BUTTON(Config.Commandes.Droite.mouse)) ) {	// Droite
			erwin->getClavier()->m_fDroite = 1.0f;
			erwin->getClavier()->m_bIndic = true;
		}

		if ( keystate[Config.Commandes.CameraMonter.key]||(mouse&SDL_BUTTON(Config.Commandes.CameraMonter.mouse)) )	{	//gauche
			float posVue[3];
			erwin->getPosVue(posVue);
			posVue[1] += 0.003f;
			erwin->setPosVue(posVue);
		}

		if ( keystate[Config.Commandes.CameraDescendre.key]||(mouse&SDL_BUTTON(Config.Commandes.CameraDescendre.mouse)) )	{	//gauche
			float posVue[3];
			erwin->getPosVue(posVue);
			posVue[1] -= 0.003f;
			erwin->setPosVue(posVue);
		}

		if ( keystate[Config.Commandes.CameraDroite.key]||(mouse&SDL_BUTTON(Config.Commandes.CameraDroite.mouse)) )	{	//gauche
			float posVue[3];
			erwin->getPosVue(posVue);
			posVue[0] += 0.003f;
			erwin->setPosVue(posVue);
		}

		if ( keystate[Config.Commandes.CameraGauche.key]||(mouse&SDL_BUTTON(Config.Commandes.CameraGauche.mouse)) )	{	//gauche
			float posVue[3];
			erwin->getPosVue(posVue);
			posVue[0] -= 0.003f;
			erwin->setPosVue(posVue);
		}

		if ( keystate[Config.Commandes.CameraReculer.key]||(mouse&SDL_BUTTON(Config.Commandes.CameraReculer.mouse)) )	{	//gauche
			float posVue[3];
			erwin->getPosVue(posVue);
			posVue[2] -= 0.003f;
			erwin->setPosVue(posVue);
		}

		if ( keystate[Config.Commandes.CameraAvancer.key]||(mouse&SDL_BUTTON(Config.Commandes.CameraAvancer.mouse)) )	{	//gauche
			float posVue[3];
			erwin->getPosVue(posVue);
			posVue[2] += 0.003f;
			erwin->setPosVue(posVue);
		}

		// Recul de vue (reculer plus)
		if ( keystate[Config.Commandes.CameraAvancer.key]||(mouse&SDL_BUTTON(Config.Commandes.CameraAvancer.mouse)) )	{
			float posVue[3];
			erwin->getPosVue(posVue);
			posVue[2] += 0.003f;
			erwin->setPosVue(posVue);
		}

		if( keystate[Config.Commandes.Tir1.key]||(mouse&SDL_BUTTON(Config.Commandes.Tir1.mouse)) ) {	// Tire avec l'arme sélectionnée
			erwin->tir();		// Valide un tir
		}

		if( keystate[Config.Commandes.Monter.key]||(mouse&SDL_BUTTON(Config.Commandes.Monter.mouse)) ) {	// Monter
			erwin->getClavier()->m_fMonte = 1.0;
			erwin->getClavier()->m_bIndic = true;
		}

		if( keystate[Config.Commandes.Tir2.key]||(mouse&SDL_BUTTON(Config.Commandes.Tir2.mouse)) ) {	// Tire de projectile qui rebondit
			CPlayer *balle;

			float cosTeta = /*FastCos0( erwin->Teta/180.0f*Pi );	//*/cosf( erwin->teta()/180.0f*Pi );
			float sinTeta = /*FastSin0( erwin->Teta/180.0f*Pi );	//*/sinf( erwin->teta()/180.0f*Pi );
			float cosPhi = /*FastCos0( erwin->Phi/180.0f*Pi );	//*/cosf( erwin->phi()/180.0f*Pi );
			float sinPhi = /*FastSin0( erwin->Phi/180.0f*Pi );	//*/sinf( erwin->phi()/180.0f*Pi );

			balle = new CPlayer(); //cr�e un autre joueur
			float vect[3];
			erwin->getPosition( vect );
			balle->setPosition( vect[0], vect[1], vect[2]);		// positionne le projectile sur la map

			erwin->getVitesse( vect );
			float projectilLaunchSpeed = deltaTime * PROJECTIL_LAUNCH_SPEED;
			balle->changeVitesse( 	sinTeta*cosPhi*projectilLaunchSpeed + vect[0], sinPhi*projectilLaunchSpeed + vect[1], cosTeta*projectilLaunchSpeed + vect[2]);

			balle->changeAction( gravitePlayer );	// associe au projectile une fonction de gravité
			balle->changeContact( contactSprite );	// associe une fonction pour les contacts avec la map

			Game.addPlayer(balle);			// ajoute le projectile � la liste des joueurs
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
	case SDL_MOUSEMOTION:		// Si c'est un �v�nement 'souris'
		if( erwin ) {
			// Rotation par rapport au plan horizontal (regarder en l'air ou vers le bas)
			erwin->phi( erwin->phi() + event->motion.yrel );

			if(erwin->phi() > 90.0f) {
				erwin->phi( 90.0f );
			}

			if(erwin->phi() < -90.0f) {
				erwin->phi( -90.0f );
			}

			// Rotation par rapport à l'axe verticale (regarder à droite ou à gauche)
			erwin->teta( erwin->teta() + event->motion.xrel );

			if(erwin->teta() > 180.0f) {
				erwin->teta(erwin->teta() - 360.0f);
			}

			if(erwin->teta() < -180.0f) {
				erwin->teta(erwin->teta() + 360.0f);
			}
		}
		break;

	case SDL_MOUSEBUTTONDOWN:
		if( erwin ) {
			unsigned char mouseButtonDown = event->button.button;

			// Tir de l'arme active
			if(mouseButtonDown == Config.Commandes.Tir1.mouse) {
				erwin->tir();	// Tir avec l'arme active
			}

			// Sélection arme suivante
			if(mouseButtonDown == Config.Commandes.SelectWeaponUp.mouse) {
				erwin->armeUp();
			}

			// Sélection arme précédente
			if(mouseButtonDown == Config.Commandes.SelectWeaponDown.mouse) {
				erwin->armeDown();
			}

			// Recul de vue (reculer plus)
			if(mouseButtonDown == Config.Commandes.VueReculer.mouse)	{
				erwin->setReculVue(erwin->getReculVue() + 0.03f);
			}

			// Recul de vue (reculer moins)
			if(mouseButtonDown == Config.Commandes.VueAvancer.mouse)	{
				erwin->setReculVue(erwin->getReculVue() - 0.03f);
			}

			// Désactive / active la gravité
			if(mouseButtonDown == Config.Commandes.Gravity.mouse) {
				if( Game.getMap() )
					Game.setGravite( !Game.getGravite() );
			}

			// Affiche / masque le damier des textures
			if(mouseButtonDown == Config.Commandes.Textures.mouse) {
				// Affichage la page suivante des textures du jeu
				// Si la page d�passe le nombre max, l'affiche la remettra � 0
				damierTextures_page++;
			}
		}
		break;

	case SDL_KEYDOWN: {
		SDLKey keyDown = event->key.keysym.sym;

		if( erwin ) {
			// Tir de l'arme active
			if(keyDown == Config.Commandes.Tir1.key) {
				erwin->tir();
			}

			// Sélection arme suivante
			if(keyDown == Config.Commandes.SelectWeaponUp.key) {
				erwin->armeUp();
			}

			// Sélection arme précédente
			if(keyDown == Config.Commandes.SelectWeaponDown.key) {
				erwin->armeDown();
			}

			// Recul de vue (reculer plus)
			if(keyDown == Config.Commandes.VueReculer.key)	{
				erwin->setReculVue(erwin->getReculVue() + 0.03f);
			}

			// Recul de vue (reculer moins)
			if(keyDown == Config.Commandes.VueAvancer.key)	{
				erwin->setReculVue(erwin->getReculVue() - 0.03f);
			}

			// Désactive / active la gravité
			if(keyDown == Config.Commandes.Gravity.key) {
				if( Game.getMap() )
					Game.setGravite( !Game.getGravite() );
			}

			// Affiche / masque le damier des textures
			if(keyDown == Config.Commandes.Textures.key) {
				damierTextures_page++;
			}
		}

		switch(keyDown) {
		case SDLK_F1 :
			lanceMenuAide( 0 );
			break;

		case SDLK_F2:
			lanceMenuPrinc( 0 );
			break;

			// Affiche les erreurs
		case SDLK_F3 : {
			LOGINFO(("Derniere erreur FMOD : %s", FMOD_ErrorString(FSOUND_GetError())));
			LOGINFO(("Derniere erreur SDL : %s", SDL_GetError()));
			LOGINFO(("Derniere erreur SDL_Net : %s", SDLNet_GetError()));
			LOGINFO(("Derniere erreur openGL : %s", (const char*)gluErrorString( glGetError() )));
			LOGINFO(("Derniere erreur Agar : %s", AG_GetError()));
		}
		break;

			// Place le focus sur le menu
		case SDLK_F5:
			pFocus->SwitchPlayOrConsoleFocus();
			break;

			// Force la rotation du fichier de logs
		case SDLK_F6:
			Trace::instance().rotate();
			break;

			// Annule la vitesse du joueur actif
		case SDLK_v :
			if( Game.getMap() )
				Game.Erwin()->changeVitesse(0.0f, 0.0f, 0.0f);
			break;

			// Sauve le fichier map actue
		case SDLK_RETURN :
			if(Game.getMap()) {
				string saveMapFilename = "map-" + FileUtils::horodatage();
				Game.getMap()->Save(saveMapFilename);

				ConsoleView* console = ((ConsoleView*)Fabrique::getAgarView()->getView(Viewer::CONSOLE_VIEW));
				console->println(ConsoleView::COT_INFO, string("Map enregistr\u00e9e : ") + saveMapFilename);
			}
			break;

			// Demande la prise d'une photo au prochain display
		case SDLK_p :
			Game.RequeteProcess.setTakePicture();
			break;

			// Change de mode de rendu (normal ou filaire)
		case SDLK_m :
			if( JKT_RenderMode==GL_TRIANGLES )
				JKT_RenderMode = GL_LINE_STRIP;
			else
				JKT_RenderMode = GL_TRIANGLES;
			break;

			// Retour du joueur � la position d'origine avec vitesse nulle
		case SDLK_o :
			if(Game.Erwin()) {
				Game.Erwin()->choiceOneEntryPoint();
				Game.Erwin()->changeVitesse( 0.0f, 0.0f, 0.0f );
				Game.Erwin()->teta( 0.0 );
				Game.Erwin()->phi( 0.0 );
			}
			break;

		case SDLK_n: {
			set<CSon*>::iterator p;
			cout << endl << "Bilan des sons :";
			cout << endl << "----------------";
			int i=1;

			for( p=demonSons->m_TabSon.begin() ; p!=demonSons->m_TabSon.end() ; p++ ) {
				CSon* son = *p;
				cout << endl << i++ << "\t" << son->nom << "\t" << (unsigned int)son->m_TabReq.size();
			}

			cout << endl;
		}
		break;

		case SDLK_c :	// Change le joueur principal (=> change le point de vue et l'interraction clavier)
			cout << endl << "Nombre de joeurs dans la partie : " << Game.getNbrPlayers();

			if(Game.getMap()) {
				CPlayer* erwin = Game.nextPlayer(numMainPlayer);	// Lit le num�ro du joueur suivant

				if(!erwin) {				// Si on a atteint la fin de la liste alors prend � nouveau le premier de liste
					erwin = Game.nextPlayer(numMainPlayer);
				}

				// S�lectionne le joueur en tant que joueur principal
				Game.setErwin(erwin);
				((ConsoleView*)Fabrique::getAgarView()->getView(Viewer::CONSOLE_VIEW))->setActivePlayerName(erwin->getName());
			}
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

	while( SDL_PollEvent( &sdlevent ) ) {

		/* ****************************************** */
		/* Gestion événements prioritaires            */
		/* ****************************************** */

		switch( sdlevent.type ) {
		case SDL_QUIT:
			quit_game("SDL quit event", 0 );
			break;


		case SDL_KEYDOWN:
			// Bascule entre le menu Agar et le jeu
			if(sdlevent.key.keysym.sym == SDLK_ESCAPE) {
				if(pFocus->isPlayFocus()) {
					Controller::executeAction(Controller::Action::ShowMenuAction);

					pFocus->SetMenuAgarFocus();
				}
				else {
					Controller::executeAction(Controller::Action::HideMenuAction);
					pFocus->SetPlayFocus();
				}
			}
			// Affiche la configuration avancée
			else if(sdlevent.key.keysym.sym == SDLK_F2) {
				Controller::executeAction(Controller::Action::ShowConfigurationAdvancedViewAction);
				pFocus->SetMenuAgarFocus();
			}
			break;

		default:
			break;
		}


		/* ****************************************** */
		/* Gestion événements du jeu                  */
		/* ****************************************** */

		pFocus->ExecFocus( &sdlevent );
	}

	AG_WindowProcessQueued();
}

bool deprecatedOpenMAP(const void *nomFichier) {
	/**************************************
	 * Ouverture de la Map proprement dite
	 **************************************/

	string nomFichierMap = (char*)nomFichier;

	if( !Game.openMap( nomFichierMap ) ) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur � l'ouverture du fichier Map : " << nomFichierMap;
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

	CMap *pMapJoueur = new CMap(0);
	pMapJoueur->Lit(mapJoueurPrincipal, 0);
	pMapJoueur->echangeXZ();					// Ajuste les coordonnées
	pMapJoueur->scale( -0.06f, 0.06f, 0.06f );
	pMapJoueur->init();

	CMap *pMapJoueur2 = new CMap(0);
	pMapJoueur2->Lit(nomFichierJoueur, 0);
	pMapJoueur2->echangeXZ();					// Ajuste les coordonnées
	pMapJoueur2->scale( -0.06f, 0.06f, 0.06f );
	pMapJoueur2->init();

	cout << endl;


	/**************************************
	 * Chargement de sons pour les joueurs
	 **************************************/

	// R�cup�ration des ressources de cris des personnages
	string cri1 = "@Bruit\\cri_1.wav";
	jkt::RessourcesLoader::getFileRessource(cri1);

	string cri2 = "@Bruit\\cri_1.wav";
	jkt::RessourcesLoader::getFileRessource(cri2);


	/***********************************
	 * Création des joueurs dans la Map
	 ***********************************/

	// Création du joueur principal
	CPlayer *erwin = new CPlayer();				// Cr�e le joueur principal (celui géré par le clavier et l'�cran)
	erwin->changeAction( gravitePlayer );		// Associe au joueur une fonction de gravité
	erwin->changeContact( contactPlayer );		// Associe une fonction de gestion des contacts avec la map
	erwin->Skin( pMapJoueur );
	erwin->setCri( cri1.c_str() );				// Cri du joueur
	erwin->setName( "ERWIN" );
	erwin->init();								// Initialise certaines données
	erwin->choiceOneEntryPoint();
	Game.addPlayer(erwin);	// Ajoute le joueur principal � la liste des joueurs
	Game.setErwin(erwin);						// Indique que 'erwin' est le joueur principal

	// Création d'un second joueur
	CPlayer *julien;
	julien = new CPlayer();						// Crée un autre joueur
	julien->changeAction( gravitePlayer );		// Associe au joueur une fonction de gravité
	julien->changeContact( contactPlayer );		// Associe une fonction pour les contacts avec la map
	julien->Skin( pMapJoueur2 );
	julien->setCri( cri1.c_str() );
	julien->setName( "JULIEN" );
	julien->init();
	julien->choiceOneEntryPoint();
	Game.addPlayer(julien);	// Ajoute le joueur à la liste des joueurs

	// Création d'un troisi�me joueur
	CPlayer *sprite;
	sprite = new CPlayer();						// Crée un autre joueur
	sprite->changeAction( gravitePlayer );		// Associe au joueur une fonction de gravité
	sprite->changeContact( contactSprite );		// Associe une fonction pour les contacts avec la map
	sprite->Skin( pMapJoueur );
	sprite->setCri( cri2.c_str() );
	sprite->setName( "SPRITE" );
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
	//	 * exécution de l'ouverture d'une MAP en mode multijoueurs
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
	 * exécution du workflow d'ouverture d'une MAP locale
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

		Game.setErwin(0);
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
		Game.changeActiveMap(map);
		map->initPlugins();
		map->initGL();

		Dirigeable* dirigeable = new Dirigeable(map);
		map->add(dirigeable);

		// Définition des joueurs
		Game.createPlayerList(localeGameDto->getPlayersMaxNumber());

		CPlayer* erwin = localeGameDto->getErwin();

		if(erwin != NULL) {
			Game.addPlayer(erwin);		// Ajoute le joueur principal � la liste des joueurs
			Game.setErwin(erwin);			// Indique que 'erwin' est le joueur principal
		}

		for(vector<CPlayer*>::iterator iter = localeGameDto->getPlayers().begin() ; iter != localeGameDto->getPlayers().end() ; ++iter) {
			Game.addPlayer(*iter);				// Ajoute le joueur � la liste des joueurs
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
		Controller::executeAction(Controller::Action::HideMenuAction);
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
		Game.changeActiveMap(map);
		map->initPlugins();
		map->initGL();

		// Activation de chacun des joueurs
		CPlayer *player;
		int playerIndex = -1;

		while((player = Game.nextPlayer(playerIndex))) {
			player->initGL();
		}

		// Lancement du jeu en mode local
		Aide = false;
		pFocus->SetPlayFocus();						// Met l'interception des commandes sur le mode jeu
		Controller::executeAction(Controller::Action::HideMenuAction);

		jkt::CClient *client = Game.getClient();
		client->nomMAP = clientGameDto->getMapName();			// Informe le serveur sur le nom de la MAP lancée
		client->setStatut( jkt::JKT_STATUT_CLIENT_PLAY );

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

		Game.setErwin(0);
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

		Game.changeActiveMap(map);
		map->initPlugins();
		map->initGL();

		// D�finition des joueurs
		Game.createPlayerList(serverGameDto->getPlayersMaxNumber());

		CPlayer* erwin = serverGameDto->getErwin();

		if(erwin) {
			Game.addPlayer(erwin);		// Ajoute Erwin à la liste des joueurs
			Game.setErwin(erwin);		// Indique qu'Erwin est le joueur principal
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
		Controller::executeAction(Controller::Action::HideMenuAction);

		jkt::CServer *server = Game.getServer();
		server->nomMAP = serverGameDto->getMapName();					// Informe le serveur sur le nom de la MAP lancée
		server->setStatut( jkt::JKT_STATUT_SERVER_PLAY );
		server->bGame = true;						// Indique qu'une partie est en cours

		delete serverGameDto;

		Game.RequeteProcess.setOuvreMapServerEtape(CRequeteProcess::OMSE_AUCUNE);
	}
	break;
	}


	/* *******************************************
	 * exécution d'une capture d'�cran
	 * ******************************************/

	if( Game.RequeteProcess.isTakePicture() ) {	// S'il y a une demande de prise de photo de la scène
		CPhoto photo( Config.Display.X, Config.Display.Y );

		// Nom du fichier avec horodatage
		string fichierPhoto = "./photos/photo_" + FileUtils::horodatage() + ".bmp";

		ConsoleView* console = ((ConsoleView*)Fabrique::getAgarView()->getView(Viewer::CONSOLE_VIEW));


		// Prends la photo
		if( photo.save(fichierPhoto) ) {
			LOGINFO(("Photo prise '%'", fichierPhoto.c_str()));
			console->println(ConsoleView::COT_INFO, string("Photo enregistr\u00e9e : ") + fichierPhoto);
		}
		else {
			LOGINFO(("Echec de la photo '%'", fichierPhoto.c_str()));
			console->println(ConsoleView::COT_ERROR, "Echec photo");
		}
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
				if(Game.getMap()) {						// Si une partie est en cours en mode de jeu client ou serveur
					_networkManager->recoitServer();	// Recoit les données des clients
				}
			}
		}


		/* *******************************************
		 * Le client émet et réceptionne des données
		 * ******************************************/

		// Le client r�ceptionne les données du serveur
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
	Uint32 now = SDL_GetTicks();
	SDL_Delay(10);	// On évite la division par zéro qui serait due à une durée nulle (deltaTime)

	Uint32 lastDataTreeUpdate = now;
	Uint32 newTime, varTime;
	float deltaTime = 0;

	try {
		for( ;; ) {
			newTime = SDL_GetTicks();
			varTime = newTime - now;

			if(varTime < 10) {	// Pas moins de 10ms pour un cycle calcules et affichage (ça fait 100 images par secondes !)
				SDL_Delay(10-varTime);
				newTime = SDL_GetTicks();
			}

			deltaTime = float(newTime - now) / 1000.0f;	// Nombre de secondes depuis la dernière exécution de la boucle
			now = newTime;

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
					serverDataTree->receiveChangementsFromClients();		// Le serveur reçoit les changements des données du jeu de la part des clients
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
				if( 	Game.isModeLocal() || 	(Game.isModeClient() && Game.getStatutClient()==JKT_STATUT_CLIENT_PLAY) || 	(Game.isModeServer() && Game.getStatutServer()==JKT_STATUT_SERVER_PLAY) ) {
					chopeLesEvenements(now, deltaTime);	// Prends les requêtes du joueur pour permettre même au serveur de jouer
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
			Uint32 beforeTimer = SDL_GetTicks();	// Temps au début du timer (->mesure du temps de calcul)
			Game.timer(now, deltaTime);
			Uint32 afterTimer = SDL_GetTicks();

			((ConsoleView*)Fabrique::getAgarView()->getView(Viewer::CONSOLE_VIEW))->setDureeCalcules(afterTimer - beforeTimer);


			/* ***********************************************************
			 * Affiche la scène
			 * **********************************************************/

			// Dessine la scène 3D et les menus
			display();

			process_events();	// Vérifie les événements
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
	atexit( quit_JKT );


	/* ********************************************** */
	/* Parse command line parameters                  */
	/* ********************************************** */

	string mapFile;

	bpo::options_description desc("Allowed options");
	desc.add_options()
						// First parameter describes option name/short name
						// The second is parameter to option
						// The third is description
						("help,h", "print usage message")
						("test,t", "execute unit tests")
						("openLocal,o", bpo::value(&mapFile), "open the specified local map");

	bpo::variables_map cmdParameters;

	try {
		bpo::store(bpo::parse_command_line(argc, argv, desc), cmdParameters);
		bpo::notify(cmdParameters);
	}
	catch (bpo::error& error) {
		cerr << endl << "Error: " << error.what() << endl;
		cout << endl << desc << endl << flush;

		exit(0);
	}

	// Affiche aide ligne de commandes
	if(cmdParameters.count("help")) {
		cout << desc << endl << flush;
		exit(0);
	}
	// Execute les tests unitaires
	else if (cmdParameters.count("test")) {		// Execute unit tests
		cout << endl << "MODE TEST" << endl;
		jkt::TestSuite* testSuite = new jkt::TestSuite();

		testSuite->init();
		testSuite->launchTests();
		cout << flush;
		cerr << flush;
		exit(0);
	}
	// Ouvre en local la Map spécifiée
	else if(cmdParameters.count("openLocal")) {
		string mapName = cmdParameters["openLocal"].as<string>();

		LOGINFO(("Ouverture map locale en ligne de commandes '%s'", mapName.c_str()));

		Game.RequeteProcess.setOuvreMapLocal(mapName);
	}


	/* ********************************************** */
	/* Lecture configuration                          */
	/* ********************************************** */

	LOGINFO(("LECTURE CONFIGURATION"));

	Config.Reseau.Init();	// Initialisation du réseau

	// Détection de la machine
	string host;
	IPaddress ipaddress;

	if(!SDLNet_ResolveHost( &ipaddress, "localhost", 0 )) {
		const char* hostChar = SDLNet_ResolveIP( &ipaddress );

		if(hostChar) {
			LOGINFO(("Nom et adresse de la machine locale : %s / %s", hostChar, IpUtils::translateAddress(ipaddress).c_str()));
			host = hostChar;
		}
		else {
			LOGWARN(("Nom de la machine locale : <Inconnu> / %s",IpUtils::translateAddress(ipaddress).c_str()));
			host = "";
		}
	}
	else {
		LOGERROR(("Echec de resolution du nom de la machine locale"));
		host = "";
	}

	// Lecture fichier configuration
	Config.AfficheDateCompilation();				// Affiche la date de compilation du programme
	Config.NommeConfig(nomFichierConfig, host);		// Nomme le fichier de configuration
	Config.Lit();									// Lit le fichier de configuration


	/* ********************************************** */
	/* Initialisations (vidéo, réseau, ...)           */
	/* ********************************************** */

	LOGINFO(("RESUME DE L'INITIALISATION VIDEO"));
	Config.Display.Init();	// Initialisation SDL, OpenGL et Agar

	_networkManager = new NetworkManager();

	Config.Audio.Init();	// Initialisation audio
	Config.Ecrit();			// Enregistre les éventuelles modifications de la configuration

	// Purge les événements SDL, si on ne le fait pas deux fenêtres du jeu s'affichent et les événements sont mal gérés
	SDL_PumpEvents();

	srand(SDL_GetTicks());		// Initialisation de la fonction rand() pour les nombres aléatoires

	HttpServer htmlServer(Config.Web.getHtmlServerPort());
	htmlServer.open();

	_grahicObjectsToInitializeMutex = SDL_CreateMutex();
	_grahicObjectsToDestructMutex = SDL_CreateMutex();



	Fabrique::construct();

	// Création du démon de gestion des sons
	demonSons = new CDemonSons();

	// Lancement de l'introduction du jeu
	if(Config.General._playIntro) {
		load_Intro( Config.Display.X, Config.Display.Y );	// Affiche l'introduction du jeu
	}

	// Lecture fonte
	string fonteFile = "@Fonte\\Mermaid1001.ttf";			// Chargement de la fonte de caractères
	jkt::RessourcesLoader::getFileRessource(fonteFile);

	fonte.load(fonteFile, 48);


	// Initialisation de la classe CDlgBoite pour l'IHM
	if( !CDlgBoite::INIT_CLASSE() )
		return 1;	// Erreur fatale si la classe n'a pas pu être initialisée

	// Initialisation de la classe CRocket, réfléchir où mettre cette initialisation
	if( !CRocket::INIT_CLASSE() )
		return 1;	// Erreur fatale si CRocket ne peut être initialisée

	// Délai d'attente après l'intro du jeu, je sais plus à quoi il sert
	SDL_Delay( 1000 );

	// Création d'un haut parleur qui se déplace pour tester le son 3D
	machin = new CMachin();
	string fichierSon3D = "@Musique\\drumloop.wav";
	jkt::RessourcesLoader::getFileRessource(fichierSon3D);
	CSon3D *son3D = demonSons->createSon3D( fichierSon3D.c_str() );
	CReqSon *reqSon = demonSons->playID( (CSon*)son3D, true );
	reqSon->boucle( true );
	machin->req_son = reqSon;
	demonSons->play( machin->req_son );

	// Initialisation des plugins démarrés par défaut
	Fabrique::getPluginEngine()->activateDefaultGlobalPlugins();

	initMenu();

	// Entr�e dans la boucle principale du jeu
	Fabrique::getAgarView()->showView(Viewer::CONSOLE_VIEW);
	boucle();

	return 1;	// On ne devrait normalement jamais exécuter cette ligne
}

