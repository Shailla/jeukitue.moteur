/*
 * NotConnectedException.cpp
 *
 *  Created on: 21 juin 2013
 *      Author: vgdj7997
 */

#include <string>

using namespace std;

#include "reseau/new/exception/NotConnectedException.h"

NotConnectedException::NotConnectedException(void) : JktException("NotConnectedException") {
}

NotConnectedException::NotConnectedException(const string& message) : JktException("NotConnectedException", message) {
}

NotConnectedException::~NotConnectedException() {
}
