	/****************************/
	/*							*/
	/*   MENU PRINCIPAL SIMPLE	*/
	/*							*/
	/****************************/

#include <fstream>
using namespace std;

#include "ihm/DlgBoite.h"
#include "ihm/Menu.h"
#include "util/Trace.h"
#include "main/Focus.h"

#include "ihm/MMenuPrinc.h"

using namespace JktMenu;

class CGame;

namespace JktMenu
{
	/************************/
	/*						*/
	/*   MENU CONFIGURATION	*/
	/*						*/
	/************************/
void lanceMenuConfigVideo(void *arg);
void lanceMenuConfigAudio(void *arg);
void lanceMenuConfigCommandes(void *arg);
void lanceMenuConfigReseau(void *arg);
void lanceMenuConfigDebug(void *arg);

char title_menu_config[] = "CONFIGURATION";

PF liste_suivant_config[] =
{
	lanceMenuConfigVideo,
	lanceMenuConfigAudio,
	lanceMenuConfigCommandes,
	lanceMenuConfigReseau,
	lanceMenuConfigDebug,
};

const char *item_menu_config[] =
{
	"Video",
	"Audio",
	"Commandes",
	"Reseau",
	"Debug",
};

CMenu MenuConfig(title_menu_config, (const char**)item_menu_config, 5, liste_suivant_config, lanceMenuPrinc );

void lanceMenuConfig(void *var)
{
TRACE().p( TRACE_MENU, "lanceMenuConfig(var=%x)", var );
	CDlg::SetMenuActif( &MenuConfig );
}

}	// JktMenu
