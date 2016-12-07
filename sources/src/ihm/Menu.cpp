
#include <iostream>
#include <set>
#include <fstream>
#include <map>

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/glu.h>
#include "SDL.h"

using namespace std;

class CGame;
#include "util/fonte/Fonte.h"
#include "util/Trace.h"
#include "spatial/IfstreamMap.h"
#include "main/Focus.h"
#include "main/Cfg.h"

#include "ihm/Dlg.h"
#include "ihm/DlgBoite.h"
#include "ihm/Menu.h"

using namespace jkt;

extern CFocus *pFocus;
extern bool Aide;
extern Fonte fonte;
extern CCfg Config;

namespace jkt
{

#define TAILLEFONT 0.8f
#define MAXITEM 11
#define CORX	0
#define CORY	100

CMenu::CMenu(const char *newTitle, const char **newItems, int nbrItems, PF *fct_suivante, PF fct_retour, void **liste_arg, PFV fct_refresh ) : CDlg() {
	LOGDEBUG(("CMenu::CMenu(newTitle=%s,nbrItems=%d,...)%T", newTitle, nbrItems, this));
	ajust = 0;

	// Titre du menu
	titre = new char[ strlen( newTitle )+1 ];
	strcpy( titre, newTitle );

	// Items (verticaux)
	choixY = 0;							// On focus sur le premier item du menu
	nbrChoixY = nbrItems;				// Nombre d'items
	items = new char*[ nbrItems ];		// Crétion et copie des items du menu
	for( int i=0 ; i<nbrChoixY ; i++ ) {
		items[ i ] = new char[ strlen(newItems[i] )+1 ];
		strcpy( items[ i ], newItems[ i ] );
	}

	// Fonctions suivantes des items
	fonction_suivante = new PF[ nbrChoixY ];
	for( int i=0 ; i<nbrChoixY ; i++ )
		fonction_suivante[ i ] = fct_suivante[ i ];

	// Arguments des fonctions suivantes
	if( liste_arg ) {
		arg = new void*[ nbrChoixY ];
		for( int i=0 ; i<nbrChoixY ; i++ )
			arg[ i ] = liste_arg[ i ];
	}
	else {
		arg = 0;
	}

	fonction_retour = fct_retour;	// Fonction à exécuter au retour du menu

	bItemsDroits = false;		// Indique qu'aucun item droit n'et sélectionné
	items_droits = 0;			// Pas d'items droits par défaut
	items_rem = 0;				// Pas d'items remarques par défaut
	mode = 0;					// Mode normal

	fct_Refresh = fct_refresh;		// Fonction de rafraichissement du menu
};

CMenu::~CMenu() {
	LOGDEBUG(("CMenu::~CMenu() titre=%s%T", titre, this));

	// Titre
	if( titre )
		delete[] titre;

	// Items
	del_Items();

	// Fonctions suivantes des items
	if( fonction_suivante )
		delete[] fonction_suivante;

	// Arguments des fonctions suivantes des items
	if( arg ) {
		delete[] arg;
	}

	del_ItemsDroits();
	del_ItemsRem();
}

void CMenu::afficheTitre() {
	float color[] = { 1.0f, 0.0f, 0.8f };
	fonte.drawString( titre, 280+CORX, 360+CORY, INFOFONTESCALAR, color);
}

void CMenu::afficheItems() {
	int i = 0;

	// Affichage de la barre de défilement
	if( nbrChoixY > MAXITEM ) {	// S'il y a beaucoup d'items à afficher
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glDisable( GL_DEPTH_TEST );
		glDepthMask( GL_FALSE );

		// Dimension du curseur
		float dim = (292-100) * ( float(MAXITEM) / float(nbrChoixY) );
		if( dim<4 )
			dim = 4;

		// Position du curseur sur la barre
		float pos = ( ( MAXITEM * 20 )-8 - dim ) * ( float(choixY) / float(nbrChoixY-1) );

		glBegin( GL_QUADS );
		// Affichage de la barre
		glColor3f( 0.0, 1.0, 0.0 );
		glVertex2f( 240+CORX, 300-(MAXITEM*20)+4	+CORY);
		glVertex2f( 240+CORX, 300-4					+CORY);
		glVertex2f( 220+CORX, 300-4					+CORY);
		glVertex2f( 220+CORX, 300-(MAXITEM*20)+4	+CORY);

		// Affichage du curseur de la barre
		glColor3f( 1.0, 0.0, 0.0 );
		glVertex2f( 238+CORX, 300-4			+ CORY - pos );
		glVertex2f( 238+CORX, 300-4 - dim	+ CORY - pos );
		glVertex2f( 222+CORX, 300-4 - dim	+ CORY - pos );
		glVertex2f( 222+CORX, 300-4			+ CORY - pos );
		glEnd();

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
	}

	if( nbrChoixY > MAXITEM ) {
		if( choixY < (MAXITEM / 2) + 1 )
			ajust = 0;
		else if( choixY >= nbrChoixY - (MAXITEM/2) )
			ajust = nbrChoixY - MAXITEM;
		else
			ajust = choixY - (MAXITEM/2);
	}
	else {
		ajust = 0;
	}

	// Affichage des items du menu
	while( i<MAXITEM && i+ajust<nbrChoixY ) {
		if( i+ajust==choixY )
			glColor3f( 1.0, 0.0, 0.0 );		// Couleur rouge si focussé
		else if( fonction_suivante[ i+ajust ] == 0 )
			glColor3f( 0.6f, 0.6f, 0.6f );
		else
			glColor3f( 1.0, 1.0, 1.0 );		// Couleur blanche par défaut

		fonte.drawString(items[i+ajust], float(260+CORX), float(300+CORY-((i+1)*20)), INFOFONTESCALAR);
		i++;
	}

	glDisable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );
}

void CMenu::afficheItemsDroits() {
	if( items_droits ) {
		int i = 0;
		// Affichage les items droit du menu
		while( i<MAXITEM && i+ajust<nbrChoixY )
		{
			if( fonction_suivante[ i+ajust ]==0 )
				glColor3f( 0.6f, 0.6f, 0.6f );
			else if( i+ajust==choixY && bItemsDroits )
				glColor3f( 0.0, 1.0, 0.0 );		// Couleur blanche par défaut
			else
				glColor3f( 1.0, 1.0, 1.0 );		// Couleur rouge si focussé

			if( items_droits[i+ajust] )
				fonte.drawString( items_droits[i+ajust], float(430+CORX), float(300+CORY-(i*20)), INFOFONTESCALAR);
			i++;
		}
	}
}

void CMenu::afficheItemsRem() {
	if( items_rem ) {
		// Affichage de la remarque associée à l'item focussé
		glColor3f( 0.5f, 0.5f, 0.5f );
		if( items_rem[choixY] )
			fonte.drawString( items_rem[choixY], 100, 50+CORY, INFOFONTESCALAR);
	}
}

void CMenu::go() {
	if( fct_Refresh )	// S'il y a une fonction de rafraichissement pour ce menu, exécute-la
		fct_Refresh();

	afficheTitre();			// Affiche le titre du menu
	afficheItems();			// Affiche les items du menu
	afficheItemsDroits();	// Affiche les items droits du menu
	afficheItemsRem();		// Affiche la remarque associée à l'item
}

void CMenu::down() {
	choixY++;

	if( choixY==nbrChoixY )
		choixY = 0;
}

void CMenu::up() {
	choixY--;

	if( choixY<0 )
		choixY = nbrChoixY-1;
}

PF CMenu::enter() {
	return fonction_suivante[choixY];
}

void *CMenu::argument() {
	if( arg!=0 )
		return arg[choixY];
	else
		return 0;
}

void CMenu::add_ItemsDroits(int num, const char *txt) {	// Implémente le num° item droit
	if( !items_droits ) {
		items_droits = new char*[ nbrChoixY ];

		for( int i=0 ; i<nbrChoixY ; i++ )
			items_droits[i] = 0;
	}

	if( items_droits[ num ] )
		delete items_droits[ num ];

	items_droits[ num ] = new char[ (strlen( txt )+1)*sizeof(char) ];
	strcpy( items_droits[ num ], txt );
}

void CMenu::add_ItemsRem(int num, char *txt) {	// Implémente le num° item droit
	if( !items_rem ) {
		items_rem = new char*[ nbrChoixY ];

		for( int i=0 ; i<nbrChoixY ; i++ )
			items_rem[i] = 0;
	}

	if( items_rem[num] )
		delete[] items_rem[num];

	items_rem[num] = new char[ strlen( txt ) + 1 ];
	strcpy( items_rem[num], txt );
}

void CMenu::del_Items() {
	if( items ) {
		for( int i=0 ; i<nbrChoixY ; i++ )
			if( items[i] )
				delete[] items[i];

		delete[] items;
		items = 0;
	}
}

void CMenu::del_ItemsDroits() {	// Efface la liste des items droits
	if( items_droits ) {
		for( int i=0 ; i<nbrChoixY ; i++ )
			if( items_droits[ i ] )
				delete[] items_droits[ i ];

		delete[] items_droits;
	}

	items_droits = 0;
}

void CMenu::del_ItemsRem() {
	if( items_rem ) {
		for( int i=0 ; i<nbrChoixY ; i++ )
			if( items_rem[ i ] )
				delete[] items_rem[ i ];

		delete[] items_rem;
	}

	items_rem = 0;
}

void CMenu::KeyDown(SDL_Event *event) {
	switch( event->key.keysym.sym ) {
	case SDLK_ESCAPE:
		if( fonction_retour ) {
			fonction_retour( 0 );
		}
		else {
			pFocus->SetPlayFocus();
			Aide = false;
		}

		break;

	case SDLK_RETURN :
		if( enter() )
			enter()(argument());
		break;

	case SDLK_UP :
		up();
		break;

	case SDLK_DOWN :
		down();
		break;

	case SDLK_RIGHT :
		break;

	case SDLK_LEFT  :
		break;

	default:
		break;
	}
}

void CMenu::MouseBoutonDown(SDL_Event *event) {
	if( event->button.button==SDL_BUTTON_RIGHT ) {
		if( fonction_retour )
			fonction_retour( 0 );
		else {
			pFocus->SetPlayFocus();
			Aide = false;
		}
	}
	else if( event->button.button==SDL_BUTTON_LEFT ) {
		int x = event->button.x;
		int y = event->button.y;

		if( (x>=250) && (x<=450) ) {
			int yref = (300+CORY) - (Config.Display.Y - y);
			if( yref >=0 ) {
				int choix = yref/20;

				if( choix < nbrChoixY ) {
					choixY = choix + ajust;
					if( enter() )
						enter()(argument());
				}
			}
		}
	}
	else if( event->button.button==4 ) {
		up();
	}
	else if( event->button.button==5 ) {
		down();
	}
}

void CMenu::handle_key_down( SDL_Event *event ) {
	switch( mode ) {
	case 0:	// Mode menu normal
		switch( event->type )
		{
		case SDL_KEYDOWN:
			KeyDown( event );
			break;

		case SDL_MOUSEBUTTONDOWN:
			MouseBoutonDown( event );
			break;
		}
		break;

		case 1:	// Mode saisie de touches
			enter()((void*)event);
			break;
	}
}

}	// JktMenu
