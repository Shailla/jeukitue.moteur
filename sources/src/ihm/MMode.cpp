	/****************************/
	/*							*/
	/*   MENU PRINCIPAL SIMPLE	*/
	/*							*/
	/****************************/

#include <fstream>
#ifdef WIN32
	#include <windows.h>
#endif

#include "includes.h"

#include "ihm/DlgBoite.h"
#include "ihm/Menu.h"
#include "util/V3D.h"
#include "util/Tableau.h"
#include "spatial/IfstreamMap.h"
#include "main/Projectil.h"
#include "son/DemonSons.h"
#include "reseau/SPA.h"
#include "reseau/Client.h"
#include "reseau/Server.h"
#include "main/RequeteProcess.h"
#include "spatial/Map.h"
#include "main/Game.h"
#include "main/Focus.h"
#include "main/Cfg.h"
#include "util/Trace.h"

#include "ihm/MMode.h"

using namespace std;
using namespace jkt;

extern CFocus *pFocus;
extern bool Aide;

namespace jkt
{
	/****************/
	/*				*/
	/*   MENU MODE	*/
	/*				*/
	/****************/

	// Menu mode principal

PF liste_suivant_mode[] =
{
	lanceMenuModeHighlight,
};


const char *item_menu_mode[] =
{
	"MODE HIGHLIGHT",
};

CMenu MenuMode( "MODES", item_menu_mode, 1, liste_suivant_mode, 0 );

	// Sous Menu mode

void fonctionModeHighlight1(void *var)
{
	cout << "Solidité 1\n";
}

void fonctionModeHighlight2(void *var)
{
	cout << "Suppression 2\n";
}

PF liste_suivant_Mode_Highlight[] =
{
	fonctionModeHighlight1,
	fonctionModeHighlight2,
};

const char *item_menu_Mode_Highlight[] =
{
	"SOLIDITE",
	"SUPPRESSION",
};

CMenu MenuModeHighlight( "MODE HIGHLIGHT", item_menu_Mode_Highlight, 2,
						liste_suivant_Mode_Highlight, lanceMenuMode );

void lanceMenuMode(void *var) {
LOGDEBUG(("lanceMenuMode(var=%x)", var));
	CDlg::SetMenuActif( &MenuMode );
	pFocus->SetMenuFocus();		// Mets le focus sur le menu
	Aide = true;
}

void lanceMenuModeHighlight(void *var) {
LOGDEBUG(("lanceMenuModeHighlight(var=%x)", var));
	CDlg::SetMenuActif( &MenuModeHighlight );
}

}	// JktMenu
