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
#include "main/Cfg.h"
#include "util/Trace.h"

#include "Menu/MAudio.h"

using namespace JktMenu;

extern CGame Game;
extern CFocus *pFocus;
extern bool Aide;
extern CCfg Config;

#include "Menu/MConfiguration.h"

namespace JktMenu
{
extern CMenu MenuPrinc2;

void lanceMenuConfigAudioDrivers(void*);

	/********************************/
	/*								*/
	/*   MENU CONFIGURATION	AUDIO	*/
	/*								*/
	/********************************/

char title_menu_config_audio[] = "CONFIGURATION AUDIO";

PF liste_suivant_config_audio[] =
{
	lanceMenuConfigAudioDrivers,
};

char *item_menu_config_audio[] =
{
	"Drivers",
};

CMenu MenuConfigAudio(title_menu_config_audio, item_menu_config_audio, 1,
						liste_suivant_config_audio, lanceMenuConfig );

void lanceMenuConfigAudio(void *var)
{
TRACE().p( TRACE_MENU, "lanceMenuConfigAudio(var=%x)", var );
	CDlg::SetMenuActif( &MenuConfigAudio );
}

}	// JktMenu
