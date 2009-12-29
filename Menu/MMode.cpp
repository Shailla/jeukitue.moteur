	/****************************/
	/*							*/
	/*   MENU PRINCIPAL SIMPLE	*/
	/*							*/
	/****************************/

#include <fstream>
#ifdef WIN32
	#include <windows.h>
#endif

using namespace std;

#include "DlgBoite.h"
#include "Menu.h"

using namespace JKT_PACKAGE_MENU;

class CMaterial;
class CLight;
class CMouve;
class CGame;
class CGeoObject;
class CPorte;
#include "util/V3D.h"
#include "util/Tableau.h"
#include "spatial/IfstreamMap.h"
#include "util/TableauIndex.h"
#include "main/Projectil.h"
#include "son/DemonSons.h"
#include "reseau/SPA.h"
#include "main/Player.h"
#include "reseau/Client.h"
#include "reseau/Server.h"
#include "main/RequeteProcess.h"
#include "spatial/Map.h"
#include "main/Game.h"
#include "Focus.h"
#include "main/Cfg.h"
#include "util/Trace.h"

#include "Menu/MMode.h"

extern CGame Game;
extern CMenu MenuPrinc2;
extern CFocus *pFocus;
extern bool Aide;
extern CCfg Config;

namespace JKT_PACKAGE_MENU
{
	/****************/
	/*				*/
	/*   MENU MODE	*/
	/*				*/
	/****************/

	// Menu mode principal

PF liste_suivant_mode[] =
{
	lanceMenuModeSelection,
};


char *item_menu_mode[] =
{
	"MODE SELECTION",
};

CMenu MenuMode( "MODES", item_menu_mode, 1, liste_suivant_mode, 0 );

	// Sous Menu mode

void fonctionModeSelection1(void *var)
{
	cout << "Solidité 1\n";
}

void fonctionModeSelection2(void *var)
{
	cout << "Suppression 2\n";
}

PF liste_suivant_Mode_Selection[] =
{
	fonctionModeSelection1,
	fonctionModeSelection2,
};

char *item_menu_Mode_Selection[] =
{
	"SOLIDITE",
	"SUPPRESSION",
};

CMenu MenuModeSelection( "MODE SELECTION", item_menu_Mode_Selection, 2,
						liste_suivant_Mode_Selection, lanceMenuMode );

void lanceMenuMode(void *var)
{
TRACE().p( TRACE_MENU, "lanceMenuMode(var=%x)", var );
	CDlg::SetMenuActif( &MenuMode );
	pFocus->SetMenuFocus();		// Mets le focus sur le menu
	Aide = true;
}

void lanceMenuModeSelection(void *var)
{
TRACE().p( TRACE_MENU, "lanceMenuModeSelection(var=%x)", var );
	CDlg::SetMenuActif( &MenuModeSelection );
}

}	// JKT_PACKAGE_MENU
