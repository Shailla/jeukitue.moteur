	/****************************/
	/*							*/
	/*   MENU PRINCIPAL SIMPLE	*/
	/*							*/
	/****************************/

#include <sstream>
using namespace std;

#include "ihm/DlgBoite.h"
#include "ihm/Menu.h"

#include "main/Focus.h"
#include "main/Cfg.h"
#include "util/Trace.h"

using namespace jkt;

class CGame;

extern CCfg Config;

#include "ihm/MConfiguration.h"

namespace jkt
{
void lanceMenuConfigVideoDisplay(void*);
void lanceMenuConfigVideoFullscreenOnOff(void*);

	/********************************/
	/*								*/
	/*   MENU CONFIGURATION	VIDEO	*/
	/*								*/
	/********************************/

PF liste_suivant_config_video[] =
{
	lanceMenuConfigVideoDisplay,
	lanceMenuConfigVideoFullscreenOnOff,
};

const char *item_menu_config_video[] =
{
	"Display",
	"Fullscreen",
};

CMenu MenuConfigVideo( "CONFIGURATION VIDEO", item_menu_config_video, 2,
						liste_suivant_config_video, lanceMenuConfig );

void lanceMenuConfigVideo(void *var) {
LOGDEBUG(("lanceMenuConfigVideo(var=%x)", var));
	ostringstream size;
	size << Config.Display.X << " * " << Config.Display.Y;
	MenuConfigVideo.add_ItemsDroits( 0, size.str().c_str() );

	if( Config.Display.Fullscreen() )
		MenuConfigVideo.add_ItemsDroits( 1, "Yes" );
	else
		MenuConfigVideo.add_ItemsDroits( 1, "No" );

	CDlg::SetMenuActif( &MenuConfigVideo );
}

void lanceMenuConfigVideoFullscreenOnOff(void *var) {
LOGDEBUG(("lanceMenuConfigVideoFullscreenOnOff(var=%x)", var));
	Config.Display.Fullscreen( !Config.Display.Fullscreen() );
	Config.Ecrit();

	lanceMenuConfigVideo( 0 );
}

}	// JktMenu
