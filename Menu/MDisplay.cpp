	/****************************/
	/*							*/
	/*   MENU PRINCIPAL SIMPLE	*/
	/*							*/
	/****************************/

#include <sstream>
using namespace std;

#include "DlgBoite.h"
#include "Menu.h"

#include "Focus.h"
#include "Cfg.h"
#include "Trace.h"

#include "Menu/MReseau.h"

using namespace JKT_PACKAGE_MENU;

class CGame;

extern CGame Game;
extern CFocus *pFocus;
extern bool Aide;
extern CCfg Config;

#include "Menu/MConfiguration.h"

namespace JKT_PACKAGE_MENU
{
extern CMenu MenuPrinc2;

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

char *item_menu_config_video[] =
{
	"Display",
	"Fullscreen",
};

CMenu MenuConfigVideo( "CONFIGURATION VIDEO", item_menu_config_video, 2,
						liste_suivant_config_video, lanceMenuConfig );

void lanceMenuConfigVideo(void *var)
{
TRACE().p( TRACE_MENU, "lanceMenuConfigVideo(var=%x)", var );
	ostringstream size;
	size << Config.Display.X << " * " << Config.Display.Y;
	MenuConfigVideo.add_ItemsDroits( 0, size.str().c_str() );

	if( Config.Display.Fullscreen() )
		MenuConfigVideo.add_ItemsDroits( 1, "Yes" );
	else
		MenuConfigVideo.add_ItemsDroits( 1, "No" );

	CDlg::SetMenuActif( &MenuConfigVideo );
}

void lanceMenuConfigVideoFullscreenOnOff(void *var)
{
TRACE().p( TRACE_MENU, "lanceMenuConfigVideoFullscreenOnOff(var=%x)", var );
	Config.Display.Fullscreen( !Config.Display.Fullscreen() );
	Config.Ecrit();

	lanceMenuConfigVideo( 0 );
}

}	// JKT_PACKAGE_MENU
