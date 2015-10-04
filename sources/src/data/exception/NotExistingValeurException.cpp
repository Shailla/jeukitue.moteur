/*
 * NotExistingBrancheException.cpp
 *
 *  Created on: 7 mars 2013
 *      Author: vgdj7997
 */

#include "data/exception/NotExistingValeurException.h"

NotExistingValeurException::NotExistingValeurException(const std::string& message) : JktException("NotExistingValeurException", message) {
}

NotExistingValeurException::~NotExistingValeurException() {
}

