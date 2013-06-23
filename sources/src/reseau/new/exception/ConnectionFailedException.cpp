/*
 * ConnectionFailedException.cpp
 *
 *  Created on: 21 juin 2013
 *      Author: vgdj7997
 */

#include <string>

using namespace std;

#include "reseau/new/exception/ConnectionFailedException.h"

ConnectionFailedException::ConnectionFailedException(void) {
}

ConnectionFailedException::ConnectionFailedException(const string& message) : JktException(message) {
}

ConnectionFailedException::~ConnectionFailedException() {
}

