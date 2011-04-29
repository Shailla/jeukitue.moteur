	/****************************/
	/*							*/
	/*   MENU PRINCIPAL SIMPLE	*/
	/*							*/
	/****************************/

#include <fstream>
using namespace std;

#include "Menu/DlgBoite.h"
#include "Menu/Menu.h"
#include "main/Game.h"
#include "main/Focus.h"

#include "Menu/MMenuPrinc2.h"

using namespace JktMenu;

class CMaterial;
class CLight;
class CMouve;
class CGame;
class CGeoObject;
class CPorte;

extern CGame Game;
extern CFocus *pFocus;
extern bool Aide;

void quit_JKT();

namespace JktMenu
{
extern CMenu MenuPrinc;

void quit_JKTici( void *var );
void lanceMenuConfig( void *var );			// Menu de configuration vidéo, son, réseau, clavier,...
void lanceMenuOpenScene( void *var );		// Menu d'ouverture fichiers ASE et MAP
void lanceMenuMultijoueur( void *var );		// Menu pour le jeu multi-joueurs
void lanceMenuPrinc( void *var );
void lanceMenuTest( void *var );

	/****************************/
	/*							*/
	/*   MENU PRINCIPAL 2		*/
	/*							*/
	/****************************/
void quit_PartieEnCours(void *var)
{
	Game.Quit();

	lanceMenuPrinc( 0 );
}

PF liste_suivant_princ2[] =
{
	lanceMenuOpenScene,
	lanceMenuMultijoueur,
	lanceMenuConfig,
	quit_PartieEnCours,
	quit_JKTici,
};

char *item_menu_princ2[] =
{
	"Ouvrir une scene",
	"Multi-joueurs",
	"Configuration",
	"Quitter la partie",
	"Quitter",
};

CMenu MenuPrinc2( "MENU PRINCIPAL", item_menu_princ2, 5, liste_suivant_princ2, 0 );

}	// JktMenu
