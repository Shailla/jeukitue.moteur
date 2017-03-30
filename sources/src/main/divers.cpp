
#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/glu.h>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>

#include <agar/core.h>
#include "SDL.h"
#include "fmod.h"
#include "fmod_errors.h"
#include "SDL_image.h"
#include "SDL_net.h"
#include "util/fonte/Fonte.h"

#include "ressource/RessourcesLoader.h"
#include "util/Trace.h"
#include "util/utils_SDL.h"
#include "son/DemonSons.h"
#include "son/Son.h"
#include "spatial/materiau/Textures.h"
#include "main/Cfg.h"

#include "main/divers.h"

using namespace std;
using namespace jkt;

extern jkt::CDemonSons *DemonSons;

void quit_game(const char* txt, int code)	// Quitte proprement le jeu
{
LOGINFO(("quit_game(code=%d,txt=%s)", code, txt));
	cerr << endl << __FILE__ << ":" << __LINE__ << " Quit game with code and message : " << code << " - '" << txt << "'" << flush;
	exit( code );
}

void quit_game(const string& txt, int code)	// Quitte proprement le jeu
{
LOGINFO(("quit_game(code=%d,txt=%s)", code, txt.c_str()));
	cerr << endl << __FILE__ << ":" << __LINE__ << " Quit game with code and message : " << code << " - '" << txt << "'" << flush;
	exit( code );
}

void quit_JKT() {
	LOGINFO(("quit_JKT()"));

	string trace = "Denière erreurs des librairies :";

	trace = "\n - FMOD : ";
	trace += FMOD_ErrorString(FSOUND_GetError());

	trace = "\n - SDL : ";
	trace += SDL_GetError();

	trace = "\n - SDL_Net : ";
	trace += SDLNet_GetError();

	trace = "\n - openGL : ";
	trace += (char*)gluErrorString(glGetError());

	trace = "\n - Agar : ";
	const char* agarError = AG_GetError();

	if(agarError != NULL) {
		trace += agarError;
	}
	else {
		trace += "Aucune";
	}

	LOGINFO((trace.c_str()));

	cerr << endl << __FILE__ << ":" << __LINE__ << endl << trace << flush;

	FSOUND_Close();		// Fermeture d'FMOD
	SDLNet_Quit();		// Fermeture d'SDL_Net
	SDL_Quit();			// Fermeture de SDL
}

bool checkEventsForIntro(void)		// Vérifie si 'escape' ou le bouton souris ont été frappés
{									// true si c'est le cas, false sinon
    SDL_Event event;

    while( SDL_PollEvent( &event ) ) {
        switch( event.type ) {
 		case SDL_MOUSEBUTTONDOWN:	// Handle mouse button presses
			if( event.button.button==SDL_BUTTON_LEFT )			// Si le bouton de gauche
				if( event.button.type==SDL_MOUSEBUTTONDOWN )	// est préssé
					return true;

			break;

		case SDL_KEYDOWN:			// Handle key presses
			if( event.key.keysym.sym==SDLK_ESCAPE || event.key.keysym.sym==SDLK_RETURN )
				return true;

			break;

        case SDL_QUIT:
			quit_game("SDL quit event in intro", 0);
			break;

		default:
			break;
        }
    }
	return false;
}

CSon* sonChariot;		// Son retour chariot machine à écrire
CSon* sonTouche;		// Son frappe d'une touche clavier
CSon* sonEspace;		// Son frappe de la touche espace clavier
CSon* sonHurlement;		// Son hurlement du sauveur de la planète


void load_Intro( int width, int height ) {
	LOGDEBUG(("load_Intro(width=%d,height=%d)", width, height));

	// Son retour chariot machine à écrire
	string bruitChariot = "@Bruit\\chariot.wav";
	jkt::RessourcesLoader::getFileRessource(bruitChariot);
	sonChariot = DemonSons->CreateSon( bruitChariot.c_str() );

	// Son frappe d'une touche clavier
	string bruitTouche = "@Bruit\\touche.wav";
	jkt::RessourcesLoader::getFileRessource(bruitTouche);
	sonTouche = DemonSons->CreateSon( bruitTouche.c_str() );

	// Son frappe de la touche espace clavier
	string bruitEspace = "@Bruit\\espace.wav";
	jkt::RessourcesLoader::getFileRessource(bruitEspace);
	sonEspace = DemonSons->CreateSon( bruitEspace.c_str() );

	// Son hurlement du sauveur de la planète
	string bruitHurlement = "@Bruit\\hurlement.wav";
	jkt::RessourcesLoader::getFileRessource(bruitHurlement);
	sonHurlement = DemonSons->CreateSon( bruitHurlement.c_str() );

	load_IntroSub( width, height);

	DemonSons->Delete(sonChariot);		// Son retour chariot machine à écrire
	DemonSons->Delete(sonTouche);		// Son frappe d'une touche clavier
	DemonSons->Delete(sonEspace);		// Son frappe de la touche espace clavier
	DemonSons->Delete(sonHurlement);	// Son hurlement du sauveur de la planète
}


void load_IntroSub(const int width, const int height) {
	LOGDEBUG(("load_IntroSub(width=%d,height=%d)", width, height));

	// Chargement de la fonte de caractères
	string fileFonteIntro = "@Fonte\\Mermaid1001.ttf";
	jkt::RessourcesLoader::getFileRessource(fileFonteIntro);

	Fonte fonteIntro;
	fonteIntro.load(fileFonteIntro, 24);

	string str1 = "Nous sommes en 2056.\n\nLa surface de la Terre n'est plus qu'un oc\351an d'acide.\nLa loi a laiss\351 sa place a celle du plus fort et tout se r\350gle\nd\351sormais lors de combats sans gland.\n\n      Voici le seul homme qui peut encore sauver la plan\350te... ";
	string str2 = "Alors, vous l'avez compris...\n   On est vraiment dans la merde !";
	vector< string > lignes;		// Lignes séparées par un retour chariot

	srand( SDL_GetTicks() );	// Initialisation de la fonction rand() pour les nombres aléatoires

	string introJKT = "@Fond\\intro_JKT2.jpg";
	jkt::RessourcesLoader::getFileRessource(introJKT);
	SDL_Surface *image1 = IMG_Load(introJKT.c_str());		// Lit le fichier image

	if(image1 == 0) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur : Ouverture d'image (" << introJKT << ")" << endl;
		return;
	}

	Uint8 *image2 = new Uint8[width*height*3];

	SDL_LockSurface( image1 );
	gluScaleImage( GL_RGB, image1->w, image1->h, GL_UNSIGNED_BYTE, image1->pixels, width, height, GL_UNSIGNED_BYTE, image2 );
	SDL_UnlockSurface( image1 );
	SDL_FreeSurface( image1 );

	glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
	gluOrtho2D(0.0, width, 0.0, height );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();


	glEnable(GL_TEXTURE_2D);
	glDisable( GL_DEPTH_TEST );

	vector< string >::const_iterator iter;
	float vertical;
	wchar_t lettre;


	/* ***********************************************
	 * AFFICHAGE D'UN TEXTE D'INTRO
	 * lettre par lettre (genre machine à écrire)
	 * ***********************************************/

	glColor3f( 1.0, 1.0, 1.0 );

	for( unsigned int i=0 ; i<str1.length() ; i++ ) {
		lettre = str1[ i ];

		glClear( GL_COLOR_BUFFER_BIT );

		if( i==0 )
			lignes.push_back( "" );

		if( (lettre=='\n') ) {	// Si on a affaire à un passage à la ligne
			DemonSons->Play( sonChariot );	// Envoie le son retour chariot
			lignes.push_back( "" );			// Et passe à la ligne
			SDL_Delay( 700 );
		}
		else {
			if( lettre!= ' ' )	// Si c'est pas un espace
				DemonSons->Play( sonTouche );	// Envoie le son pour une frappe de touche normale
			else
				DemonSons->Play( sonEspace );	// Sinon le son d'un espace

			SDL_Delay( 45 );
			*lignes.rbegin() += lettre ; // Ajoute la lettre à la fin de la dernière ligne
		}

		// Affichage du texte
		vertical = height - 20.0f;
		for( iter=lignes.begin() ; iter!=lignes.end() ; iter++ ) {	// Affiche chaque ligne du texte
			fonteIntro.drawString(*iter, 20.0f, vertical, 1.0f);
			vertical -= 25.0f;
		}

		SDL_GL_SwapBuffers();	// Echange des buffers graphique -> affiche à l'écran
		SDL_Delay( (unsigned int)( 60.0f * ((float)rand()/(float)RAND_MAX) ) );	// Petite attente entre deux lettres consécutives

		if( checkEventsForIntro() ) {	// Vérifie si l'utilisateur veut sortir de l'intro
			delete image2;
			return;
		}
	}

	// Attends un petit instant à la fin de l'affichage du texte complet
	SDL_Delay( 1500 );
	lignes.clear();	// Efface tout le texte


	/* ***********************************************
	 * AFFICHAGE DE L'IMAGE DU SAUVEUR DE LA PLANETE
	 * ***********************************************/

	glDisable(GL_TEXTURE_2D);
	glDisable( GL_DEPTH_TEST );
	glClear( GL_COLOR_BUFFER_BIT );		// Efface l'écran

	DemonSons->Play( sonHurlement );	// Cri du sauveur de la planète

	glRasterPos2i( 0, 0 );		// A partir du coin de l'écran
	glDrawPixels( width, height, GL_RGB, GL_UNSIGNED_BYTE, image2 );

	delete image2;

	glEnable(GL_TEXTURE_2D);
	glDisable( GL_DEPTH_TEST );


	SDL_GL_SwapBuffers();	// Echange des buffers graphique -> affiche à l'écran
	SDL_Delay( 4000 );

	for( int i=0 ; i<20 ; i++ ) {		// Laisse quelques instants l'image affichée
		SDL_Delay( 100 );

		if( checkEventsForIntro() ) {	// Vérifie si l'utilisateur veut sortir de l'intro
			return;
		}
	}


	/* ***********************************************
	 * AFFICHAGE DE LA FIN DU TEXTE D'INTRO
	 * lettre par lettre (genre machine à écrire)
	 * ***********************************************/
		
	glColor3f( 1.0, 1.0, 1.0 );

	for(unsigned int i=0 ; i<str2.length() ; i++) {
		lettre = str2[ i ];

		glClear( GL_COLOR_BUFFER_BIT );

		if( i==0 )
			lignes.push_back( "" );

		if( (lettre=='\n') ) {
			DemonSons->Play( sonChariot );
			lignes.push_back( "" );
			SDL_Delay( 700 );
		}
		else {
			if( lettre!=' ' )	// Si c'est pas un espace
				DemonSons->Play( sonTouche );	// Envoie le son d'une frappe de touche clavier normale
			else
				DemonSons->Play( sonEspace );	// Sinon celui de la frappe d'un espace

			*lignes.rbegin() += lettre ; // Ajoute la lettre à la fin de la dernière ligne
		}

			// Affichage de tout le texte
		vertical = height - 360.0f;

		for( iter=lignes.begin() ; iter!=lignes.end() ; iter++ )	// Affiche chaque ligne
		{
			fonteIntro.drawString(*iter, 20.0f, vertical, 1.0f);
			vertical -= 45.0f;
		}

		SDL_GL_SwapBuffers();	// Echange des buffers graphique -> affiche à l'écran
		SDL_Delay( 75 + (unsigned int)( 60.0f * ((float)rand()/(float)RAND_MAX) ) );	// Petite attente entre deux lettres consécutives

		if( checkEventsForIntro() )	{	// Vérifie si l'utilisateur veut sortir de l'intro
			return;
		}
	}

	SDL_Delay( 1000 );
}
