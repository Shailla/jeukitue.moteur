/*
 * MapInformationDto.h
 *
 *  Created on: 29 avr. 2012
 *      Author: Erwin
 */

#ifndef ASEFILEINFORMATIONDTO_H_
#define ASEFILEINFORMATIONDTO_H_

#include <string>

/**
 * AseFileInformationDto is a DTO to contain general information about a MAP (name, number of players, ...).
 */
class AseFileInformationDto {
	std::string _aseFileFullName;
	std::string _aseFileMinimalName;
public:
	AseFileInformationDto();
	virtual ~AseFileInformationDto();

	// Setters
	void setAseFileFullName(const std::string& aseFileFullName);
	void setAseFileMinimalName(const std::string& aseFileMinimalName);

	// Getters
	std::string getAseFileFullName();
	std::string getAseFileMinimalName();
};

#endif /* MAPINFORMATIONDTO_H_ */
