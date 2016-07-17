/*
 * MapInformationDto.cpp
 *
 *  Created on: 29 avr. 2012
 *      Author: Erwin
 */

#include <service/dto/AseInformationDto.h>

using namespace std;

AseInformationDto::AseInformationDto() {
}

AseInformationDto::~AseInformationDto() {
}


// Setters
void AseInformationDto::setAseFileFullName(const string& aseFileFullName) {
	_aseFileFullName = aseFileFullName;
}

void AseInformationDto::setAseFileMinimalName(const string& aseFileMinimalName) {
	_aseFileMinimalName = aseFileMinimalName;
}


// Getters
string AseInformationDto::getAseFileFullName() {
	return _aseFileFullName;
}

string AseInformationDto::getAseFileMinimalName() {
	return _aseFileMinimalName;
}

