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
#include "main/Cfg.h"
#include "util/Trace.h"

#include "ihm/MAudio.h"

using namespace JktMenu;

#include "ihm/MConfiguration.h"


namespace JktMenu
{

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

const char *item_menu_config_audio[] =
{
	"Drivers",
};

CMenu MenuConfigAudio(title_menu_config_audio, item_menu_config_audio, 1,
						liste_suivant_config_audio, lanceMenuConfig );

void lanceMenuConfigAudio(void *var)
{
TRACE().debug("lanceMenuConfigAudio(var=%x)", var);
	CDlg::SetMenuActif( &MenuConfigAudio );
}

}	// JktMenu
