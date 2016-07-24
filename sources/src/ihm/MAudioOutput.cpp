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

using namespace jkt;

extern CCfg Config;

#include "fmod.h"
#include "MAudioDrivers.h"

namespace jkt
{
void lanceOutput1(void *arg);
void lanceOutput2(void *arg);
void lanceOutput3(void *arg);
void lanceOutput4(void *arg);

	/************************************************/
	/*												*/
	/*   MENU CONFIGURATION AUDIO DRIVERS OUTPUT	*/
	/*												*/
	/************************************************/

PF liste_suivant_config_audio_drivers_output[] =
{
	lanceOutput1,
	lanceOutput2,
	lanceOutput3,
	lanceOutput4,
};

const char *item_menu_config_audio_drivers_output[] =
{
#ifdef _WIN32
	"Direct Sound",
	"Windows Multimedia Waveout",
	"ASIO",
	"NoSound",
#elif defined(__linux__)
	"OSS - Open Sound System",
	"ESD - Elightment Sound Daemon",
	"ALSA 0.9 - Advanced Linux Sound Architecture",
	"NoSound",
#endif
};

CMenu MenuConfigAudioDriversOutput("DRIVERS OUTPUT",
								   item_menu_config_audio_drivers_output, 4,
								   liste_suivant_config_audio_drivers_output,
								   lanceMenuConfigAudioDrivers);

void lanceMenuConfigAudioDriversOutput(void *var)
{
LOGDEBUG(("lanceMenuConfigAudioDriversOutput(var=%x)", var));
	CDlg::SetMenuActif( &MenuConfigAudioDriversOutput );
}

void lanceOutput1(void *arg)
{
LOGDEBUG(("lanceOutput1(var=%x)", arg));
#ifdef _WIN32
	Config.Audio.m_Output = FSOUND_OUTPUT_DSOUND;
#elif defined(__linux__)
	Config.Audio.m_Output = FSOUND_OUTPUT_OSS;
#endif
//	FSOUND_SetOutput( Config.Audio.Output );
	Config.Ecrit();
	lanceMenuConfigAudioDrivers(0);
}

void lanceOutput2(void *arg)
{
LOGDEBUG(("lanceOutput2(var=%x)", arg));
#ifdef _WIN32
	Config.Audio.m_Output = FSOUND_OUTPUT_WINMM;
#elif defined(__linux__)
	Config.Audio.m_Output = FSOUND_OUTPUT_ESD;
#endif
//	FSOUND_SetOutput( Config.Audio.Output );
	Config.Ecrit();
	lanceMenuConfigAudioDrivers(0);
}

void lanceOutput3(void *arg)
{
LOGDEBUG(("lanceOutput3(var=%x)", arg));
#ifdef _WIN32
	Config.Audio.m_Output = FSOUND_OUTPUT_ASIO;
#elif defined(__linux__)
	Config.Audio.m_Output = FSOUND_OUTPUT_ALSA;
#endif
//	FSOUND_SetOutput( Config.Audio.Output );
	Config.Ecrit();
	lanceMenuConfigAudioDrivers(0);
}

void lanceOutput4(void *arg)
{
LOGDEBUG(("lanceOutput4(var=%x)", arg));
	Config.Audio.m_Output = FSOUND_OUTPUT_NOSOUND;
//	FSOUND_SetOutput( Config.Audio.Output );
	Config.Ecrit();
	lanceMenuConfigAudioDrivers(0);
}

}	// JktMenu
