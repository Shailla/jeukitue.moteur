/*
 * IpUtils.cpp
 *
 *  Created on: 16 août 2013
 *      Author: Erwin
 */

#include <sstream>

#include "util/IpUtils.h"

using namespace std;

bool operator < (const IPaddress& adr1, const IPaddress& adr2) {
	if(adr1.host == adr2.host) {
		return adr1.port < adr2.port;
	}
	else {
		return adr1.host < adr2.host;
	}
}

namespace jkt {

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

} /* namespace jkt */
