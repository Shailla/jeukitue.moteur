/*
 * VersionsDto.h
 *
 *  Created on: 30 avr. 2012
 *      Author: Erwin
 */

#ifndef VERSIONSDTO_H_
#define VERSIONSDTO_H_

#include <string>

using namespace std;

namespace JktService {

class VersionsDto {
	string _openGlVersion;
	string _gluVersion;
	string _fmodVersion;
	string _agarVersion;
public:
	VersionsDto(const string& openGlVersion, const string& gluVersion, const string& fmodVersion, const string& agarVersion);
	virtual ~VersionsDto();

	string getOpenGlVersion() const;
	string getGluVersion() const;
	string getFmodVersion() const;
	string getAgarVersion() const;
};

} /* namespace JktService */
#endif /* VERSIONSDTO_H_ */
