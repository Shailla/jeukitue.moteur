/*
 * PlayerInformationDto.cpp
 *
 *  Created on: 3 mars 2016
 *      Author: VGDJ7997
 */

#include <service/dto/PlayerInformationDto.h>

PlayerInformationDto::PlayerInformationDto() {
}

PlayerInformationDto::~PlayerInformationDto() {
}

string PlayerInformationDto::getName() const {
	return _name;
}

void PlayerInformationDto::setName(const string& name) {
	_name = name;
}

