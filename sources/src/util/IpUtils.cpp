/*
 * IpUtils.cpp
 *
 *  Created on: 16 août 2013
 *      Author: Erwin
 */

#include <sstream>

using namespace std;

#include "util/IpUtils.h"

namespace JktUtils {

IpUtils::IpUtils() {
}

IpUtils::~IpUtils() {
}


string IpUtils::translateIp(IPaddress address) {
	stringstream str;
	Uint8* add = (Uint8*)&address.host;

	str << (int)(Uint8)(*(add+0)) << "." << (int)(Uint8)(*(add+1)) << "." << (int)(Uint8)(*(add+2)) << "." << (int)(Uint8)(*(add+3));

	return str.str();
}

string IpUtils::translateAddress(IPaddress address) {
	stringstream str;

	str << translateIp(address) << ":" << SDLNet_Read16(&address.port);

	return str.str();
}

} /* namespace JktUtils */
