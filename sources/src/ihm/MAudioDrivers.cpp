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

#include "ihm/MAudioDrivers.h"

using namespace JktMenu;

extern CCfg Config;

#include "ihm/MAudio.h"
#include "son/audio.h"

namespace JktMenu
{
void lanceMenuConfigAudioDriversOutput(void *arg);
void lanceMenuConfigAudioDriversDriver(void *arg);
void lanceMenuConfigAudioDriversRecord(void *arg);

	/****************************************/
	/*										*/
	/*   MENU CONFIGURATION AUDIO DRIVERS	*/
	/*										*/
	/****************************************/

PF liste_suivant_config_audio_drivers[] =
{
	lanceMenuConfigAudioDriversOutput,
	0,
	lanceMenuConfigAudioDriversDriver,
	lanceMenuConfigAudioDriversRecord,
};

const char *item_menu_config_audio_drivers[] =
{
	"Output",
	"Mixer",
	"Driver",
	"Record",
};

CMenu MenuConfigAudioDrivers( "DRIVERS", item_menu_config_audio_drivers, 4,
						liste_suivant_config_audio_drivers, lanceMenuConfigAudio );

void lanceMenuConfigAudioDrivers(void *var)
{
TRACE().p( TRACE_MENU, "lanceMenuConfigAudioDrivers(var=%x)", var );
	MenuConfigAudioDrivers.add_ItemsDroits( 0, resolveOutput(Config.Audio.m_Output) );
	MenuConfigAudioDrivers.add_ItemsDroits( 1, resolveMixer(Config.Audio.m_Mixer) );
	MenuConfigAudioDrivers.add_ItemsDroits( 2, (char*)resolveDriver(Config.Audio.m_Driver) );
	MenuConfigAudioDrivers.add_ItemsDroits( 3, (char*)resolveDriverRecord(Config.Audio.m_DriverRecord) );

	CDlg::SetMenuActif( &MenuConfigAudioDrivers );
}

}	// JktMenu
