
#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

#include <agar/core.h>
#include "SDL.h"
#include "fmod.h"
#include "fmod_errors.h"
#include "SDL_image.h"
#include "SDL_net.h"
#include "glfont2.h"
using namespace glfont;

#include "ressource/RessourcesLoader.h"
#include "util/Trace.h"
#include "util/utils_SDL.h"
#include "son/DemonSons.h"
#include "son/Son.h"
#include "spatial/materiau/Textures.h"
#include "main/Cfg.h"

#include "main/divers.h"

using namespace JktSon;

extern JktSon::CDemonSons *DemonSons;

void quit_game(const char* txt, int code)	// Quitte proprement le jeu
{
TRACE().debug("quit_game(code=%d,txt=%s)", code, txt);
	cerr << endl << __FILE__ << ":" << __LINE__ << " Quit game with code and message : " << code << " - '" << txt << "'" << flush;
	exit( code );
}

void quit_game(const string& txt, int code)	// Quitte proprement le jeu
{
TRACE().debug("quit_game(code=%d,txt=%s)", code, txt.c_str());
	cerr << endl << __FILE__ << ":" << __LINE__ << " Quit game with code and message : " << code << " - '" << txt << "'" << flush;
	exit( code );
}

void quit_JKT() {
TRACE().debug("quit_JKT()");
	string trace1 = "Derniere erreur FMOD : ";
	trace1 += FMOD_ErrorString(FSOUND_GetError());

	string trace2 = "Derniere erreur SDL : ";
	trace2 += SDL_GetError();

	string trace3 = "Derniere erreur SDL_Net : ";
	trace3 += SDLNet_GetError();

	string trace4 = "Derniere erreur openGL : ";
	trace4 += (char*)gluErrorString(glGetError());

	string trace5 = "Derniere erreur Agar : ";
	const char* agarError = AG_GetError();

	if(agarError != NULL) {
		trace5 += agarError;
	}
	else {
		trace5 += "Aucune";
	}

TRACE().debug(trace1.c_str());
TRACE().debug(trace2.c_str());
TRACE().debug(trace3.c_str());
TRACE().debug(trace4.c_str());
TRACE().debug(trace5.c_str());

	cerr << endl << __FILE__ << ":" << __LINE__;
	cerr << trace1 << endl;
	cerr << trace2 << endl;
	cerr << trace3 << endl;
	cerr << trace4 << endl;
	cerr << trace5 << endl;
	cerr.flush();

	FSOUND_Close();		// Fermeture d'FMOD
	SDLNet_Quit();		// Fermeture d'SDL_Net
	SDL_Quit();			// Fermeture de SDL

	TRACE().error("Erreur OpenGL : %s", gluErrorString(glGetError()));
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
TRACE().debug("load_Intro(width=%d,height=%d)", width, height);
	string bruitChariot = "@Bruit\\chariot.wav";		// Chargement de la fonte de caractères
	JktUtils::RessourcesLoader::getFileRessource(bruitChariot);
	sonChariot = DemonSons->CreateSon( bruitChariot.c_str() );		// Son retour chariot machine à écrire

	string bruitTouche = "@Bruit\\touche.wav";		// Chargement de la fonte de caractères
	JktUtils::RessourcesLoader::getFileRessource(bruitTouche);
	sonTouche = DemonSons->CreateSon( bruitTouche.c_str() );		// Son frappe d'une touche clavier

	string bruitEspace = "@Bruit\\espace.wav";		// Chargement de la fonte de caractères
	JktUtils::RessourcesLoader::getFileRessource(bruitEspace);
	sonEspace = DemonSons->CreateSon( bruitEspace.c_str() );		// Son frappe de la touche espace clavier

	string bruitHurlement = "@Bruit\\hurlement.wav";		// Chargement de la fonte de caractères
	JktUtils::RessourcesLoader::getFileRessource(bruitHurlement);
	sonHurlement = DemonSons->CreateSon( bruitHurlement.c_str() );	// Son hurlement du sauveur de la planète

	load_IntroSub( width, height );

	DemonSons->Delete( sonChariot );	// Son retour chariot machine à écrire
	DemonSons->Delete( sonTouche );		// Son frappe d'une touche clavier
	DemonSons->Delete( sonEspace );		// Son frappe de la touche espace clavier
	DemonSons->Delete( sonHurlement );	// Son hurlement du sauveur de la planète
}


void load_IntroSub(const int width, const int height ) {
TRACE().debug("load_IntroSub(width=%d,height=%d)", width, height);
	GLFont fonteIntro;
	unsigned int texFonteIntro;

	string fileFonteIntro = "@Fonte\\FonteIntro.glf";	// Chargement de la fonte de caractères
	JktUtils::RessourcesLoader::getFileRessource(fileFonteIntro);

	string str1 = "Nous sommes en 2056.\n\nLa surface de la Terre n'est plus qu'un ocean d'acide.\nLa loi a laisse sa place a celle du plus fort et tout se regle\ndesormais lors de combats sans gland.\n\n      Voici le seul homme qui peut encore sauver la planete... ";
	string str2 = "Alors, vous l'avez compris...\n   On est vraiment dans la merde !";
	vector< string > lignes;		// Lignes séparées par un retour chariot

	srand( SDL_GetTicks() );	// Initialisation de la fonction rand() pour les nombres aléatoires

	string introJKT = "@Fond\\intro_JKT2.jpg";
	JktUtils::RessourcesLoader::getFileRessource(introJKT);
	SDL_Surface *image1 = IMG_Load(introJKT.c_str());		// Lit le fichier image

	if(image1 == 0) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur : Ouverture d'image (" << introJKT << ")" << endl;
		return;
	}

	Uint8 *image2 = new Uint8[width*height*3];

	SDL_LockSurface( image1 );
	gluScaleImage( GL_RGB, image1->w, image1->h, GL_UNSIGNED_BYTE, image1->pixels,
		width, height, GL_UNSIGNED_BYTE, image2 );
	SDL_UnlockSurface( image1 );
	SDL_FreeSurface( image1 );

	glGenTextures( 1, &texFonteIntro );

	if( !fonteIntro.Create( fileFonteIntro, texFonteIntro ) ) {
		TRACE().debug("loadSubIntro() Texture de fonte (%s) : %d", fileFonteIntro.c_str(), texFonteIntro);
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur : Echec d'ouverture de la fonte : " << fileFonteIntro << endl;
	}
	else {
		TRACE().debug("loadSubIntro() Texture de fonte (%s) : %d", fileFonteIntro.c_str(), texFonteIntro);
	}

	glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
	gluOrtho2D(0.0, width, 0.0, height );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();


	glEnable(GL_TEXTURE_2D);
	glDisable( GL_DEPTH_TEST );

	fonteIntro.Begin();
	vector< string >::const_iterator iter;
	float vertical;
	char lettre;


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
			fonteIntro.DrawString( *iter, 1.2f, 20.0f, vertical );
			vertical -= 25.0f;
		}

		SDL_GL_SwapBuffers();	// Echange des buffers graphique -> affiche à l'écran
		SDL_Delay( (unsigned int)( 60.0f * ((float)rand()/(float)RAND_MAX) ) );	// Petite attente entre deux lettres consécutives

		if( checkEventsForIntro() ) {	// Vérifie si l'utilisateur veut sortir de l'intro
			glDeleteTextures( 1, &texFonteIntro );	// Destruction de la texture de fonte
			delete image2;
			return;
		}
	}

		// Attends un petit instant à la fin de l'affichage du texte complet
	SDL_Delay( 400 );
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

	for( int i=0 ; i<35 ; i++ ) {		// Laisse quelques instants l'image affichée
		SDL_Delay( 100 );

		if( checkEventsForIntro() ) {	// Vérifie si l'utilisateur veut sortir de l'intro
			glDeleteTextures( 1, &texFonteIntro );	// Destruction de la texture de fonte
			return;
		}
	}


	/* ***********************************************
	 * AFFICHAGE DE LA FIN DU TEXTE D'INTRO
	 * lettre par lettre (genre machine à écrire)
	 * ***********************************************/
		
	fonteIntro.Begin();
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
			fonteIntro.DrawString( *iter, 1.2f, 20.0f, vertical );
			vertical -= 45.0f;
		}

		SDL_GL_SwapBuffers();	// Echange des buffers graphique -> affiche à l'écran
		SDL_Delay( 75 + (unsigned int)( 60.0f * ((float)rand()/(float)RAND_MAX) ) );	// Petite attente entre deux lettres consécutives

		if( checkEventsForIntro() )	{	// Vérifie si l'utilisateur veut sortir de l'intro
			glDeleteTextures( 1, &texFonteIntro );	// Destruction de la texture de fonte
			return;
		}
	}

	SDL_Delay( 1000 );


	/* *******************************
	 * Destruction des ressources
	 * *******************************/

	glDeleteTextures( 1, &texFonteIntro );	// Destruction de la texture de fonte
}
