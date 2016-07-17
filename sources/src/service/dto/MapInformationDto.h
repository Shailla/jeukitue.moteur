/*
 * MapInformationDto.h
 *
 *  Created on: 29 avr. 2012
 *      Author: Erwin
 */

#ifndef MAPINFORMATIONDTO_H_
#define MAPINFORMATIONDTO_H_

#include <string>

/**
 * AseFileInformationDto is a DTO to contain general information about a MAP (name, number of players, ...).
 */
class MapInformationDto {
	std::string _mapFileFullName;
	std::string _mapFileMinimalName;
public:
	MapInformationDto();
	virtual ~MapInformationDto();

	// Setters
	void setMapFileFullName(const std::string& aseFileFullName);
	void setMapFileMinimalName(const std::string& aseFileMinimalName);

	// Getters
	std::string getMapFileFullName();
	std::string getMapFileMinimalName();
};

#endif /* MAPINFORMATIONDTO_H_ */
