/*
 * ConnectionFailedException.cpp
 *
 *  Created on: 21 juin 2013
 *      Author: vgdj7997
 */

#include <string>

#include "reseau/new/exception/ConnectionFailedException.h"

using namespace std;

ConnectionFailedException::ConnectionFailedException(void) : JktException("ConnectionFailedException") {
}

ConnectionFailedException::ConnectionFailedException(const string& message) : JktException("ConnectionFailedException", message) {
}

ConnectionFailedException::~ConnectionFailedException() {
}

