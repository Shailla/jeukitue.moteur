/*
 * MapInformationDto.h
 *
 *  Created on: 29 avr. 2012
 *      Author: Erwin
 */

#ifndef MAPINFORMATIONDTO_H_
#define MAPINFORMATIONDTO_H_

#include <string>

using namespace std;

/**
 * AseFileInformationDto is a DTO to contain general information about a MAP (name, number of players, ...).
 */
class MapInformationDto {
	string _mapFileFullName;
	string _mapFileMinimalName;
public:
	MapInformationDto();
	virtual ~MapInformationDto();

	// Setters
	void setMapFileFullName(const string& aseFileFullName);
	void setMapFileMinimalName(const string& aseFileMinimalName);

	// Getters
	string getMapFileFullName();
	string getMapFileMinimalName();
};

#endif /* MAPINFORMATIONDTO_H_ */
