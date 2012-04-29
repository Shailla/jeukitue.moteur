/*
 * MapInformationDto.h
 *
 *  Created on: 29 avr. 2012
 *      Author: Erwin
 */

#ifndef ASEFILEINFORMATIONDTO_H_
#define ASEFILEINFORMATIONDTO_H_

#include <string>

using namespace std;

/**
 * AseFileInformationDto is a DTO to contain general information about a MAP (name, number of players, ...).
 */
class AseFileInformationDto {
	string _aseFileFullName;
	string _aseFileMinimalName;
public:
	AseFileInformationDto();
	virtual ~AseFileInformationDto();

	// Setters
	void setAseFileFullName(const string& aseFileFullName);
	void setAseFileMinimalName(const string& aseFileMinimalName);

	// Getters
	string getAseFileFullName();
	string getAseFileMinimalName();
};

#endif /* MAPINFORMATIONDTO_H_ */
