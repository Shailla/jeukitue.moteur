/*
 * PlayerInformationDto.h
 *
 *  Created on: 3 mars 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_SERVICE_DTO_PLAYERINFORMATIONDTO_H_
#define SRC_SERVICE_DTO_PLAYERINFORMATIONDTO_H_

#include <string>

class PlayerInformationDto {
	std::string _name;
public:
	PlayerInformationDto();
	virtual ~PlayerInformationDto();

	std::string getName() const;
	void setName(const std::string& name);
};

#endif /* SRC_SERVICE_DTO_PLAYERINFORMATIONDTO_H_ */
