/*
 * InformationService.h
 *
 *  Created on: 29 avr. 2012
 *      Author: Erwin
 */

#ifndef INFORMATIONSERVICE_H_
#define INFORMATIONSERVICE_H_

#include <vector>

#include "service/dto/VersionsDto.h"

using namespace std;

namespace jkt {

/**
 * Service class providing access to MAP information.
 */
class InformationService {
public:
	InformationService();
	virtual ~InformationService();

	/** Read and return information about the versions of OpenGL, Agar, ... */
	static VersionsDto loadVersions();
};

} /* namespace jkt */

#endif /* MAPSERVICE_H_ */
