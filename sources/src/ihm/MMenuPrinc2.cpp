	/****************************/
	/*							*/
	/*   MENU PRINCIPAL SIMPLE	*/
	/*							*/
	/****************************/

#include <fstream>

#include "ihm/DlgBoite.h"
#include "ihm/Menu.h"
#include "main/Game.h"
#include "main/Focus.h"

#include "ihm/MMenuPrinc2.h"

using namespace std;
using namespace jkt;

class CMaterial;
class CLight;
class CMouve;
class CGame;
class CGeoObject;
class CPorte;

extern CGame Game;

namespace jkt
{

void quit_JKTici( void *var );
void lanceMenuConfig( void *var );			// Menu de configuration vidéo, son, réseau, clavier,...
void lanceMenuOpenScene( void *var );		// Menu d'ouverture fichiers ASE et MAP
void lanceMenuPrinc( void *var );

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
	0,
	lanceMenuConfig,
	quit_PartieEnCours,
	quit_JKTici,
};

const char *item_menu_princ2[] =
{
	"Ouvrir une sc\u00e8ne",
	"Multi-joueurs",
	"Configuration",
	"Quitter la partie",
	"Quitter",
};

CMenu MenuPrinc2( "MENU PRINCIPAL", item_menu_princ2, 5, liste_suivant_princ2, 0 );

}	// JktMenu
