/*
 * NotExistingBrancheException.cpp
 *
 *  Created on: 7 mars 2013
 *      Author: vgdj7997
 */

#include "data/exception/DataCommunicationException.h"

DataCommunicationException::DataCommunicationException() : JktException("DataCommunicationException") {
}

DataCommunicationException::DataCommunicationException(const std::string& message) : JktException("DataCommunicationException", message) {
}

DataCommunicationException::~DataCommunicationException() {
}

