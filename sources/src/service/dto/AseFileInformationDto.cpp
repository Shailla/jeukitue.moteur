/*
 * MapInformationDto.cpp
 *
 *  Created on: 29 avr. 2012
 *      Author: Erwin
 */

#include "service/dto/AseFileInformationDto.h"

using namespace std;

AseFileInformationDto::AseFileInformationDto() {
}

AseFileInformationDto::~AseFileInformationDto() {
}


// Setters
void AseFileInformationDto::setAseFileFullName(const string& aseFileFullName) {
	_aseFileFullName = aseFileFullName;
}

void AseFileInformationDto::setAseFileMinimalName(const string& aseFileMinimalName) {
	_aseFileMinimalName = aseFileMinimalName;
}


// Getters
string AseFileInformationDto::getAseFileFullName() {
	return _aseFileFullName;
}

string AseFileInformationDto::getAseFileMinimalName() {
	return _aseFileMinimalName;
}

