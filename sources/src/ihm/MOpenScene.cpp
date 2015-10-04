	/****************************/
	/*							*/
	/*   MENU PRINCIPAL SIMPLE	*/
	/*							*/
	/****************************/

#include <fstream>
using namespace std;

#include "ihm/DlgBoite.h"
#include "ihm/Menu.h"
#include "main/Game.h"
#include "main/Focus.h"
#include "util/Trace.h"

#include "MMenuPrinc.h"

using namespace JktMenu;

class CMaterial;
class CLight;
class CMouve;
class CGame;
class CGeoObject;
class CPorte;


namespace JktMenu
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

const char *item_menu_open_scene[] =
{
	"Convertir une scene ASE",
	"Ouvrir une scene MAP",
};

CMenu MenuOpenScene( "OUVRIR UNE SCENE", item_menu_open_scene, 2,
						liste_suivant_open_scene, lanceMenuPrinc );

void lanceMenuOpenScene(void *var) {
LOGDEBUG(("lanceMenuOpenScene(var=%x)", var));
	CDlg::SetMenuActif( &MenuOpenScene );
}

}	// JktMenu
