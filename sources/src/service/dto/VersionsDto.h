/*
 * VersionsDto.h
 *
 *  Created on: 30 avr. 2012
 *      Author: Erwin
 */

#ifndef VERSIONSDTO_H_
#define VERSIONSDTO_H_

#include <string>

namespace jkt {

class VersionsDto {
	std::string _openGlVersion;
	std::string _gluVersion;
	std::string _fmodVersion;
	std::string _agarVersion;
public:
	VersionsDto(const std::string& openGlVersion, const std::string& gluVersion, const std::string& fmodVersion, const std::string& agarVersion);
	virtual ~VersionsDto();

	std::string getOpenGlVersion() const;
	std::string getGluVersion() const;
	std::string getFmodVersion() const;
	std::string getAgarVersion() const;
};

} /* namespace jkt */
#endif /* VERSIONSDTO_H_ */
