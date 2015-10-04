/*
 * AlreadyExistingBrancheException.cpp
 *
 *  Created on: 7 mars 2013
 *      Author: vgdj7997
 */

#include "data/exception/AlreadyExistingBrancheException.h"

AlreadyExistingBrancheException::AlreadyExistingBrancheException(const std::string& message) : JktException("AlreadyExistingBrancheException", message) {
}

AlreadyExistingBrancheException::~AlreadyExistingBrancheException() {
}

