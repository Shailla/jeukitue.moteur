	/****************************/
	/*							*/
	/*   MENU PRINCIPAL SIMPLE	*/
	/*							*/
	/****************************/

#include <fstream>
#include <iostream>

#include "SDL.h"

#include "ihm/DlgBoite.h"
#include "ihm/Menu.h"
#include "main/Cfg.h"
#include "main/Focus.h"
#include "util/Trace.h"
#include "main/divers.h"

using namespace jkt;

class CGame;

extern CCfg Config;

#include "ihm/MDisplaySize.h"

using namespace std;

namespace jkt
{
void lanceDisplaySizeChange(void*);
void retourVideoSize(void *arg);

	/****************************************/
	/*										*/
	/*   MENU CONFIGURATION	VIDEO DISPLAY	*/
	/*										*/
	/****************************************/
CMenu *MenuConfigVideoDisplaySize;

class rect {
public:
	int w;
	int h;
	rect(int ww, int hh ) {	w = ww;		h = hh;	}
};

void lanceMenuConfigVideoDisplay(void *var) {
LOGDEBUG(("lanceMenuConfigVideoDisplay(var=%x)", var));
	int nbr;
		// Récupère la liste des modes vidéo disponibles
	SDL_Rect **modes = SDL_ListModes(NULL, SDL_FULLSCREEN|SDL_HWSURFACE);

	// Compte le nombre de modes disponibles
	for( nbr=0 ; modes[nbr] ; nbr++ );	// Le ';' est tout à fait normal !!!

	PF *liste_suivants = new PF[ nbr ];
	void **liste_arguments = new void*[ nbr ];
	char **items_menu = new char*[ nbr ];

	for( int i=0 ; i<nbr ; i++ ) {
		char txt[50];
		sprintf( txt, "%d x %d", modes[i]->w, modes[i]->h );
		liste_suivants[i] = lanceDisplaySizeChange;

		items_menu[i] = new char[ strlen(txt) + 1 ];
		sprintf( items_menu[i], "%s", txt );

		liste_arguments[i] = new rect( modes[i]->w, modes[i]->h );
	}

	MenuConfigVideoDisplaySize = new CMenu( "DIMENSIONS D'ECRAN", (const char**)items_menu, nbr, liste_suivants, retourVideoSize, liste_arguments );

	for(int i=0 ; i<nbr ; i++ )
		delete items_menu[i];

	delete[] items_menu;
	delete[] liste_suivants;

	CDlg::SetMenuActif( MenuConfigVideoDisplaySize );
}

void lanceDisplaySizeChange(void *arg) {
LOGDEBUG(("lanceDisplay640_468(var=%x)", arg));

	rect *rr = (rect*)arg;

	Config.Display.ChangeVideoSize(rr->w, rr->h);
	Config.Ecrit();
}

void retourVideoSize(void *arg) {
	delete MenuConfigVideoDisplaySize;
	CDlg::SetMenuActif( 0 );

	lanceMenuConfigVideo( 0 );
}

}	// JktMenu
