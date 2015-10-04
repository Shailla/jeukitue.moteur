/*
 * UnavailableOperationException.cpp
 *
 *  Created on: 7 mars 2013
 *      Author: vgdj7997
 */

#include "data/exception/UnavailableOperationException.h"

UnavailableOperationException::UnavailableOperationException(const std::string& message) : JktException("UnavailableOperationException", message) {
}

UnavailableOperationException::~UnavailableOperationException() {
}

