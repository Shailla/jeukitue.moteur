/*
 * InformationService.cpp
 *
 *  Created on: 29 avr. 2012
 *      Author: Erwin
 */

#include <string>
#include <sstream>
#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/glu.h>
#include <agar/core.h>
#include "fmod.h"

#include "service/InformationService.h"

using namespace std;

namespace jkt {

InformationService::InformationService() {
}

InformationService::~InformationService() {
}

VersionsDto InformationService::loadVersions() {
    AG_AgarVersion agarVersion;
    AG_GetVersion(&agarVersion);
    stringstream agarVersionStr;
    agarVersionStr << agarVersion.major << "." << agarVersion.minor << "." << agarVersion.patch << " [" << agarVersion.release << "]";

    stringstream fmodVersionStr;
    fmodVersionStr << FSOUND_GetVersion();

	return VersionsDto(
			string((char*)glGetString(GL_VERSION)),
			string((char*)gluGetString(GLU_VERSION)),
			fmodVersionStr.str(),
			agarVersionStr.str()
			);
}

} /* namespace jkt */
