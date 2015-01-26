/*
 * NotExistingBrancheException.cpp
 *
 *  Created on: 7 mars 2013
 *      Author: vgdj7997
 */

#include "data/exception/NotExistingBrancheException.h"

NotExistingBrancheException::NotExistingBrancheException(const std::string& message) : JktException(message) {
}

NotExistingBrancheException::~NotExistingBrancheException() {
}

