/*
 * SolidAndTargettable.cpp
 *
 *  Created on: 29 juil. 2016
 *      Author: VGDJ7997
 */

#include "spatial/basic/SolidAndTargettable.h"

namespace jkt {

SolidAndTargettable::SolidAndTargettable() {
}

SolidAndTargettable::SolidAndTargettable(const SolidAndTargettable& other) {
}

SolidAndTargettable::~SolidAndTargettable() {
}

bool SolidAndTargettable::Contact(const float pos[3], float dist) {
	return false;
}

} /* namespace jkt */
