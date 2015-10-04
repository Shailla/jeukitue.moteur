/*
 * NotSpecifiedClientException.cpp
 *
 *  Created on: 7 mars 2013
 *      Author: vgdj7997
 */

#include "data/exception/NotSpecifiedClientException.h"

NotSpecifiedClientException::NotSpecifiedClientException(const std::string& message) : JktException("NotSpecifiedClientException", message) {
}

NotSpecifiedClientException::~NotSpecifiedClientException() {
}

