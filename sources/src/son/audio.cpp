

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

#include "util/Trace.h"
#include "util/V3D.h"
#include "son/DemonSons.h"
#include "main/Cfg.h"

#include "audio.h"

using namespace jkt;

//char **remarquesDriver;
CDemonSons *DemonSons;	// Requêtes des son à jouer

const char *resolveOutput(int output)
{
	const char* result;

	switch( output )
	{
		case FSOUND_OUTPUT_NOSOUND:	result = "NoSound";						break;
		case FSOUND_OUTPUT_WINMM:	result = "Windows Multimedia Waveout";	break;
		case FSOUND_OUTPUT_DSOUND:	result = "Direct Sound";					break;
		case FSOUND_OUTPUT_ASIO:	result = "ASIO";							break;
		case FSOUND_OUTPUT_ALSA:	result = "ALSA 0.9-Advanced Linux Sound Architecture";	break;
		case FSOUND_OUTPUT_ESD:		result = "ESD-Elightment Sound Daemon";	break;
		case FSOUND_OUTPUT_OSS:		result = "OSS-Open Sound System";			break;
		default:					result = "OUTPUT inconnu";		break;
	}

	return result;
}

const char *resolveMixer(int mixer)
{
	const char* result;

	switch( mixer )
	{
		case FSOUND_MIXER_BLENDMODE:	result = "FSOUND_MIXER_BLENDMODE";		break;
		case FSOUND_MIXER_MMXP5:		result = "FSOUND_MIXER_MMXP5";			break;
		case FSOUND_MIXER_MMXP6:		result = "FSOUND_MIXER_MMXP6";			break;
		case FSOUND_MIXER_QUALITY_FPU:	result = "FSOUND_MIXER_QUALITY_FPU";		break;
		case FSOUND_MIXER_QUALITY_MMXP5:result = "FSOUND_MIXER_QUALITY_MMXP5";	break;
		case FSOUND_MIXER_QUALITY_MMXP6:result = "FSOUND_MIXER_QUALITY_MMXP6";	break;
		default:						result = "MIXER inconnu";					break;
	}

	return result;
}

const char *resolveDriver(int driver)
{
	return FSOUND_GetDriverName(driver);
}

const char *resolveDriverRecord(int record)
{
	return FSOUND_Record_GetDriverName(record);
}
