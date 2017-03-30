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
#include "main/Cfg.h"
#include "util/Trace.h"

#include "ihm/MAudio.h"

#include "ihm/MConfiguration.h"

using namespace std;
using namespace jkt;

namespace jkt
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
LOGDEBUG(("lanceMenuConfigAudio(var=%x)", var));
	CDlg::SetMenuActif( &MenuConfigAudio );
}

}	// JktMenu
