/*
 * VersionsDto.cpp
 *
 *  Created on: 30 avr. 2012
 *      Author: Erwin
 */

#include "service/dto/VersionsDto.h"

using namespace std;

namespace jkt {

VersionsDto::VersionsDto(const string& openGlVersion, const string& gluVersion, const string& fmodVersion, const string& agarVersion) {
	_openGlVersion = openGlVersion;
	_gluVersion = gluVersion;
	_fmodVersion = fmodVersion;
	_agarVersion = agarVersion;
}

VersionsDto::~VersionsDto() {
}

string VersionsDto::getAgarVersion() const {
	return _agarVersion;
}

string VersionsDto::getFmodVersion() const {
	return _fmodVersion;
}

string VersionsDto::getGluVersion() const {
	return _gluVersion;
}

string VersionsDto::getOpenGlVersion() const {
	return _openGlVersion;
}

} /* namespace jkt */
