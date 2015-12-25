/*
 * IpUtils.h
 *
 *  Created on: 16 ao�t 2013
 *      Author: Erwin
 */

#ifndef IPUTILS_H_
#define IPUTILS_H_

#include <string>

#include "SDL.h"
#include "SDL_net.h"

bool operator < (const IPaddress& adr1, const IPaddress& adr2);

namespace JktUtils {

class IpUtils {
public:
	IpUtils();
	virtual ~IpUtils();

	static std::string translateIp(IPaddress address);
	static std::string translateAddress(IPaddress address);
};

} /* namespace JktUtils */
#endif /* IPUTILS_H_ */
