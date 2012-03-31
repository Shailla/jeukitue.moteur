

#ifdef WIN32				// Pour FMOD
  #include <conio.h>		// Pour FMOD
  #include <windows.h>		// Pour FMOD
#else						// Pour FMOD
  #include "wincompat.h"	// Pour FMOD
#endif						// Pour FMOD
#include <vector>
#include <iostream>

using namespace std;

#include "SDL.h"
#include "fmod.h"
#include "fmod_errors.h"	/* optional */

#include "util/Trace.h"
#include "util/V3D.h"
#include "son/DemonSons.h"
#include "main/Cfg.h"

#include "audio.h"

using namespace JktSon;

extern CCfg Config;
//char **remarquesDriver;
CDemonSons *DemonSons;	// Requêtes des son à jouer

char *resolveOutput( int output )
{
	switch( output )
	{
		case FSOUND_OUTPUT_NOSOUND:	return "NoSound";						break;
		case FSOUND_OUTPUT_WINMM:	return "Windows Multimedia Waveout";	break;
		case FSOUND_OUTPUT_DSOUND:	return "Direct Sound";					break;
		case FSOUND_OUTPUT_ASIO:	return "ASIO";							break;
		case FSOUND_OUTPUT_ALSA:	return "ALSA 0.9-Advanced Linux Sound Architecture";	break;
		case FSOUND_OUTPUT_ESD:		return "ESD-Elightment Sound Daemon";	break;
		case FSOUND_OUTPUT_OSS:		return "OSS-Open Sound System";			break;
		default:					return "OUTPUT inconnu";		break;
	}
}

char *resolveMixer( int mixer )
{
	switch( mixer )
	{
		case FSOUND_MIXER_BLENDMODE:	return "FSOUND_MIXER_BLENDMODE";		break;
		case FSOUND_MIXER_MMXP5:		return "FSOUND_MIXER_MMXP5";			break;
		case FSOUND_MIXER_MMXP6:		return "FSOUND_MIXER_MMXP6";			break;
		case FSOUND_MIXER_QUALITY_FPU:	return "FSOUND_MIXER_QUALITY_FPU";		break;
		case FSOUND_MIXER_QUALITY_MMXP5:return "FSOUND_MIXER_QUALITY_MMXP5";	break;
		case FSOUND_MIXER_QUALITY_MMXP6:return "FSOUND_MIXER_QUALITY_MMXP6";	break;
		default:						return "MIXER inconnu";					break;
	}
}

const char *resolveDriver( int driver )
{
	return FSOUND_GetDriverName( driver );
}

const char *resolveDriverRecord( int record )
{
	return FSOUND_Record_GetDriverName( record );
}
