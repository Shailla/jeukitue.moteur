/*
 * MapInformationDto.h
 *
 *  Created on: 29 avr. 2012
 *      Author: Erwin
 */

#ifndef ASEINFORMATIONDTO_H_
#define ASEINFORMATIONDTO_H_

#include <string>

/**
 * AseInformationDto is a DTO to contain general information about a MAP (name, number of players, ...).
 */
class AseInformationDto {
	std::string _aseFileFullName;
	std::string _aseFileMinimalName;
public:
	AseInformationDto();
	virtual ~AseInformationDto();

	// Setters
	void setAseFileFullName(const std::string& aseFileFullName);
	void setAseFileMinimalName(const std::string& aseFileMinimalName);

	// Getters
	std::string getAseFileFullName();
	std::string getAseFileMinimalName();
};

#endif /* ASEINFORMATIONDTO_H_ */
