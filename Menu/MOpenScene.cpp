	/****************************/
	/*							*/
	/*   MENU PRINCIPAL SIMPLE	*/
	/*							*/
	/****************************/

#include <fstream>
using namespace std;

#include "DlgBoite.h"
#include "Menu.h"
#include "Game.h"
#include "Focus.h"
#include "Trace.h"

#include "MMenuPrinc.h"

using namespace JKT_PACKAGE_MENU;

class CMaterial;
class CLight;
class CMouve;
class CGame;
class CGeoObject;
class CPorte;

extern CGame Game;
extern CMenu MenuPrinc2;
extern CFocus *pFocus;
extern bool Aide;

namespace JKT_PACKAGE_MENU
{
	/************************/
	/*						*/
	/*   MENU OPEN SCENE	*/
	/*						*/
	/************************/

void lanceMenuConvertASE(void *var);
void lanceMenuOpenMAP(void *var);

PF liste_suivant_open_scene[] =
{
	lanceMenuConvertASE, 
	lanceMenuOpenMAP,
};

char *item_menu_open_scene[] =
{
	"Convertir une scene ASE",
	"Ouvrir une scene MAP",
};

CMenu MenuOpenScene( "OUVRIR UNE SCENE", item_menu_open_scene, 2,
						liste_suivant_open_scene, lanceMenuPrinc );

void lanceMenuOpenScene(void *var)
{
TRACE().p( TRACE_MENU, "lanceMenuOpenScene(var=%x)", var );
	CDlg::SetMenuActif( &MenuOpenScene );
}

}	// JKT_PACKAGE_MENU
